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
    , redBall(nullptr)
    , blueBall(nullptr) {
    ui->setupUi(this);

    redBall = new Ball(redBallParameters, ui->ballWidget);
    blueBall = new Ball(blueBallParameters, ui->ballWidget);

    connectSignals();
    setRedBallParameters();
    setBlueBallParameters();
    placeBalls();
}

BallColider::~BallColider() {
    delete ui;
}

void BallColider::connectSignals() {
    // speed two way value binding
    connect(ui->blueBallSpeedSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setSpeed);
    connect(blueBallParameters, &BallParameters::speedChanged, ui->blueBallRadiusSlider, &QSlider::setValue);

    connect(ui->redBallSpeedSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setSpeed);
    connect(redBallParameters, &BallParameters::speedChanged, ui->redBallRadiusSlider, &QSlider::setValue);

    // radius two way value binding
    connect(ui->blueBallRadiusSlider, &QSlider::valueChanged, blueBallParameters, &BallParameters::setRadius);
    connect(blueBallParameters, &BallParameters::radiusChanged, ui->blueBallRadiusSlider, &QSlider::setValue);

    connect(ui->redBallRadiusSlider, &QSlider::valueChanged, redBallParameters, &BallParameters::setRadius);
    connect(redBallParameters, &BallParameters::radiusChanged, ui->redBallRadiusSlider, &QSlider::setValue);
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
    std::uniform_real_distribution<double> dist(0.0, 2 * M_PI);
    parameters->setSpeed(100.0);
    parameters->setDirectionAngle(dist(randomEngine));
    parameters->setRadius(100.0);
}

void BallColider::placeBalls() {

}
