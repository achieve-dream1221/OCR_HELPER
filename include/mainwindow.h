// @Time: 2024/8/26 14:28
// @Author: ACHIEVE_DREAM
// @File: mainwindow.h
// @Software: CLion
#pragma once
#include <QMainWindow>
#include "CustomGraphicsView.h"
#include "OcrWorker.h"
#include <QThread>
QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void initThread();

private slots:
    void on_btnOcr_clicked();

    void enable_btn() const;

    void do_ocrRecognize(const QPixmap &pixmap);

private:
    Ui::MainWindow *ui;
    CustomGraphicsView *customGraphicsView;
    QElapsedTimer elapsedTimer;
    QThread *ocrThread{};
    OcrWorker *ocrWorker{};
};
