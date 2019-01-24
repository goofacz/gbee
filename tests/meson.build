project(
    'gbee-tests',
    'cpp',
    default_options : ['cpp_std=c++17']
)

gtest = dependency(
    'gtest',
    main : true,
    required : false
)

if not gtest.found()
  error('Google tests not found!')
endif

target = executable(
    'unit-tests',
    'packet.cpp',
    'helpers.cpp',
    include_directories: include_directories('../include'),
    dependencies : gtest
)

test(
    'gtest test',
    target
)
