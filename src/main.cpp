#include <QApplication>
#include <BallCollider.hpp>


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    BallCollider collider;
    collider.show();

    return app.exec();
}
