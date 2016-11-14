CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


# set temporaries names
SET(SRC_PATH src)
SET(EXECUTABLE client)

# store sources in SOURCES varable
SET (SOURCES ${SRC_PATH}/main.cpp
             ${SRC_PATH}/buffer.cpp
             ${SRC_PATH}/client.cpp
             ${SRC_PATH}/fake_tcp.cpp
             ${SRC_PATH}/fake_tcp_states.cpp
             ${SRC_PATH}/job.cpp
             ${SRC_PATH}/joined_threads.cpp
             ${SRC_PATH}/parser.cpp
             ${SRC_PATH}/thread_pool.cpp
)

# check if a compiller supports c++11 standart

# as I understand it exports all supported compillers's flags into CheckCXXCompilerFlag variable
include(CheckCXXCompilerFlag)
# check if that variable contains flag we are intrested in
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(NOT COMPILER_SUPPORTS_CXX11)
	set_property(TARGET ${OPTIONS} APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++11 ")
endif()

# store compillers flags we are going to use into OPTIONS varable
SET(OPTIONS -Wall -Werror -g2 -O0 -std=c++11)

# compillers flags we are going to use
add_definitions(${OPTIONS})

# compile
add_executable(${EXECUTABLE} ${SOURCES} )

# find pthread library on system we are building on
find_package(Threads REQUIRED)

# add linkage flags -lpthread
target_link_libraries(${EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT})

# specify directory the program will be installed to
INSTALL(PROGRAMS ${EXECUTABLE} 
        DESTINATION ${CMAKE_SOURCE_DIR}
)
