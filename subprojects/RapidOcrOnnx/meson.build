project('RapidOcrOnnx', 'cpp', version : '1.2.3')

compiler = meson.get_compiler('cpp')  # 获取 C++ 编译器对象

RapidOcrOnnx_includes = include_directories('include')

dlls = files(
    'lib/RapidOcrOnnx.dll',
)
install_subdir('models', install_dir : meson.build_root())
install_data(dlls, install_dir : meson.build_root())

RapidOcrOnnx_dep = compiler.find_library('RapidOcrOnnx',
                                         dirs : [meson.current_source_dir() / 'lib'],
                                         header_include_directories : RapidOcrOnnx_includes
)