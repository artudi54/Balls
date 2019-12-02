#pragma once
#include <QObject>
#include <QPointF>
#include <QColor>

class BallParameters : public QObject {
    Q_OBJECT
public:
    explicit BallParameters(QObject* parent = nullptr);

    const std::pair<double, double>& getSpeed() const;
    void setSpeed(const std::pair<double, double>& speed);

    double getSpeedValue() const;
    void setSpeedValue(double speedValue);

    double getDirectionAngle() const;
    void setDirectionAngle(double directionAngle);

    double getRadius() const;
    void setRadius(double radius);

    const QPointF& getPosition() const;
    void setPosition(const QPointF& position);

    const QColor& getBorderColor() const;
    void setBorderColor(const QColor& borderColor);

    const QColor& getCircleColor() const;
    void setCircleColor(const QColor& circleColor);

signals:
    void speedChanged(const std::pair<double, double>& speed);
    void radiusChanged(double radius);
    void positionChanged(const QPointF& position);
    void borderColorChanged(const QColor& borderColor);
    void circleColorChanged(const QColor& circleColor);

private:
    std::pair<double, double> speed;
    double radius;
    QPointF position;
    QColor borderColor;
    QColor circleColor;
};
