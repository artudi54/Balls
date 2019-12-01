#include "BallMoveAnimator.hpp"
#include <cmath>

BallMoveAnimator::BallMoveAnimator(BallParameters *redBallParameters, BallParameters *blueBallParameters,
                                   QSizeF areaSize, std::uint32_t updateDelay, QObject *parent)
    : QObject(parent)
    , redBallParameters(redBallParameters)
    , blueBallParameters(blueBallParameters)
    , areaSize(areaSize)
    , updateDelay(updateDelay)
    , timer(this)
    , ballsColliding(false) {
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

    if ((isOutsideRight(radius, position) && isRightAngle(angle)) ||
        (isOutsideLeft(radius, position) && isLeftAngle(angle)))
        ballParameters->setDirectionAngle(std::atan2(speedY, -speedX));
    else if ((isOutsideTop(radius, position) && isTopAngle(angle)) ||
             (isOutsideBottom(radius, position) && isBottomAngle(angle)))
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

bool BallMoveAnimator::isRightAngle(double angle) {
    return angle >= -M_PI_2 && angle <= M_PI_2;
}

bool BallMoveAnimator::isLeftAngle(double angle) {
    return angle <= -M_PI_2 || angle >= M_PI_2;
}

bool BallMoveAnimator::isTopAngle(double angle) {
    return angle >= 0.0;
}

bool BallMoveAnimator::isBottomAngle(double angle) {
    return angle <= 0.0;
}


void BallMoveAnimator::fixBallColision() {
    QPointF lPoint = redBallParameters->getPosition();
    QPointF rPoint = blueBallParameters->getPosition();
    double lRadius = redBallParameters->getRadius();
    double rRadius = blueBallParameters->getRadius();
    double distance = std::hypot(lPoint.x() - rPoint.x(), lPoint.y() - rPoint.y());
    double lSpeed = redBallParameters->getSpeed();
    double rSpeed= blueBallParameters->getSpeed();
    double lAngle = redBallParameters->getDirectionAngle();
    double rAngle = blueBallParameters->getDirectionAngle();

    if (distance < lRadius + rRadius) {
        if (!ballsColliding) {
            double collisionAngle = std::atan2(lPoint.y() - rPoint.y(), lPoint.x() - rPoint.x());

            double lSpeedX = lSpeed * std::cos(lAngle);
            double lSpeedY = lSpeed * std::sin(lAngle);
            double rSpeedX = rSpeed * std::cos(rAngle);
            double rSpeedY = rSpeed * std::sin(rAngle);


            double lSpeedU = lSpeed * std::cos(lAngle - collisionAngle);
            double lSpeedW = lSpeed * std::sin(lAngle - collisionAngle);
            double rSpeedU = rSpeed * std::cos(rAngle - collisionAngle);
            double rSpeedW = rSpeed * std::sin(rAngle - collisionAngle);


            std::swap(lSpeedU, rSpeedU);

            double lSpeedP = std::hypot(lSpeedU, lSpeedW);
            double rSpeedP = std::hypot(rSpeedU, rSpeedW);


            double lAngleP = std::atan2(lSpeedW, lSpeedU);
            double rAngleP = std::atan2(rSpeedW, rSpeedU);

            lSpeedX = lSpeedP * std::cos(lAngleP + collisionAngle);
            lSpeedY = lSpeedP * std::sin(lAngleP + collisionAngle);
            rSpeedX = rSpeedP * std::cos(rAngleP + collisionAngle);
            rSpeedY = rSpeedP * std::sin(rAngleP + collisionAngle);


            redBallParameters->setDirectionAngle(std::atan2(lSpeedY, lSpeedX));
            redBallParameters->setSpeed(std::hypot(lSpeedX, lSpeedY));
            blueBallParameters->setDirectionAngle(std::atan2(rSpeedY, rSpeedX));
            blueBallParameters->setSpeed(std::hypot(rSpeedX, rSpeedY));


            ballsColliding = true;
        }
    }
    else {
        ballsColliding = false;
    }
}