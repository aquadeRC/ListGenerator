import QtQuick

QtObject {
    property color central_background_color: "#ebe8e8"
    property color background_color: "#ebe8e8"
    property color text_color: "#737c8e"
    property color button_color: "#b5b7bf"
    property color button_color_down: "#9da0ad"
    property color button_text_color: "#b5b7bf"
    property color icon_color: "black"

    property color switch_indicator_color: "#c0c7d6"

    property color field_bacground_color: "#ffffff"
    property color field_border_color: "#d2d6df"
    property color field_higlight_color: "#eceaea"

    property string font: "Montserrat"

    property string mode: "light_mode"

    function setDarkMode() {
        central_background_color = "#2c2c2c";
        background_color = "#4c4c4c";
        button_color = "#5e626a";
        button_color_down = "#9da0ad";
        button_text_color = "#9eaab5";
        text_color = "#909aa2";

        field_bacground_color = "#383838";
        field_border_color = "#8d919a";
        field_higlight_color = "#eceaea";

        mode = "dark_mode";

        console.log("Dark mode");
    }

    function setLightMode() {
        central_background_color = "#f7f8fa";
        background_color = "#ebe8e8";
        button_color = "#b5b7bf";
        button_color_down = "#9da0ad";
        button_text_color = "#b5b7bf";
        text_color = "#737c8e";

        field_bacground_color = "#ffffff";
        field_border_color = "#d2d6df";
        field_higlight_color = "#eceaea";

        mode = "light_mode";

        console.log("Light mode");
    }
}
