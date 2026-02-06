project('SoftOmniSwift', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++20'])

include_dirs = include_directories('Include')
subdir('Sources')

lib_args = ['-DBUILDING_MESON_LIBRARY']

lib = shared_library('SoftOmniSwift', sources,
                     install : true,
                     include_directories: include_dirs,
                     cpp_args : ['-Wall', '-Wextra', '-pedantic', '-Werror', '-Wvla', '-O2', '-Wold-style-cast']
)

debug_lib = shared_library('SoftOmniSwift_Debug', sources,
                           install : true,
                           include_directories: include_dirs,
                           cpp_args : ['-Wall', '-Wextra', '-pedantic', '-Werror', '-Wvla', '-g3', '-O0', '-Wold-style-cast']
)

gtest_dependency = dependency('gtest_main',
                              required: true,
                              fallback: ['gtest', 'gtest_main_dep']
)

subdir('Tests')
