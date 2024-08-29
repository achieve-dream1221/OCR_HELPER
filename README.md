<!-- TOC -->

* [基于PaddleOcrV4模型+RapidOcr+Qt6的离线OCR部署](#基于paddleocrv4模型rapidocrqt6的离线ocr部署)
* [如何使用](#如何使用)
    * [更改其他模型版本](#更改其他模型版本)
* [本项目使用的第三方库文件版本](#本项目使用的第三方库文件版本)
* [感谢](#感谢)

<!-- TOC -->

# 基于PaddleOcrV4模型+RapidOcr+Qt6的离线OCR部署

# 如何使用

```shell
git clone https://github.com/achieve-dream1221/OCR_HELPER 
cd OCR_HELPER
# 初始化项目
meson setup buildDir #meson setup buildDir -Docr_lib=RapidOcrOnnx 指定ocr_lib, 默认为OcrLiteOnnx, 
# 安装依赖的模型文件
meson install -C buildDir
# 首次运行,生成空的翻译文件才能正常编译exe
meson compile update_ts -C buildDir 
# 编译exe文件
meson compile your_exe_name -j 14 -C buildDir
# 读取编译的文件中需要翻译的字符串,重新生成翻译文件
meson compile update_ts -C buildDir 
```

## 更改其他模型版本

1. 下载模型文件并转换: [PaddleOcr预训练模型转ONNX](https://github.com/achieve-dream1221/paddleocr2onnx),可跳过,项目默认使用的V4模型
2. 将模型文件重命名,参考[constants.h](include/constants.h), 然后复制到编译目录下的models文件夹
3. 切换第三方OCR库:

```shell
# RapidOcrOnnx 
meson configure -Docr_lib=RapidOcrOnnx buildDir 
# or OcrLiteOnnx
meson configure -Docr_lib=OcrLiteOnnx buildDir
```

# 本项目使用的第三方库文件版本

> OcrLiteOnnx Release **1.8.1**

> RapidOcrOnnx PreRelease **1.2.3**

# 感谢

1. [PaddleOCR](https://github.com/PaddlePaddle/PaddleOCR)
2. [OcrLiteOnnx](https://github.com/benjaminwan/OcrLiteOnnx)
3. [RapidOcrOnnx](https://github.com/RapidAI/RapidOcrOnnx)