gtest = dependency('gtest',
                   main : true,
                   required : true)

gmock = dependency('gmock',
                   main : true,
                   required : true)

target = executable('unit-tests',
                    ['packet.cpp', 'helpers.cpp', 'frame.cpp'],
                    include_directories: gbee_include,
                    dependencies : [gtest, gmock])

test('gtest test',
     target)
