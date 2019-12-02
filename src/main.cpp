#include <QApplication>
#include <gui/BallWidget.hpp>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    gui::BallWidget ballWidget;
    ballWidget.show();

    return app.exec();
}
