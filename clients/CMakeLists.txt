cmake_minimum_required(VERSION 3.22.1)
project(kursach)

set(CMAKE_CXX_STANDARD 20)

add_executable(client
        client.cpp
        ../backup_system/backup_system.cpp
#        ../validator/command_validator.cpp
        ../string_methods/mystring.cpp
)