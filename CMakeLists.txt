cmake_minimum_required(VERSION 3.0)
project(lt_posix)


add_library(lt_posix SHARED
    ./src/lt_posix_dir.c 
    ./src/lt_posix_file.c 
    ./src/lt_posix_pipe.c 
    
    )

target_compile_options(lt_posix PUBLIC -O1)
target_link_libraries(lt_posix lt_api)
install(TARGETS lt_posix DESTINATION lib)

# test
enable_testing()
add_subdirectory(./tests)