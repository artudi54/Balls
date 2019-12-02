#include "BallParameters.hpp"
#include <cmath>

namespace model {
    BallParameters::BallParameters(QObject *parent)
            : QObject(parent), speed(0.0, 0.0), radius(0.0), position() {}

    const std::pair<double, double> &BallParameters::getSpeed() const {
        return speed;
    }

    void BallParameters::setSpeed(const std::pair<double, double> &speed) {
        if (this->speed == speed)
            return;
        this->speed = speed;
        emit speedChanged(this->speed);
    }

    double BallParameters::getSpeedValue() const {
        return std::hypot(speed.first, speed.second);
    }

    void BallParameters::setSpeedValue(double speedValue) {
        double angle = getDirectionAngle();
        setSpeed({speedValue * std::cos(angle), speedValue * std::sin(angle)});
    }

    double BallParameters::getDirectionAngle() const {
        return std::atan2(speed.second, speed.first);
    }

    void BallParameters::setDirectionAngle(double directionAngle) {
        double speedValue = getSpeedValue();
        setSpeed({speedValue * std::cos(directionAngle), speedValue * std::sin(directionAngle)});
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

    const QPointF &BallParameters::getPosition() const {
        return position;
    }

    void BallParameters::setPosition(const QPointF &position) {
        if (this->position == position)
            return;
        this->position = position;
        emit positionChanged(this->position);
    }

    const QColor &BallParameters::getBorderColor() const {
        return borderColor;
    }

    void BallParameters::setBorderColor(const QColor &borderColor) {
        if (this->borderColor == borderColor)
            return;
        this->borderColor = borderColor;
        emit borderColorChanged(this->borderColor);
    }

    const QColor &BallParameters::getCircleColor() const {
        return circleColor;
    }

    void BallParameters::setCircleColor(const QColor &circleColor) {
        if (this->circleColor == circleColor)
            return;
        this->circleColor = circleColor;
        emit circleColorChanged(this->circleColor);
    }
}
