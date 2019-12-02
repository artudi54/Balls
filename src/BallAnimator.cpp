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
    , timer(this)
    , ballCollisionHandled(false) {
    connect(&timer, &QTimer::timeout, this, &BallAnimator::moveBalls);
}

void BallAnimator::randomPlaceBalls() {
    std::uniform_real_distribution<double> dist(-M_PI, M_PI);
    redBallParameters->setSpeedValue(100.0);
    redBallParameters->setDirectionAngle(dist(randomEngine));
    redBallParameters->setRadius(50.0);

    blueBallParameters->setSpeedValue(100.0);
    blueBallParameters->setDirectionAngle(dist(randomEngine));
    blueBallParameters->setRadius(50.0);

    setBallPositions();
}

void BallAnimator::start(std::uint32_t updateDelay) {
    timer.start(static_cast<int>(updateDelay));
}

void BallAnimator::setBallPositions() {
    setRandomBallPosition(redBallParameters);
    do {
        setRandomBallPosition(blueBallParameters);
    } while (circlesIntersecting());
}

void BallAnimator::setRandomBallPosition(BallParameters *ballParameters) {
    double radius = ballParameters->getRadius();
    double width = areaSize.width();
    double height = areaSize.height();
    std::uniform_real_distribution<double> xDist(radius, width - radius);
    std::uniform_real_distribution<double> yDist(radius, height - radius);
    ballParameters->setPosition(QPointF(xDist(randomEngine), yDist(randomEngine)));
}

void BallAnimator::moveBalls() {
    updatePosition(redBallParameters);
    updatePosition(blueBallParameters);
    fixCollisions();
}

void BallAnimator::updatePosition(BallParameters *ballParameters) {
    double time = timer.interval() / 1000.0;
    double distanceX = ballParameters->getSpeed().first * time;
    double distanceY = ballParameters->getSpeed().second * time;
    QPointF nextPosition = ballParameters->getPosition() + QPointF(distanceX, distanceY);
    ballParameters->setPosition(nextPosition);
}

void BallAnimator::fixCollisions() {
    handleWallCollision(redBallParameters);
    handleWallCollision(blueBallParameters);
    handleBallCollision();
}


void BallAnimator::handleWallCollision(BallParameters *ballParameters) {
    auto& speed = ballParameters->getSpeed();

    bool inverseX = (isOutsideRight(ballParameters) && isMovingRight(speed)) ||
                    (isOutsideLeft(ballParameters) && isMovingLeft(speed));
    if (inverseX) {
        ballParameters->setSpeed({-speed.first, speed.second});
        return;
    }

    bool inverseY = (isOutsideTop(ballParameters) && isMovingTop(speed)) ||
                    (isOutsideBottom(ballParameters) && isMovingBottom(speed));
    if (inverseY) {
        ballParameters->setSpeed({speed.first, -speed.second});
        return;
    }
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

bool BallAnimator::isMovingRight(const std::pair<double, double>& speed) {
    return speed.first >= 0;
}

bool BallAnimator::isMovingLeft(const std::pair<double, double>& speed) {
    return speed.first <= 0;
}

bool BallAnimator::isMovingTop(const std::pair<double, double>& speed) {
    return speed.second >= 0;
}

bool BallAnimator::isMovingBottom(const std::pair<double, double>& speed) {
    return speed.second <= 0;
}


void BallAnimator::handleBallCollision() {
    if (circlesIntersecting()) {
        if (!ballCollisionHandled) {
            adjustBallSpeeds();
            ballCollisionHandled = true;
        }
    }
    else {
        ballCollisionHandled = false;
    }
}

void BallAnimator::adjustBallSpeeds() {
    double redSpeedValue = redBallParameters->getSpeedValue();
    double redAngle = redBallParameters->getDirectionAngle();
    double blueSpeedValue = blueBallParameters->getSpeedValue();
    double blueAngle = blueBallParameters->getDirectionAngle();
    double collisionAngle = calculateCollisionAngle();

    double redSpeedU = redSpeedValue * std::cos(redAngle - collisionAngle);
    double redSpeedW = redSpeedValue * std::sin(redAngle - collisionAngle);
    double blueSpeedU = blueSpeedValue * std::cos(blueAngle - collisionAngle);
    double blueSpeedW = blueSpeedValue * std::sin(blueAngle - collisionAngle);
    std::swap(redSpeedU, blueSpeedU);

    double redSpeedValueP = std::hypot(redSpeedU, redSpeedW);
    double redAngleP = std::atan2(redSpeedW, redSpeedU);
    double blueSpeedValueP = std::hypot(blueSpeedU, blueSpeedW);
    double blueAngleP = std::atan2(blueSpeedW, blueSpeedU);

    double redSpeedX = redSpeedValueP * std::cos(redAngleP + collisionAngle);
    double redSpeedY = redSpeedValueP * std::sin(redAngleP + collisionAngle);
    double blueSpeedX = blueSpeedValueP * std::cos(blueAngleP + collisionAngle);
    double blueSpeedY = blueSpeedValueP * std::sin(blueAngleP + collisionAngle);

    redBallParameters->setSpeed({redSpeedX, redSpeedY});
    blueBallParameters->setSpeed({blueSpeedX, blueSpeedY});
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
