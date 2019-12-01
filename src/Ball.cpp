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
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int fillWidth = 2;
    painter.setBrush(QBrush(ballParameters->getCircleColor()));
    painter.setPen(QPen(ballParameters->getCircleColor(), fillWidth));
    painter.drawEllipse(fillWidth / 2, fillWidth / 2, width() - fillWidth, height() - fillWidth);

    int borderWidth = 4;
    painter.setBrush(QBrush());
    painter.setPen(QPen(ballParameters->getBorderColor(), borderWidth));
    painter.drawEllipse(borderWidth / 2, borderWidth / 2, width() - borderWidth, height() - borderWidth);
}

void Ball::updateBallGeometry() {
    int size = static_cast<int>(std::round(ballParameters->getRadius()));
    move(ballParameters->getPosition().toPoint() - QPoint(size, size));
    resize(size * 2, size * 2);
    scheduleBallRedraw();
}

void Ball::scheduleBallRedraw() {
    repaint();
}
