#pragma once
#include <QObject>
#include <QPointF>
#include <QColor>

class BallParameters : public QObject {
    Q_OBJECT
public:
    explicit BallParameters(QObject* parent = nullptr);
    
    double getSpeed() const;
    void setSpeed(double speed);

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
    void speedChanged();
    void directionAngleChanged();
    void radiusChanged();
    void positionChanged();
    void borderColorChanged();
    void circleColorChanged();

private:
    double speed;
    double directionAngle;
    double radius;
    QPointF position;
    QColor borderColor;
    QColor circleColor;
};
