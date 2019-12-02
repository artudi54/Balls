#include "BallWidget.hpp"
#include "ui_BallWidget.h"
#include <cmath>

namespace gui {
    BallWidget::BallWidget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::BallWidget)
        , redBallParameters(new model::BallParameters(this))
        , blueBallParameters(new model::BallParameters(this))
        , animator(nullptr)
        , redBall(nullptr)
        , blueBall(nullptr) {
        ui->setupUi(this);

        redBall = new Ball(redBallParameters, ui->ballWidget);
        blueBall = new Ball(blueBallParameters, ui->ballWidget);

        connectSignals();
        setBallColors();

        animator = new animations::BallAnimator(redBallParameters, blueBallParameters, ui->ballWidget->size(), this);
        animator->randomPlaceBalls();
        animator->start(10);
    }

    BallWidget::~BallWidget() {
        delete ui;
    }

    void BallWidget::connectSignals() {
        connectSpeedsTwoWay();
        connectRadiusesTwoWay();
    }

    void BallWidget::connectSpeedsTwoWay() {
        connect(ui->redBallSpeedSlider, &QSlider::valueChanged, redBallParameters, &model::BallParameters::setSpeedValue);
        connect(redBallParameters, &model::BallParameters::speedChanged, this,
                [&](auto &value) { updateSliderValueWithoutNotifying(ui->redBallSpeedSlider, value); });

        connect(ui->blueBallSpeedSlider, &QSlider::valueChanged, blueBallParameters, &model::BallParameters::setSpeedValue);
        connect(blueBallParameters, &model::BallParameters::speedChanged, this,
                [&](auto &value) { updateSliderValueWithoutNotifying(ui->blueBallSpeedSlider, value); });
    }

    void BallWidget::connectRadiusesTwoWay() {
        connect(ui->redBallRadiusSlider, &QSlider::valueChanged, redBallParameters, &model::BallParameters::setRadius);
        connect(redBallParameters, &model::BallParameters::radiusChanged, this,
                [&](double value) { updateSliderValueWithoutNotifying(ui->redBallRadiusSlider, value); });

        connect(ui->blueBallRadiusSlider, &QSlider::valueChanged, blueBallParameters, &model::BallParameters::setRadius);
        connect(blueBallParameters, &model::BallParameters::radiusChanged, this,
                [&](double value) { updateSliderValueWithoutNotifying(ui->blueBallRadiusSlider, value); });
    }

    void BallWidget::setBallColors() {
        redBallParameters->setBorderColor(Qt::darkRed);
        redBallParameters->setCircleColor(Qt::red);

        blueBallParameters->setBorderColor(Qt::darkBlue);
        blueBallParameters->setCircleColor(Qt::cyan);
    }

    void BallWidget::updateSliderValueWithoutNotifying(QSlider *slider, double value) {
        slider->blockSignals(true);
        slider->setValue(static_cast<int>(value));
        slider->blockSignals(false);
    }

    void BallWidget::updateSliderValueWithoutNotifying(QSlider *slider, const std::pair<double, double> &value) {
        updateSliderValueWithoutNotifying(slider, std::hypot(value.first, value.second));
    }
}
