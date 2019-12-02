#include "BallCollider.hpp"
#include "ui_BallCollider.h"
#include <cmath>

BallCollider::BallCollider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BallCollider)
    , redBallParameters(new BallParameters(this))
    , blueBallParameters(new BallParameters(this))
    , animator(nullptr)
    , redBall(nullptr)
    , blueBall(nullptr) {
    ui->setupUi(this);

    redBall = new Ball(redBallParameters, ui->ballWidget);
    blueBall = new Ball(blueBallParameters, ui->ballWidget);

    connectSignals();
    setBallColors();

    animator = new BallAnimator(redBallParameters, blueBallParameters, ui->ballWidget->size(), this);
    animator->randomPlaceBalls();
    animator->start(10);
}

BallCollider::~BallCollider() {
    delete ui;
}

void BallCollider::connectSignals() {
    connectSpeedsTwoWay();
    connectRadiusesTwoWay();
}

void BallCollider::connectSpeedsTwoWay() {
    connect(ui->redBallSpeedSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setSpeedValue);
    connect(redBallParameters, &BallParameters::speedChanged, this,
            [&](auto& value) { updateSliderValueWithoutNotifying(ui->redBallSpeedSlider, value);});

    connect(ui->blueBallSpeedSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setSpeedValue);
    connect(blueBallParameters, &BallParameters::speedChanged, this,
            [&](auto& value) { updateSliderValueWithoutNotifying(ui->blueBallSpeedSlider, value);});
}

void BallCollider::connectRadiusesTwoWay() {
    connect(ui->redBallRadiusSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setRadius);
    connect(redBallParameters, &BallParameters::radiusChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->redBallRadiusSlider, value);});

    connect(ui->blueBallRadiusSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setRadius);
    connect(blueBallParameters, &BallParameters::radiusChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->blueBallRadiusSlider, value);});
}

void BallCollider::setBallColors() {
    redBallParameters->setBorderColor(Qt::darkRed);
    redBallParameters->setCircleColor(Qt::red);

    blueBallParameters->setBorderColor(Qt::darkBlue);
    blueBallParameters->setCircleColor(Qt::cyan);
}

void BallCollider::updateSliderValueWithoutNotifying(QSlider *slider, double value) {
    slider->blockSignals(true);
    slider->setValue(static_cast<int>(value));
    slider->blockSignals(false);
}

void BallCollider::updateSliderValueWithoutNotifying(QSlider *slider, const std::pair<double, double> &value) {
    updateSliderValueWithoutNotifying(slider, std::hypot(value.first, value.second));
}
