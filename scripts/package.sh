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

# Re-sign after install_name_tool rewrote the libraries.
codesign --force --deep --sign - "$APP"
codesign --verify --deep --strict "$APP"

hdiutil create -volname ClipStash -srcfolder "$APP" -ov -format UDZO "$OUT/ClipStash.dmg" >/dev/null

echo
echo "Done:"
echo "  $APP"
echo "  $OUT/ClipStash.dmg"
