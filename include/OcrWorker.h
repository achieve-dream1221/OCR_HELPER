// @Time: 2024/8/28 16:00
// @Author: ACHIEVE_DREAM
// @File: OcrWorker.h
// @Software: CLion
#pragma once
#include <QObject>
#include <OcrLiteCApi.h>
#include <QPixmap>

class OcrWorker final : public QObject {
    Q_OBJECT

public:
    explicit OcrWorker();

    ~OcrWorker() override;

public slots:
    void doOcrRecognize(const QPixmap &pixmap);

private:
    OCR_HANDLE *ocrHandle;
signals:
    void ocrCompleted(const QString &result);

    void ocrFailed(const QString &errorMessage);
};
