#include <QApplication>
#include <BallParameters.hpp>
#include <Ball.hpp>


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWidget widget;
    widget.resize(500, 500);

    BallParameters ballParameters;
    ballParameters.setRadius(50);
    ballParameters.setPosition(QPointF(250, 250));

    Ball ball(&ballParameters, &widget);

    widget.show();
    return app.exec();
}
