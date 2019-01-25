project('c++ gbee', 
        'cpp', 
        version : '0.1', 
        license : 'GPL3', 
        default_options : ['cpp_std=c++17'])

gbee_include = include_directories('include')

subdir('include/gbee')
subdir('tests')

declare_dependency(include_directories: gbee_include)

pkg_module = import('pkgconfig')
pkg_module.generate(version : '0.1',
                    name : 'libgbee',
                    filebase : 'gbee',
                    description: '')
