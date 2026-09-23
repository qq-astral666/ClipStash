import QtQuick
import QtQuick.Shapes

// Stroke icons drawn from SVG path data (24×24 grid, Lucide-style),
// so the app ships without any image assets.
Item {
    id: root

    property string name: ""
    property color color: "white"
    property real size: 16
    property real strokeWidth: 1.9

    implicitWidth: size
    implicitHeight: size
    width: size
    height: size

    readonly property var paths: ({
        "search": "M4 11a7 7 0 1 0 14 0a7 7 0 1 0 -14 0 M20 20l-4.35 -4.35",
        "text": "M5 7V5h14v2 M12 5v14 M9 19h6",
        "link": "M10 13a5 5 0 0 0 7.54 .54l3 -3a5 5 0 0 0 -7.07 -7.07l-1.72 1.71 M14 11a5 5 0 0 0 -7.54 -.54l-3 3a5 5 0 0 0 7.07 7.07l1.71 -1.71",
        "image": "M5 3h14a2 2 0 0 1 2 2v14a2 2 0 0 1 -2 2H5a2 2 0 0 1 -2 -2V5a2 2 0 0 1 2 -2z M7.5 9a1.5 1.5 0 1 0 3 0a1.5 1.5 0 1 0 -3 0 M21 15l-5 -5L5 21",
        "file": "M14 3H6a2 2 0 0 0 -2 2v14a2 2 0 0 0 2 2h12a2 2 0 0 0 2 -2V9z M14 3v6h6",
        "droplet": "M12 3c0 0 -6 6.5 -6 11a6 6 0 0 0 12 0c0 -4.5 -6 -11 -6 -11z",
        "pin": "M12 17v5 M9 4h6 M10 4v6l-3 4h10l-3 -4V4",
        "clipboard": "M9 3h6a1 1 0 0 1 1 1v2a1 1 0 0 1 -1 1H9a1 1 0 0 1 -1 -1V4a1 1 0 0 1 1 -1z M8 5H7a2 2 0 0 0 -2 2v12a2 2 0 0 0 2 2h10a2 2 0 0 0 2 -2V7a2 2 0 0 0 -2 -2h-1 M9 12h6 M9 16h4",
        "settings": "M4 6h9 M17 6h3 M4 12h3 M11 12h9 M4 18h11 M19 18h1 M15 4v4 M9 10v4 M17 16v4",
        "back": "M15 18l-6 -6l6 -6",
        "external": "M15 3h6v6 M10 14L21 3 M18 13v6a2 2 0 0 1 -2 2H5a2 2 0 0 1 -2 -2V8a2 2 0 0 1 2 -2h6",
        "trash": "M3 6h18 M8 6V4h8v2 M6 6l1 14h10l1 -14",
        "pause": "M8 5v14 M16 5v14",
        "check": "M5 12l5 5L20 7"
    })

    Shape {
        width: 24
        height: 24
        scale: root.size / 24
        transformOrigin: Item.TopLeft
        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            strokeColor: root.color
            strokeWidth: root.strokeWidth
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin
            PathSvg { path: root.paths[root.name] ?? "" }
        }
    }
}
