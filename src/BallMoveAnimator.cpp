#include "BallMoveAnimator.hpp"
#include <cmath>

BallMoveAnimator::BallMoveAnimator(BallParameters *redBallParameters, BallParameters *blueBallParameters,
                                   QSizeF areaSize, std::uint32_t updateDelay, QObject *parent)
    : QObject(parent)
    , redBallParameters(redBallParameters)
    , blueBallParameters(blueBallParameters)
    , areaSize(areaSize)
    , updateDelay(updateDelay)
    , timer(this) {
    connect(&timer, &QTimer::timeout, this, &BallMoveAnimator::moveBalls);
    timer.setInterval(static_cast<int>(updateDelay));
}

void BallMoveAnimator::start() {
    timer.start();
}

void BallMoveAnimator::moveBalls() {
    updatePosition(redBallParameters);
    updatePosition(blueBallParameters);
    fixCollisions();
}

void BallMoveAnimator::updatePosition(BallParameters *ballParameters) {
    double time = updateDelay / 1000.0;
    double distance = ballParameters->getSpeed() * time;
    double distanceX = std::cos(ballParameters->getDirectionAngle()) * distance;
    double distanceY = std::sin(ballParameters->getDirectionAngle()) * distance;
    QPointF nextPosition = ballParameters->getPosition() + QPointF(distanceX, distanceY);
    ballParameters->setPosition(nextPosition);
}

void BallMoveAnimator::fixCollisions() {
    fixWallCollision(redBallParameters);
    fixWallCollision(blueBallParameters);
    fixBallColision();
}


void BallMoveAnimator::fixWallCollision(BallParameters *ballParameters) {
    double radius = ballParameters->getRadius();
    double angle = ballParameters->getDirectionAngle();
    QPointF position = ballParameters->getPosition();
    double speedX = ballParameters->getSpeed() * std::cos(angle);
    double speedY = ballParameters->getSpeed() * std::sin(angle);

    if (isOutsideRight(radius, position) || isOutsideLeft(radius, position))
        ballParameters->setDirectionAngle(std::atan2(speedY, -speedX));
    else if (isOutsideTop(radius, position) || isOutsideBottom(radius, position))
        ballParameters->setDirectionAngle(std::atan2(-speedY, speedX));
}

bool BallMoveAnimator::isOutsideRight(double radius, QPointF position) {
    return position.x() + radius > areaSize.width();
}

bool BallMoveAnimator::isOutsideTop(double radius, QPointF position) {
    return position.y() + radius > areaSize.height();
}

bool BallMoveAnimator::isOutsideLeft(double radius, QPointF position) {
    return position.x() - radius < 0.0;
}

bool BallMoveAnimator::isOutsideBottom(double radius, QPointF position) {
    return position.y() - radius < 0.0;
}

bool BallMoveAnimator::fixBallColision() {
    QPointF lPoint = redBallParameters->getPosition();
    QPointF rPoint = blueBallParameters->getPosition();
    double lRadius = redBallParameters->getRadius();
    double rRadius = blueBallParameters->getRadius();
    double distance = std::hypot(lPoint.x() - rPoint.x(), lPoint.y() - rPoint.y());
    if (distance < lRadius + rRadius) {

    }
}
