// @Time: 2024/8/26 14:28
// @Author: ACHIEVE_DREAM
// @File: mainwindow.h
// @Software: CLion
#pragma once
#include <OcrWorker.h>
#include <QMainWindow>
#include "CustomGraphicsView.h"
#include <QElapsedTimer>

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

private slots:
    void on_btnOcr_clicked();
    void enable_btn() const;
    void do_ocrRecognize();

private:
    Ui::MainWindow *ui;
    CustomGraphicsView *customGraphicsView;
    QElapsedTimer elapsedTimer;
    OCR_HANDLE *ocrHandle;
};
