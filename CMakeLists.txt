if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
endif(CMAKE_COMPILER_IS_GNUCC)

add_executable(videomedian videomedian.c)
