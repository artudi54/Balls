#include "Ball.hpp"
#include <QPainter>
#include <cmath>

Ball::Ball(BallParameters* ballParameters, QWidget* parent)
    : QWidget(parent)
    , ballParameters(ballParameters) {
    connect(ballParameters, &BallParameters::speedChanged, this, &Ball::scheduleBallRedraw); 
    connect(ballParameters, &BallParameters::directionAngleChanged, this, &Ball::scheduleBallRedraw); 
    connect(ballParameters, &BallParameters::radiusChanged, this, &Ball::updateBallGeometry); 
    connect(ballParameters, &BallParameters::positionChanged, this, &Ball::updateBallGeometry); 
    connect(ballParameters, &BallParameters::borderColorChanged, this, &Ball::scheduleBallRedraw); 
    connect(ballParameters, &BallParameters::circleColorChanged, this, &Ball::scheduleBallRedraw); 

    updateBallGeometry();
}

void Ball::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
}

void Ball::updateBallGeometry() {
    int size = static_cast<int>(std::round(2 * ballParameters->getRadius()));
    move(ballParameters->getPosition().toPoint());
    resize(size, size);
    scheduleBallRedraw();
}

void Ball::scheduleBallRedraw() {
    repaint();
}
