project('OcrLiteOnnx', 'cpp', version : '1.8.1')

compiler = meson.get_compiler('cpp')  # 获取 C++ 编译器对象

OcrLiteOnnx_includes = include_directories('include')

dlls = files(
    'lib/OcrLiteOnnx.dll',
)

install_subdir('models', install_dir : meson.build_root())

install_data(dlls, install_dir : meson.build_root())

OcrLiteOnnx_dep = compiler.find_library('OcrLiteOnnx',
                                        dirs : [meson.current_source_dir() / 'lib'],
                                        header_include_directories : OcrLiteOnnx_includes
)