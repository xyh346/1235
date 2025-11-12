# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称（必须与要求一致）
project(cpp_exam)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件（关键！）
add_executable(cpp_exam main.cpp Song.cpp)

# 启用测试
enable_testing()

# 添加测试用例
set(TEST_CASES 1 2 3 4 5 6 7 8)
foreach(TEST_NUM ${TEST_CASES})
    add_test(
        NAME test_${TEST_NUM}
        COMMAND ${CMAKE_COMMAND} -E compare_files
        ${CMAKE_CURRENT_SOURCE_DIR}/testcases/expected_${TEST_NUM}.txt
        ${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NUM}.txt
    )

    # 在测试前运行程序生成输出
    add_test(
        NAME run_test_${TEST_NUM}
        COMMAND $<TARGET_FILE:cpp_exam> < ${CMAKE_CURRENT_SOURCE_DIR}/testcases/input_${TEST_NUM}.txt > ${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NUM}.txt
    )

    # 设置测试依赖
    set_tests_properties(test_${TEST_NUM} PROPERTIES DEPENDS run_test_${TEST_NUM})
endforeach()

# 设置测试属性
foreach(TEST_NUM ${TEST_CASES})
    set_tests_properties(run_test_${TEST_NUM} PROPERTIES TIMEOUT 10)
    set_tests_properties(test_${TEST_NUM} PROPERTIES TIMEOUT 5)
endforeach()