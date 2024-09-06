project(ble CXX)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS -g)
include_directories(
    include
    /usr/include/libmount
    /usr/include/blkid
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include)
link_libraries(-lgio-2.0 -lgobject-2.0 -lglib-2.0)

# add_subdirectory(src)

enable_testing()
add_subdirectory(tests)