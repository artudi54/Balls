#pragma once
#include <QPaintEvent>
#include <QWidget>
#include <BallParameters.hpp>

class Ball : public QWidget {
    Q_OBJECT
public:
    explicit Ball(BallParameters* ballParameters, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void updateBallGeometry();
    void scheduleBallRedraw();

    BallParameters* ballParameters;
};
