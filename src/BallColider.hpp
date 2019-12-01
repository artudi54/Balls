#pragma once
#include <QWidget>
#include <QSlider>
#include <BallAnimator.hpp>
#include <BallParameters.hpp>
#include <Ball.hpp>

namespace Ui {
    class BallColider;
}

class BallColider : public QWidget {
    Q_OBJECT
public:
    explicit BallColider(QWidget *parent = nullptr);
    ~BallColider() override;

private:
    void connectSignals();
    void setRedBallColors();
    void setBlueBallColors();
    void updateSliderValueWithoutNotifying(QSlider* slider, double value);

    Ui::BallColider* ui;
    BallParameters* redBallParameters;
    BallParameters* blueBallParameters;
    BallAnimator* animator;
    Ball* redBall;
    Ball* blueBall;
};
