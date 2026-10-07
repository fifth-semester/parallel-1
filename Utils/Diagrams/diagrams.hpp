#pragma once

extern void TEST_TEST();

#include <QWidget>
#include <vector>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <QDialog>

#include <QApplication>

namespace Diagram {


class Diagram : private QDialog {
  public:

    Diagram(QWidget * parent = nullptr);

    void show();

    void setBarAxis(std::vector<std::string> & axisList);
    void setValueAxis(const std::string &name, double min, double max);

    void addSet(const std::string &name, const std::vector<double> &data);
    void cleanSets();

  private:

    QBarSeries * m_barSeries;
    std::vector<QBarSet> m_barSets;
    QBarCategoryAxis * m_barAxis;
    QValueAxis * m_valueAxis;
};



}
