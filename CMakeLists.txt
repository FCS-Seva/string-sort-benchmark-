cmake_minimum_required(VERSION 3.20)
project(SET9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# release-сборка по умолчанию для адекватных замеров времени
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# главный исполняемый файл - проводит замеры и пишет CSV
add_executable(SET9
        main.cpp
        src/StringGenerator.cpp
        src/StringSortTester.cpp
        src/sorts.cpp
)
target_include_directories(SET9 PRIVATE src)

add_executable(A1q_string_quicksort codeforces/A1q_string_quicksort.cpp)
add_executable(A1m_string_mergesort codeforces/A1m_string_mergesort.cpp)
add_executable(A1r_msd_radix codeforces/A1r_msd_radix.cpp)
add_executable(A1rq_msd_radix_quick codeforces/A1rq_msd_radix_quick.cpp)
