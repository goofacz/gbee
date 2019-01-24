project(
    'gbee-tests',
    'cpp',
    default_options : ['cpp_std=c++17']
)

gtest = dependency(
    'gtest',
    main : true,
    required : true
)

gmock = dependency(
    'gmock',
    main : true,
    required : true
)

target = executable(
    'unit-tests',
    ['packet.cpp', 'helpers.cpp'],
    include_directories: include_directories('../include'),
    dependencies : [gtest, gmock]
)

test(
    'gtest test',
    target
)
