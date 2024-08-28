// @Time: 2024/8/28 10:39
// @Author: ACHIEVE_DREAM
// @File: CustomGraphicsView.cpp
// @Software: CLion
#include "CustomGraphicsView.h"
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QMimeData>

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    setAcceptDrops(true);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(ScrollHandDrag); // 允许鼠标上下左右拖动图片
    const auto scene = new QGraphicsScene(this);
    setScene(scene);
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event) {
    constexpr double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        const auto origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }
    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (rubberBand->isVisible()) {
        rubberBand->setGeometry((QRect(rubberBand->pos(), event->pos()).normalized()));
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        const auto selectionRect = mapToScene(rubberBand->geometry()).boundingRect();
        rubberBand->hide();
        // 假设场景中只有一个图像图元
        if (auto items = scene()->items(selectionRect); !items.isEmpty()) {
            if (const auto pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(items.first())) {
                // 获取图元的原始图像
                const QPixmap originalPixmap = pixmapItem->pixmap();
                // 将选中的区域转换为图元坐标系
                const QRectF itemRect = pixmapItem->mapFromScene(selectionRect).boundingRect();
                // 截取选中的区域
                const QImage selectedImage = originalPixmap.toImage().copy(itemRect.toRect());
                qDebug() << selectedImage.save("result.png");
                emit startOcr();
            }
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::dragEnterEvent(QDragEnterEvent *event) {
    // 检查是否包含 URL，并且这些 URL 是图片文件
    if (const auto mimeData = event->mimeData(); mimeData->hasUrls()) {
        if (const auto &urls = mimeData->urls(); urls.size() == 1) {
            // 只处理单个文件
            if (const QImage image(urls.first().toLocalFile()); !image.isNull()) {
                // 检查文件是否是有效的图片
                event->acceptProposedAction(); // 接受拖拽操作
                return;
            }
        }
    }
    event->ignore(); // 非图片文件时忽略事件
}

void CustomGraphicsView::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void CustomGraphicsView::dropEvent(QDropEvent *event) {
    const QImage image(event->mimeData()->urls().first().toLocalFile());
    const auto pixmap = QPixmap::fromImage(image);
    scene()->clear();
    scene()->addPixmap(pixmap);
    emit canClicked();
}
