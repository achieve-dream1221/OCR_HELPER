// @Time: 2024/8/26 14:28
// @Author: ACHIEVE_DREAM
// @File: mainwindow.cpp
// @Software: CLion
// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"

#include <constants.h>

#include "ui_MainWindow.h"
#include <QDropEvent>
#include <QGraphicsView>
#include "OcrWorker.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->statusBar->showMessage("提示: 拖动图片,鼠标右键进行选区识别");
    ui->ocrEditor->setAcceptDrops(false);

    customGraphicsView = new CustomGraphicsView(this);
    const auto layout = new QVBoxLayout(ui->frame);
    layout->addWidget(customGraphicsView);
    layout->setContentsMargins({}); // 移除外边距
    ui->frame->setLayout(layout);

    ocrHandle = static_cast<OCR_HANDLE *>(OcrInit(DET_MODEL, CLS_MODEL, REC_MODEL, KEY_FILE, THREAD_NUM));
    connect(customGraphicsView, &CustomGraphicsView::startOcr, this, &MainWindow::do_ocrRecognize);
    connect(customGraphicsView, &CustomGraphicsView::canClicked, this, &MainWindow::enable_btn);
}

MainWindow::~MainWindow() {
    OcrDestroy(ocrHandle);
    delete ui;
}

void MainWindow::on_btnOcr_clicked() {
    const auto sceneRect = customGraphicsView->scene()->sceneRect();
    QPixmap pixmap(sceneRect.size().toSize());
    QPainter painter(&pixmap);
    // 将场景渲染到 QPixmap
    customGraphicsView->scene()->render(&painter, pixmap.rect(), sceneRect);
    // const auto pixmap = customGraphicsView->grab();
    qDebug() << "Save Result: " << pixmap.save(QString("%1%2").arg(SAVE_PATH, SAVE_IMG));
    do_ocrRecognize();
}


void MainWindow::enable_btn() const {
    ui->btnOcr->setEnabled(true);
}

void MainWindow::do_ocrRecognize() {
    qDebug() << "Start OCR";
    ui->ocrEditor->clear();
    elapsedTimer.restart();

    // 动态创建线程和工作对象
    auto *ocrThread = new QThread(this);
    auto *ocrWorker = new OcrWorker(ocrHandle, nullptr);
    ocrWorker->moveToThread(ocrThread);

    // 连接信号和槽
    connect(ocrThread, &QThread::started, ocrWorker, &OcrWorker::doOcrRecognize, Qt::QueuedConnection);
    connect(ocrWorker, &OcrWorker::ocrCompleted, this, [=](const QString &result) {
        ui->ocrEditor->setPlainText(result);
        qDebug() << "耗时: " << elapsedTimer.elapsed() << " ms";
        ui->statusBar->showMessage(QString("耗时: %1 ms").arg(elapsedTimer.elapsed()));
        ocrThread->quit();
    }, Qt::QueuedConnection);

    connect(ocrWorker, &OcrWorker::ocrFailed, this, [=](const QString &errorMessage) {
        ui->statusBar->showMessage(errorMessage);
        ocrThread->quit();
    }, Qt::QueuedConnection);

    // 清理线程和工作对象
    connect(ocrThread, &QThread::finished, ocrWorker, &QObject::deleteLater);
    connect(ocrThread, &QThread::finished, ocrThread, &QObject::deleteLater);

    // 启动线程
    ocrThread->start();
}
