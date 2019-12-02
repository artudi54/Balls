#pragma once
#include <QWidget>
#include <QSlider>
#include <BallAnimator.hpp>
#include <BallParameters.hpp>
#include <Ball.hpp>

namespace Ui {
    class BallCollider;
}

class BallCollider : public QWidget {
    Q_OBJECT
public:
    explicit BallCollider(QWidget *parent = nullptr);
    ~BallCollider() override;

private:
    void connectSignals();
    void connectSpeedsTwoWay();
    void connectRadiusesTwoWay();

    void setBallColors();

    void updateSliderValueWithoutNotifying(QSlider* slider, const std::pair<double, double>& value);
    void updateSliderValueWithoutNotifying(QSlider* slider, double value);

    Ui::BallCollider* ui;
    BallParameters* redBallParameters;
    BallParameters* blueBallParameters;
    BallAnimator* animator;
    Ball* redBall;
    Ball* blueBall;
};
