project(
    'gbee-tests',
    'cpp'
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
    dependencies : gtest
)

test(
    'gtest test',
    target
)
