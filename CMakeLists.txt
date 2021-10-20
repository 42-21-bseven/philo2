cmake_minimum_required(VERSION 3.20)
project(philo2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(philo2
        philo.c
        philo.h)
