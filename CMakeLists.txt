cmake_minimum_required(VERSION 3.3)
project(vkidcollector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

#include_directories("/home/ssi/mongo-client-install/include")
#link_directories("/home/ssi/mongo-client-install/lib")
include_directories("/home/ssi/mongo-myclient-install/include")
link_directories("/home/ssi/mongo-myclient-install/lib")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)

set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS thread system regex filesystem)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

set(SOURCE_FILES
#        maintest.cpp
        main.cpp
        renderexception.cpp
        Stack.h
#        request/Request.cpp
#        request/sp.cpp
#        collect_func.cpp
#        pttest.cpp
        servsock.cpp
        dir.cpp
)
add_executable(vkidcollector ${SOURCE_FILES})
target_link_libraries(vkidcollector
        mongoclient
        pthread
        sasl2
        crypto
        ssl
        ${XMLPP_LIBRARIES}
        ${Boost_LIBRARIES}
        curl
        fcgi
)
