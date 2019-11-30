#include "BallParameters.hpp"

BallParameters::BallParameters(QObject* parent)
    : QObject(parent)
    , speed(0.0)
    , directionAngle(0.0)
    , radius(0.0)
    , position() {}

double BallParameters::getSpeed() const {
    return speed;
}

void BallParameters::setSpeed(double speed) {
    if (this-> speed == speed)
        return;
    this->speed = speed;
    emit speedChanged(this->speed);
}

double BallParameters::getDirectionAngle() const {
    return directionAngle;
}

void BallParameters::setDirectionAngle(double directionAngle) {
    if (this->directionAngle == directionAngle)
        return;
    this->directionAngle = directionAngle;
    emit directionAngleChanged(this->directionAngle);
}

double BallParameters::getRadius() const {
    return radius;
}

void BallParameters::setRadius(double radius) {
    if (this->radius == radius)
        return;
    this->radius = radius;
    emit radiusChanged(this->radius);
}

const QPointF& BallParameters::getPosition() const {
    return position;
}

void BallParameters::setPosition(const QPointF& position) {
    if (this->position == position)
        return;
    this->position = position;
    emit positionChanged(this->position);
}

const QColor& BallParameters::getBorderColor() const {
    return borderColor;
}

void BallParameters::setBorderColor(const QColor& borderColor) {
    if (this->borderColor == borderColor)
        return;
    this->borderColor = borderColor;
    emit borderColorChanged(this->borderColor);
}

const QColor& BallParameters::getCircleColor() const {
    return circleColor;
}

void BallParameters::setCircleColor(const QColor& circleColor) {
    if (this->circleColor == circleColor)
        return;
    this->circleColor = circleColor;
    emit circleColorChanged(this->circleColor);
}
