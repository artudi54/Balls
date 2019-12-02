#pragma once
#include <QWidget>
#include <QSlider>
#include <animations/BallAnimator.hpp>
#include <gui/Ball.hpp>
#include <model/BallParameters.hpp>

namespace Ui {
    class BallWidget;
}

namespace gui {
    class BallWidget : public QWidget {
        Q_OBJECT
    public:
        explicit BallWidget(QWidget *parent = nullptr);
        ~BallWidget() override;

    private:
        void connectSignals();
        void connectSpeedsTwoWay();
        void connectRadiusesTwoWay();

        void setBallColors();

        void updateSliderValueWithoutNotifying(QSlider *slider, const std::pair<double, double> &value);
        void updateSliderValueWithoutNotifying(QSlider *slider, double value);

        Ui::BallWidget *ui;
        model::BallParameters *redBallParameters;
        model::BallParameters *blueBallParameters;
        animations::BallAnimator *animator;
        Ball *redBall;
        Ball *blueBall;
    };
}
