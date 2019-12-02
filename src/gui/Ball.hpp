#pragma once
#include <QPaintEvent>
#include <QWidget>
#include <model/BallParameters.hpp>

namespace gui {
    class Ball : public QWidget {
    Q_OBJECT
    public:
        explicit Ball(model::BallParameters *ballParameters, QWidget *parent = nullptr);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        void updateBallGeometry();
        void scheduleBallRedraw();

        model::BallParameters *ballParameters;
    };
}
