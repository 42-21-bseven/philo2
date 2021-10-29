cmake_minimum_required(VERSION 3.19)
project(philo_bonus C)

set(CMAKE_C_STANDARD 11)

#include_directories(.)
#
#add_executable(philo2
#        philo.c
#        philo.h)
#add_executable(./src/*.c ./include s/philo_one.h)
file(GLOB SOURCE_FILES *.c *.h)
add_executable(philo_bonus ${SOURCE_FILES})