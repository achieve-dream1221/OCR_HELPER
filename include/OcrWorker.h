// @Time: 2024/8/28 16:00
// @Author: ACHIEVE_DREAM
// @File: OcrWorker.h
// @Software: CLion
#pragma once
#include <QObject>
#include <OcrLiteCApi.h>

class OcrWorker final : public QObject {
    Q_OBJECT

public:
    explicit OcrWorker(OCR_HANDLE *ocrHandle, QObject *parent = nullptr);

public slots:
    void doOcrRecognize();

private:
    OCR_HANDLE *ocrHandle;
signals:
    void ocrCompleted(const QString &result);

    void ocrFailed(const QString &errorMessage);
};
