// @Time: 2024/8/28 16:00
// @Author: ACHIEVE_DREAM
// @File: OcrWorker.cpp
// @Software: CLion
#include "OcrWorker.h"
#include "constants.h"
#include <QDebug>

OcrWorker::OcrWorker(OCR_HANDLE *ocrHandle, QObject *parent): QObject(parent), ocrHandle(ocrHandle) {
}
void OcrWorker::doOcrRecognize() {
    qDebug() << "Start OCR";
    OCR_PARAM param{};
    if (!OcrDetect(ocrHandle, SAVE_PATH, SAVE_IMG, &param)) {
        qDebug() << "OCR not Detected";
        emit ocrFailed("未识别到文本,请重新选择");
        return;
    }
    const int nLen = OcrGetLen(ocrHandle);
    if (nLen <= 0) {
        emit ocrFailed("未识别到文本,请重新选择");
        qDebug() << "OCR not Recognized";
        return;
    }
    const auto szInfo = std::make_unique<char[]>(nLen);
    if (!OcrGetResult(ocrHandle, szInfo.get(), nLen))return;
    qDebug() << szInfo.get();
    emit ocrCompleted(szInfo.get());
}
