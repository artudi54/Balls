#pragma once
#include <QObject>
#include <QSizeF>
#include <QTimer>
#include <BallParameters.hpp>

class BallAnimator : public QObject {
    Q_OBJECT
public:
    BallAnimator(BallParameters* redBallParameters, BallParameters* blueBallParameters, QSizeF areaSize, QObject* parent = nullptr);
    void randomPlaceBalls();
    void start(std::uint32_t updateDelay);

private:
    void setBallPositions();
    void setRandomBallPosition(BallParameters* ballParameters);

    void moveBalls();
    void updatePosition(BallParameters* ballParameters);

    void fixCollisions();

    void fixWallCollision(BallParameters* ballParameters);
    bool isOutsideRight(BallParameters* ballParameters);
    bool isOutsideTop(BallParameters* ballParameters);
    bool isOutsideLeft(BallParameters* ballParameters);
    bool isOutsideBottom(BallParameters* ballParameters);
    static bool isRightAngle(double angle);
    static bool isLeftAngle(double angle);
    static bool isTopAngle(double angle);
    static bool isBottomAngle(double angle);

    void fixBallCollision();
    bool circlesIntersecting() const;
    double calculateCollisionAngle() const;

    BallParameters* redBallParameters;
    BallParameters* blueBallParameters;
    QSizeF areaSize;
    std::uint32_t updateDelay;
    QTimer timer;
    bool ballsColliding;
};