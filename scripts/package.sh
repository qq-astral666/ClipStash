#!/usr/bin/env bash
# Builds a self-contained, ad-hoc signed ClipStash.app + ClipStash.dmg.
# Usage: ./scripts/package.sh        (run from the project root)
set -euo pipefail

QT="$(brew --prefix qt)"
BREW="$(brew --prefix)"
OUT=build-release
APP="$OUT/ClipStash.app"

# Start from a clean bundle (macdeployqt is not idempotent). This must happen
# BEFORE configuring: CMake writes Contents/Info.plist at configure time, and
# without it the app loses its icon, bundle id and menu-bar-only mode.
rm -rf "$APP" "$OUT/ClipStash.dmg"

cmake -S . -B "$OUT" -G Ninja -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH="$QT" -DCLIPSTASH_BUILD_TESTS=OFF
cmake --build "$OUT"
test -f "$APP/Contents/Info.plist" || { echo "Info.plist missing"; exit 1; }

# Homebrew splits Qt into many kegs and links deps via @rpath, so
# macdeployqt needs explicit search paths to find them.
"$QT/bin/macdeployqt" "$APP" \
    -qmldir=qml \
    -libpath="$QT/lib" \
    -libpath="$BREW/lib" \
    -no-strip

# --- Make the bundle independent of Homebrew -------------------------------
# 1) macdeployqt copies plugins (virtual keyboard, Qt3D, PDF…) whose Qt
#    frameworks it does not bundle. They can only load from Homebrew, so drop
#    every plugin that needs a framework missing from Contents/Frameworks.
find "$APP/Contents/PlugIns" "$APP/Contents/Resources/qml" -name '*.dylib' 2>/dev/null |
    while read -r plugin; do
        for fw in $(otool -L "$plugin" | awk '{print $1}' | sed -n 's#^@rpath/\([^/]*\.framework\)/.*#\1#p'); do
            if [ ! -d "$APP/Contents/Frameworks/$fw" ]; then
                echo "  drop ${plugin#"$APP/Contents/"} (needs $fw)"
                rm -f "$plugin"
                break
            fi
        done
    done
# macdeployqt links QML plugins from Resources/qml into PlugIns; drop the
# links left dangling above, or codesign fails with "No such file or directory".
find "$APP/Contents" -type l ! -exec test -e {} \; -print -delete
# 2) The linker leaves Homebrew's Qt dir as an LC_RPATH. dyld then resolves
#    @rpath/Qt* to /opt/homebrew, two QtCores get loaded, and after any
#    `brew upgrade qt` the app segfaults on launch. Strip absolute rpaths.
find "$APP/Contents" -type f \( -perm -u+x -o -name '*.dylib' \) | while read -r bin; do
    otool -l "$bin" 2>/dev/null | awk '/LC_RPATH/{f=1} f&&/path /{print $2; f=0}' |
        { grep '^/' || true; } | while read -r rp; do
            install_name_tool -delete_rpath "$rp" "$bin"
        done
done
# 3) Fail loudly instead of shipping a bundle that depends on Homebrew.
if find "$APP/Contents" -type f \( -perm -u+x -o -name '*.dylib' \) -exec otool -l {} \; 2>/dev/null |
        grep -A2 LC_RPATH | grep -q 'path /'; then
    echo "absolute rpath left in bundle"; exit 1
fi
# ---------------------------------------------------------------------------

# Re-sign after install_name_tool rewrote the libraries.
codesign --force --deep --sign - "$APP"
codesign --verify --deep --strict "$APP"

hdiutil create -volname ClipStash -srcfolder "$APP" -ov -format UDZO "$OUT/ClipStash.dmg" >/dev/null

echo
echo "Done:"
echo "  $APP"
echo "  $OUT/ClipStash.dmg"
