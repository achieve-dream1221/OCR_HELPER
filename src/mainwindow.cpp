// @Time: 2024/8/26 14:28
// @Author: ACHIEVE_DREAM
// @File: mainwindow.cpp
// @Software: CLion
// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QDropEvent>
#include <QGraphicsView>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->statusBar->showMessage("提示: 拖动图片,鼠标右键进行选区识别");
    ui->ocrEditor->setAcceptDrops(false);
    initThread();

    customGraphicsView = new CustomGraphicsView(this);
    const auto layout = new QVBoxLayout(ui->frame);
    layout->addWidget(customGraphicsView);
    layout->setContentsMargins({}); // 移除外边距
    ui->frame->setLayout(layout);

    connect(customGraphicsView, &CustomGraphicsView::startOcr, this, &MainWindow::do_ocrRecognize);
    connect(customGraphicsView, &CustomGraphicsView::canClicked, this, &MainWindow::enable_btn);
}

MainWindow::~MainWindow() {
    if (ocrThread->isRunning()) {
        ocrThread->quit();
        ocrThread->wait();
    }
    delete ui;
}


void MainWindow::enable_btn() const {
    ui->btnOcr->setEnabled(true);
}

void MainWindow::on_btnOcr_clicked() {
    const auto sceneRect = customGraphicsView->scene()->sceneRect();
    QPixmap pixmap(sceneRect.size().toSize());
    QPainter painter(&pixmap);
    // 将场景渲染到 QPixmap
    customGraphicsView->scene()->render(&painter, pixmap.rect(), sceneRect);
    // const auto pixmap = customGraphicsView->grab();
    do_ocrRecognize(pixmap);
}

void MainWindow::initThread() {
    // 动态创建线程和工作对象
    ocrThread = new QThread(this);
    ocrWorker = new OcrWorker;
    ocrWorker->moveToThread(ocrThread);

    connect(ocrWorker, &OcrWorker::ocrCompleted, this, [=](const QString &result) {
        ui->ocrEditor->setPlainText(result);
        qDebug() << "耗时: " << elapsedTimer.elapsed() << " ms";
        ui->statusBar->showMessage(QString("耗时: %1 ms").arg(elapsedTimer.elapsed()));
        // ocrThread->quit();
    });

    connect(ocrWorker, &OcrWorker::ocrFailed, this, [=](const QString &errorMessage) {
        ui->statusBar->showMessage(errorMessage);
        // ocrThread->quit();
    });

    // 清理线程和工作对象
    connect(ocrThread, &QThread::finished, ocrWorker, &QObject::deleteLater);
    connect(ocrThread, &QThread::finished, ocrThread, &QObject::deleteLater);
}

void MainWindow::do_ocrRecognize(const QPixmap &pixmap) {
    ui->ocrEditor->clear();
    elapsedTimer.restart();
    if (!ocrThread->isRunning()) {
        ocrThread->start();
    }
    QMetaObject::invokeMethod(ocrWorker, [this, pixmap] {
        qDebug() << pixmap.size();
        ocrWorker->doOcrRecognize(pixmap);
    }, Qt::QueuedConnection);
}
