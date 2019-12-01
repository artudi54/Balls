#include "BallColider.hpp"
#include "ui_BallColider.h"
#include <ctime>
#include <random>


static std::mt19937 randomEngine(static_cast<std::mt19937::result_type>(std::time(nullptr)));

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
    setRedBallParameters();
    setBlueBallParameters();
    placeBalls();

    animator = new BallMoveAnimator(redBallParameters, blueBallParameters, ui->ballWidget->size(), 5, this);
    animator->start();
}

BallColider::~BallColider() {
    delete ui;
}

void BallColider::connectSignals() {
    // speed two way value binding
    connect(ui->redBallSpeedSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setSpeed);
    connect(redBallParameters, &BallParameters::speedChanged, this, [&](double value) { updateSliderValueWithoutNotyfying(ui->redBallSpeedSlider, value);});

    connect(ui->blueBallSpeedSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setSpeed);
    connect(blueBallParameters, &BallParameters::speedChanged, this, [&](double value) { updateSliderValueWithoutNotyfying(ui->blueBallSpeedSlider, value);});

    // radius two way value binding
    connect(ui->redBallRadiusSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setRadius);
    connect(redBallParameters, &BallParameters::radiusChanged, this, [&](double value) { updateSliderValueWithoutNotyfying(ui->redBallRadiusSlider, value);});

    connect(ui->blueBallRadiusSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setRadius);
    connect(blueBallParameters, &BallParameters::radiusChanged, this, [&](double value) { updateSliderValueWithoutNotyfying(ui->blueBallRadiusSlider, value);});
}

void BallColider::setRedBallParameters() {
    setBaseBallParametrs(redBallParameters);
    redBallParameters->setBorderColor(Qt::GlobalColor::darkRed);
    redBallParameters->setCircleColor(Qt::GlobalColor::red);
}

void BallColider::setBlueBallParameters() {
    setBaseBallParametrs(blueBallParameters);
    blueBallParameters->setBorderColor(Qt::GlobalColor::darkBlue);
    blueBallParameters->setCircleColor(Qt::GlobalColor::cyan);
}

void BallColider::setBaseBallParametrs(BallParameters *parameters) {
    std::uniform_real_distribution<double> dist(-M_PI, M_PI);
    parameters->setSpeed(100.0);
    parameters->setDirectionAngle(dist(randomEngine));
    parameters->setRadius(50.0);
}

void BallColider::placeBalls() {
    double width = ui->ballWidget->width();
    double height = ui->ballWidget->height();

    std::uniform_real_distribution<double> redXDist(redBallParameters->getRadius(), width -  redBallParameters->getRadius());
    std::uniform_real_distribution<double> redYDist(redBallParameters->getRadius(), height - redBallParameters->getRadius());
    redBallParameters->setPosition(QPointF(redXDist(randomEngine), redYDist(randomEngine)));

    std::uniform_real_distribution<double> blueXDist(blueBallParameters->getRadius(), width -  redBallParameters->getRadius());
    std::uniform_real_distribution<double> blueYDist(blueBallParameters->getRadius(), height - redBallParameters->getRadius());
    blueBallParameters->setPosition(QPointF(blueXDist(randomEngine), blueYDist(randomEngine)));
}

void BallColider::updateSliderValueWithoutNotyfying(QSlider *slider, double value) {
    slider->blockSignals(true);
    slider->setValue(static_cast<int>(value));
    slider->blockSignals(false);
}
