#pragma once
#include <QObject>
#include <QSizeF>
#include <QTimer>
#include <model/BallParameters.hpp>

namespace animations {
    class BallAnimator : public QObject {
        Q_OBJECT
    public:
        BallAnimator(model::BallParameters* redBallParameters, model::BallParameters* blueBallParameters,
                     QSizeF areaSize, QObject* parent = nullptr);
        void randomPlaceBalls();
        void start(std::uint32_t updateDelay);

    private:
        void setBallPositions();
        void setRandomBallPosition(model::BallParameters* ballParameters);

        void moveBalls();
        void updatePosition(model::BallParameters* ballParameters);

        void fixCollisions();

        void handleWallCollision(model::BallParameters* ballParameters);
        bool isOutsideRight(model::BallParameters* ballParameters);
        bool isOutsideTop(model::BallParameters* ballParameters);
        bool isOutsideLeft(model::BallParameters* ballParameters);
        bool isOutsideBottom(model::BallParameters* ballParameters);
        static bool isMovingRight(const std::pair<double, double>& speed);
        static bool isMovingLeft(const std::pair<double, double>& speed);
        static bool isMovingTop(const std::pair<double, double>& speed);
        static bool isMovingBottom(const std::pair<double, double>& speed);

        void handleBallCollision();
        void adjustBallSpeeds();
        bool circlesIntersecting() const;
        double calculateCollisionAngle() const;

        model::BallParameters* redBallParameters;
        model::BallParameters* blueBallParameters;
        QSizeF areaSize;
        QTimer timer;
        bool ballCollisionHandled;
    };
}
