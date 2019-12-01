#include "BallAnimator.hpp"
#include <cmath>
#include <ctime>
#include <random>

static std::mt19937 randomEngine(static_cast<std::mt19937::result_type>(std::time(nullptr)));

BallAnimator::BallAnimator(BallParameters *redBallParameters, BallParameters *blueBallParameters, QSizeF areaSize, QObject *parent)
    : QObject(parent)
    , redBallParameters(redBallParameters)
    , blueBallParameters(blueBallParameters)
    , areaSize(areaSize)
    , updateDelay(0)
    , timer(this)
    , ballsColliding(false) {
    connect(&timer, &QTimer::timeout, this, &BallAnimator::moveBalls);
}

void BallAnimator::randomPlaceBalls() {
    std::uniform_real_distribution<double> dist(-M_PI, M_PI);
    redBallParameters->setSpeed(100.0);
    redBallParameters->setDirectionAngle(dist(randomEngine));
    redBallParameters->setRadius(50.0);

    blueBallParameters->setSpeed(100.0);
    blueBallParameters->setDirectionAngle(dist(randomEngine));
    blueBallParameters->setRadius(50.0);

    setBallPositions();
}

void BallAnimator::start(std::uint32_t updateDelay) {
    this->updateDelay = updateDelay;
    timer.start(static_cast<int>(updateDelay));
}

void BallAnimator::setBallPositions() {
    setRandomBallPosition(redBallParameters);
    do {
        setRandomBallPosition(blueBallParameters);
    } while (circlesIntersecting());
}

void BallAnimator::setRandomBallPosition(BallParameters *ballParameters) {
    double width = areaSize.width();
    double height = areaSize.height();

    std::uniform_real_distribution<double> redXDist(ballParameters->getRadius(), width - ballParameters->getRadius());
    std::uniform_real_distribution<double> redYDist(ballParameters->getRadius(), height - ballParameters->getRadius());

    ballParameters->setPosition(QPointF(redXDist(randomEngine), redYDist(randomEngine)));
}

void BallAnimator::moveBalls() {
    updatePosition(redBallParameters);
    updatePosition(blueBallParameters);
    fixCollisions();
}

void BallAnimator::updatePosition(BallParameters *ballParameters) {
    double time = updateDelay / 1000.0;
    double distance = ballParameters->getSpeed() * time;
    double distanceX = std::cos(ballParameters->getDirectionAngle()) * distance;
    double distanceY = std::sin(ballParameters->getDirectionAngle()) * distance;
    QPointF nextPosition = ballParameters->getPosition() + QPointF(distanceX, distanceY);
    ballParameters->setPosition(nextPosition);
}

void BallAnimator::fixCollisions() {
    fixWallCollision(redBallParameters);
    fixWallCollision(blueBallParameters);
    fixBallCollision();
}


void BallAnimator::fixWallCollision(BallParameters *ballParameters) {
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

bool BallAnimator::isOutsideRight(BallParameters* ballParameters) {
    return ballParameters->getPosition().x() + ballParameters->getRadius() > areaSize.width();
}

bool BallAnimator::isOutsideTop(BallParameters* ballParameters) {
    return ballParameters->getPosition().y() + ballParameters->getRadius() > areaSize.height();
}

bool BallAnimator::isOutsideLeft(BallParameters* ballParameters) {
    return ballParameters->getPosition().x() - ballParameters->getRadius() < 0.0;
}

bool BallAnimator::isOutsideBottom(BallParameters* ballParameters) {
    return ballParameters->getPosition().y() - ballParameters->getRadius() < 0.0;
}

bool BallAnimator::isRightAngle(double angle) {
    return angle >= -M_PI_2 && angle <= M_PI_2;
}

bool BallAnimator::isLeftAngle(double angle) {
    return angle <= -M_PI_2 || angle >= M_PI_2;
}

bool BallAnimator::isTopAngle(double angle) {
    return angle >= 0.0;
}

bool BallAnimator::isBottomAngle(double angle) {
    return angle <= 0.0;
}


void BallAnimator::fixBallCollision() {
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

bool BallAnimator::circlesIntersecting() const {
    QPointF redPoint = redBallParameters->getPosition();
    QPointF bluePoint = blueBallParameters->getPosition();
    double redRadius = redBallParameters->getRadius();
    double blueRadius = blueBallParameters->getRadius();
    double distance = std::hypot(redPoint.x() - bluePoint.x(), redPoint.y() - bluePoint.y());
    return distance < redRadius + blueRadius;
}

double BallAnimator::calculateCollisionAngle() const {
    QPointF redPoint = redBallParameters->getPosition();
    QPointF bluePoint = blueBallParameters->getPosition();
    return std::atan2(redPoint.y() - bluePoint.y(), redPoint.x() - bluePoint.x());
}
