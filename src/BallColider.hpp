#pragma once
#include <QWidget>
#include <BallMoveAnimator.hpp>
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
    void setRedBallParameters();
    void setBlueBallParameters();
    void setBaseBallParametrs(BallParameters* parameters);
    void placeBalls();

    Ui::BallColider* ui;
    BallParameters* redBallParameters;
    BallParameters* blueBallParameters;
    BallMoveAnimator* animator;
    Ball* redBall;
    Ball* blueBall;
};
