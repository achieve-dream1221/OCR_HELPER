// @Time: 2024/8/28 10:39
// @Author: ACHIEVE_DREAM
// @File: CustomGraphicsView.h
// @Software: CLion
#pragma once
#include <QGraphicsView>
#include <QRubberBand>

class CustomGraphicsView final : public QGraphicsView {
    Q_OBJECT

public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

private:
    QRubberBand *rubberBand = nullptr;
    QPoint origin;
signals:
    void startOcr();
    void canClicked();
};
