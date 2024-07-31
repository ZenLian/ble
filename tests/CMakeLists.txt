add_executable(example_object_manager example_object_manager.cpp)
# add_test(NAME example_object_manager COMMAND example_object_manager)

add_executable(example_proxy example_proxy.cpp)
# add_test(NAME example_proxy COMMAND example_proxy)

add_executable(test_dbus_path_utils test_dbus_path_utils.cpp)
add_test(NAME test_dbus_path_utils COMMAND test_dbus_path_utils)

add_executable(test_bluez test_bluez.cpp)
target_link_libraries(test_bluez blecpp)