#include <QApplication>
#include <BallColider.hpp>


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    BallColider colider;
    colider.show();

    return app.exec();
}
