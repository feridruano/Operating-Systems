project(ipc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall --pedantic -g -O0 -D_DEBUG")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

add_executable(pipe pipes/pipe.c)
add_executable(pipe1 pipes/pipe_1.c)
add_executable(pipe2 pipes/pipe_2.c)

add_executable(npipe_receiver ../npipe/npipe_receive.c)
add_executable(npipe_sender ../npipe/npipe_send.c)

add_executable(ipc_POSIX_sender ../ipc_POSIX/ipc_POSIX_send.c)
target_link_libraries(ipc_POSIX_sender rt)
add_executable(ipc_POSIX_receiver ../ipc_POSIX/ipc_POSIX_receive.c)
target_link_libraries(ipc_POSIX_receiver rt)

add_executable(shm_POSIX_time_server ../shm_POSIX/shm_POSIX_time_server.c)
target_link_libraries(shm_POSIX_time_server rt)
add_executable(shm_POSIX_time_client ../shm_POSIX/shm_POSIX_time_client.c)
target_link_libraries(shm_POSIX_time_client rt)

