# 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(ATM_linklist LANGUAGES C)

# 指定C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 自动搜索所有.c和.h的源文件和头文件
file(GLOB SOURCES "*.c" "*.h")

# 生成可执行文件
add_executable(atm_app ${SOURCES})

# (Optional)开启所有警告，帮助写出更加健壮的代码
if(MSVC)
    target_compile_options(atm_app PRIVATE /W4)
else()
    target_compile_options(atm_app PRIVATE -Wall -Wextra -Wpedantic)
endif()