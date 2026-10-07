#include <Diagrams/diagrams.hpp>

#include <print>
#include <qdialog.h>
#include <vector>
#include <QDialog>

////
#include <QQuickWidget>
#include <QBarSeries>
#include <QBarSet>
#include <QQuickItem>
#include <QValueAxis>
#include <QBarCategoryAxis>

////
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


namespace Diagram {

void TEST_TEST() {
    std::print("TEST_TEST");
}

Diagram::Diagram(QWidget * parent)
    : QDialog(parent) {
    TEST_TEST();
    
    // --- Основной лейаут
    QHBoxLayout * hLayout = new QHBoxLayout(this);

    hLayout->setContentsMargins(0, 0, 0, 0);

    QQuickWidget * graph1 = new QQuickWidget(QUrl("qrc:/Diagrams/graphs.qml"), this);
    hLayout->addWidget(graph1);
    graph1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_barSeries = graph1->rootObject()->findChild<QBarSeries * >("barSeries1");

    m_barAxis = graph1->rootObject()->findChild<QBarCategoryAxis * >("BarCategoryAxis1");

    m_valueAxis = graph1->rootObject()->findChild<QValueAxis * >("ValueAxis1");
}

void Diagram::show() {
    this->exec();
}

void Diagram::setBarAxis(std::vector<std::string> & axisList) {
    m_barAxis->clear();

    for (auto & str : axisList) {
        m_barAxis->append(QString::fromStdString(str));
    }
}

void Diagram::setValueAxis(const std::string &name, double min, double max) {
    m_valueAxis->setMin(min);
    m_valueAxis->setMax(max);
}


void Diagram::addSet(const std::string &name, const std::vector<double> &data) {
    auto set = new QBarSet(QString::fromStdString(name));

    for (auto num : data) {
        *set << num;

        if (m_valueAxis->min() > num) {
            m_valueAxis->setMin(num);
        } else if (m_valueAxis->max() < num) {
            m_valueAxis->setMax(num * 1.1);
        }
    }

    m_barSeries->append(set);
}

void Diagram::cleanSets() {
    m_barSeries->clear();
}

}
