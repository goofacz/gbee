project(
    'gbee',
    'cpp',
    version : '0.1',
    license : 'GPL3',
    default_options : ['cpp_std=c++17']
)

gbee_headers = declare_dependency(
    include_directories: include_directories('include')
)

install_headers(
    'include/gbee/gbee.hpp',
    'include/gbee/packet.hpp',
    'include/gbee/frame.hpp',
    'include/gbee/helpers.hpp',
    subdir: 'gbee'
)
