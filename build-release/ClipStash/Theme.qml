pragma Singleton
import QtQuick

// Design tokens + small formatting helpers shared by all views.
QtObject {
    id: theme

    property int mode: 0            // 0 = system, 1 = dark, 2 = light
    property bool glass: false      // native frosted glass behind the card

    readonly property bool dark: mode === 1
        || (mode === 0 && Application.styleHints.colorScheme === Qt.ColorScheme.Dark)

    // ---- surfaces
    readonly property color windowBg: dark
        ? Qt.rgba(0.106, 0.106, 0.122, glass ? 0.74 : 0.985)
        : Qt.rgba(0.988, 0.988, 0.992, glass ? 0.78 : 0.99)
    readonly property color border: dark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.11)
    readonly property color divider: dark ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(0, 0, 0, 0.07)
    readonly property color surface: dark ? Qt.rgba(1, 1, 1, 0.045) : Qt.rgba(0, 0, 0, 0.03)
    readonly property color selection: dark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.065)
    readonly property color hover: dark ? Qt.rgba(1, 1, 1, 0.05) : Qt.rgba(0, 0, 0, 0.035)
    readonly property color keycap: dark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.055)
    readonly property color segmentActive: dark ? Qt.rgba(1, 1, 1, 0.13) : "#ffffff"
    readonly property color trackOff: dark ? Qt.rgba(1, 1, 1, 0.16) : Qt.rgba(0, 0, 0, 0.14)
    readonly property color toastBg: dark ? "#3a3a40" : "#1c1c1f"

    // ---- text
    readonly property color text: dark ? "#f4f4f6" : "#18181b"
    readonly property color textSecondary: dark ? Qt.rgba(1, 1, 1, 0.58) : Qt.rgba(0, 0, 0, 0.56)
    readonly property color textTertiary: dark ? Qt.rgba(1, 1, 1, 0.36) : Qt.rgba(0, 0, 0, 0.36)

    // ---- accent
    readonly property color accent: "#7c5cff"
    readonly property color accentSoft: dark ? Qt.rgba(0.49, 0.36, 1, 0.22) : Qt.rgba(0.49, 0.36, 1, 0.12)
    readonly property color accentText: dark ? "#bcaeff" : "#5a3fe0"
    readonly property color danger: "#ff453a"
    readonly property color dangerSoft: Qt.rgba(1, 0.27, 0.23, dark ? 0.16 : 0.10)
    readonly property color dangerHover: Qt.rgba(1, 0.27, 0.23, dark ? 0.26 : 0.18)
    readonly property color success: "#30d158"
    readonly property color warning: "#ff9f0a"

    readonly property string monoFont: Qt.platform.os === "osx" ? "Menlo" : "monospace"
    readonly property var ru: Qt.locale("ru_RU")

    // ---- clip types: 0 text, 1 link, 2 image, 3 color, 4 file
    readonly property var _typeNames: ["Текст", "Ссылка", "Изображение", "Цвет", "Файлы"]
    readonly property var _typeIcons: ["text", "link", "image", "droplet", "file"]
    readonly property var _typeRgb: [[0.37, 0.62, 1.0], [0.19, 0.82, 0.35], [1.0, 0.62, 0.04],
                                     [0.75, 0.35, 0.95], [0.39, 0.82, 1.0]]

    function typeName(t) { return _typeNames[t] ?? _typeNames[0] }
    function typeIcon(t) { return _typeIcons[t] ?? _typeIcons[0] }
    function typeColor(t) {
        const c = _typeRgb[t] ?? _typeRgb[0]
        return Qt.rgba(c[0], c[1], c[2], 1)
    }
    function typeTint(t) {
        const c = _typeRgb[t] ?? _typeRgb[0]
        return Qt.rgba(c[0], c[1], c[2], dark ? 0.20 : 0.14)
    }

    // ---- formatting
    function plural(n, one, few, many) {
        const m10 = n % 10, m100 = n % 100
        if (m10 === 1 && m100 !== 11) return one
        if (m10 >= 2 && m10 <= 4 && (m100 < 10 || m100 >= 20)) return few
        return many
    }

    function shortTime(d) {
        if (!d || isNaN(d.getTime())) return ""
        const now = new Date()
        const sec = (now.getTime() - d.getTime()) / 1000
        if (sec < 60) return "сейчас"
        if (sec < 3600) return Math.floor(sec / 60) + " мин"
        if (d.toDateString() === now.toDateString()) return d.toLocaleTimeString(ru, "HH:mm")
        const yesterday = new Date(now.getTime() - 86400000)
        if (d.toDateString() === yesterday.toDateString()) return "вчера"
        return d.toLocaleDateString(ru, "d MMM")
    }

    function fullDate(d) {
        if (!d || isNaN(d.getTime())) return ""
        return d.toLocaleString(ru, "d MMMM yyyy, HH:mm")
    }

    function domainOf(url) {
        const m = String(url).trim().match(/^(?:[a-z]+:\/\/)?(?:www\.)?([^\/?#:]+)/i)
        return m ? m[1] : url
    }
}
