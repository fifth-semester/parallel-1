import QtQuick
import QtGraphs
import QtQuick.Layouts

Rectangle {
    anchors.fill: parent
    color: graphView.theme.backgroundColor

    GraphsView {
        id: graphView

        anchors.fill: parent

        theme: GraphsTheme {
            colorScheme: GraphsTheme.ColorScheme.Dark
            theme: GraphsTheme.Theme.QtGreen
        }

        axisX: BarCategoryAxis {
            objectName: "BarCategoryAxis1"
            categories: [2024, 2025, 2026]
            gridVisible: false // Убираем сетку для чистоты
        }

        axisY: ValueAxis {
            objectName: "ValueAxis1"
            min: 0
            max: 0
            gridVisible: false // Убираем сетку для чистоты
        }

        BarSeries {
            id: barSeries
            objectName: "barSeries1"

            BarSet {
                label: "Продажи"
                values: [45, 78, 62]   // 3 значения для 3 категорий (2024, 2025, 2026)
            }
            BarSet {
                label: "Расходы"
                values: [30, 55, 40]
            }
        }
    }

    Row {
        id: legend
        anchors.top: parent.top
        anchors.left: parent.left
        spacing: 12

        leftPadding: 10

        Repeater {
            model: barSeries.barSets

            delegate: Row {
                spacing: 8

                Rectangle {
                    width: 16
                    height: 16

                    // Берём цвет из темы по индексу набора
                    color: {
                        var colors = graphView.theme.seriesColors;
                        if (!colors || colors.length === 0)
                            return "transparent"; // fallback
                        return colors[index % colors.length];
                    }

                    border.color: {
                        var borders = graphView.theme.borderColors;
                        if (!borders || borders.length === 0)
                            return "white";
                        return borders[index % borders.length];
                    }
                    border.width: 1

                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: modelData.label
                    color: graphView.theme.labelTextColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
