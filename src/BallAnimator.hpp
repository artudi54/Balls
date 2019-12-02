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

    void handleWallCollision(BallParameters* ballParameters);
    bool isOutsideRight(BallParameters* ballParameters);
    bool isOutsideTop(BallParameters* ballParameters);
    bool isOutsideLeft(BallParameters* ballParameters);
    bool isOutsideBottom(BallParameters* ballParameters);
    static bool isMovingRight(const std::pair<double, double>& speed);
    static bool isMovingLeft(const std::pair<double, double>& speed);
    static bool isMovingTop(const std::pair<double, double>& speed);
    static bool isMovingBottom(const std::pair<double, double>& speed);

    void handleBallCollision();
    void adjustBallSpeeds();
    bool circlesIntersecting() const;
    double calculateCollisionAngle() const;

    BallParameters* redBallParameters;
    BallParameters* blueBallParameters;
    QSizeF areaSize;
    QTimer timer;
    bool ballCollisionHandled;
};