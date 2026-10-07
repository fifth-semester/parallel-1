#include <Diagrams/diagrams.hpp>
#include <QApplication>

int main(int argc, char * * argv) {
    QApplication app(argc, argv);

    Diagram::Diagram diagram{};
    diagram.show();

    std::vector<std::string> strs{"Случай 1", "Случай 2", "Случай 3"};

    diagram.cleanSets();

    diagram.addSet("1 поток", {1, 2, 3});
    diagram.addSet("4 потока", {2, 1, 4});
    diagram.addSet("8 потока", {2, 1, 4});
    diagram.addSet("16 потока", {20, 1, 4});
    diagram.setBarAxis(strs);
 //   diagram.setValueAxis("12312", 0, 10);

    app.exec();
}
