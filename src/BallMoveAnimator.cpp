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
    fixBallCollision();
}


void BallMoveAnimator::fixWallCollision(BallParameters *ballParameters) {
    double angle = ballParameters->getDirectionAngle();
    double speedX = ballParameters->getSpeed() * std::cos(angle);
    double speedY = ballParameters->getSpeed() * std::sin(angle);

    bool inverseX = (isOutsideRight(ballParameters) && isRightAngle(angle)) ||
                    (isOutsideLeft(ballParameters) && isLeftAngle(angle));
    if (inverseX)
        ballParameters->setDirectionAngle(std::atan2(speedY, -speedX));

    bool inverseY = (isOutsideTop(ballParameters) && isTopAngle(angle)) ||
                    (isOutsideBottom(ballParameters) && isBottomAngle(angle));
    if (inverseY)
        ballParameters->setDirectionAngle(std::atan2(-speedY, speedX));
}

bool BallMoveAnimator::isOutsideRight(BallParameters* ballParameters) {
    return ballParameters->getPosition().x() + ballParameters->getRadius() > areaSize.width();
}

bool BallMoveAnimator::isOutsideTop(BallParameters* ballParameters) {
    return ballParameters->getPosition().y() + ballParameters->getRadius() > areaSize.height();
}

bool BallMoveAnimator::isOutsideLeft(BallParameters* ballParameters) {
    return ballParameters->getPosition().x() - ballParameters->getRadius() < 0.0;
}

bool BallMoveAnimator::isOutsideBottom(BallParameters* ballParameters) {
    return ballParameters->getPosition().y() - ballParameters->getRadius() < 0.0;
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


void BallMoveAnimator::fixBallCollision() {
    double lSpeed = redBallParameters->getSpeed();
    double lAngle = redBallParameters->getDirectionAngle();
    double rSpeed = blueBallParameters->getSpeed();
    double rAngle = blueBallParameters->getDirectionAngle();

    if (circlesIntersecting()) {
        if (!ballsColliding) {
            double collisionAngle = calculateCollisionAngle();

            double lSpeedU = lSpeed * std::cos(lAngle - collisionAngle);
            double lSpeedW = lSpeed * std::sin(lAngle - collisionAngle);
            double rSpeedU = rSpeed * std::cos(rAngle - collisionAngle);
            double rSpeedW = rSpeed * std::sin(rAngle - collisionAngle);
            std::swap(lSpeedU, rSpeedU);

            double lSpeedP = std::hypot(lSpeedU, lSpeedW);
            double rSpeedP = std::hypot(rSpeedU, rSpeedW);

            double lAngleP = std::atan2(lSpeedW, lSpeedU);
            double rAngleP = std::atan2(rSpeedW, rSpeedU);

            double lSpeedX = lSpeedP * std::cos(lAngleP + collisionAngle);
            double lSpeedY = lSpeedP * std::sin(lAngleP + collisionAngle);
            double rSpeedX = rSpeedP * std::cos(rAngleP + collisionAngle);
            double rSpeedY = rSpeedP * std::sin(rAngleP + collisionAngle);

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

bool BallMoveAnimator::circlesIntersecting() const {
    QPointF redPoint = redBallParameters->getPosition();
    QPointF bluePoint = blueBallParameters->getPosition();
    double redRadius = redBallParameters->getRadius();
    double blueRadius = blueBallParameters->getRadius();
    double distance = std::hypot(redPoint.x() - bluePoint.x(), redPoint.y() - bluePoint.y());
    return distance < redRadius + blueRadius;
}

double BallMoveAnimator::calculateCollisionAngle() const {
    QPointF redPoint = redBallParameters->getPosition();
    QPointF bluePoint = blueBallParameters->getPosition();
    return std::atan2(redPoint.y() - bluePoint.y(), redPoint.x() - bluePoint.x());
}
