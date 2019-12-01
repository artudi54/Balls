#include "BallColider.hpp"
#include "ui_BallColider.h"

BallColider::BallColider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BallColider)
    , redBallParameters(new BallParameters(this))
    , blueBallParameters(new BallParameters(this))
    , animator(nullptr)
    , redBall(nullptr)
    , blueBall(nullptr) {
    ui->setupUi(this);

    redBall = new Ball(redBallParameters, ui->ballWidget);
    blueBall = new Ball(blueBallParameters, ui->ballWidget);

    connectSignals();
    setRedBallColors();
    setBlueBallColors();

    animator = new BallAnimator(redBallParameters, blueBallParameters, ui->ballWidget->size(), this);
    animator->randomPlaceBalls();
    animator->start(10);
}

BallColider::~BallColider() {
    delete ui;
}

void BallColider::connectSignals() {
    // speed two way value binding
    connect(ui->redBallSpeedSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setSpeed);
    connect(redBallParameters, &BallParameters::speedChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->redBallSpeedSlider, value);});

    connect(ui->blueBallSpeedSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setSpeed);
    connect(blueBallParameters, &BallParameters::speedChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->blueBallSpeedSlider, value);});

    // radius two way value binding
    connect(ui->redBallRadiusSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setRadius);
    connect(redBallParameters, &BallParameters::radiusChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->redBallRadiusSlider, value);});

    connect(ui->blueBallRadiusSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setRadius);
    connect(blueBallParameters, &BallParameters::radiusChanged, this,
            [&](double value) { updateSliderValueWithoutNotifying(ui->blueBallRadiusSlider, value);});
}

void BallColider::setRedBallColors() {
    redBallParameters->setBorderColor(Qt::GlobalColor::darkRed);
    redBallParameters->setCircleColor(Qt::GlobalColor::red);
}

void BallColider::setBlueBallColors() {
    blueBallParameters->setBorderColor(Qt::GlobalColor::darkBlue);
    blueBallParameters->setCircleColor(Qt::GlobalColor::cyan);
}

void BallColider::updateSliderValueWithoutNotifying(QSlider *slider, double value) {
    slider->blockSignals(true);
    slider->setValue(static_cast<int>(value));
    slider->blockSignals(false);
}
