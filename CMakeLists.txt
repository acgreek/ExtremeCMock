cmake_minimum_required (VERSION 2.8.4)
project (ExtremeCMock)

cmake_policy(SET CMP0003 NEW)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 9.2)

find_package(PkgConfig)
set(CMAKE_C_FLAGS "-Wall -Wextra -ggdb3 -fPIC" )
set(CMAKE_CXX_FLAGS "-std=c++0x -fPIC" )
add_library(ExtremeCMock SHARED mock.c)
target_link_libraries(ExtremeCMock dl)
find_package(PkgConfig)
pkg_check_modules (EXTREMECUNIT ExtremeCUnit)

string(REPLACE ";" " " EXTREMECUNIT_LDFLAGS2 "${EXTREMECUNIT_LDFLAGS}" )
string(REPLACE ";" " " EXTREMECUNIT_CFLAGS2 "${EXTREMECUNIT_CFLAGS}" )
message(STATUS "CFLAGS=" ${EXTREMECUNIT_CFLAGS2})

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-ggdb ${EXTREMECUNIT_CFLAGS2}")
	set(CMAKE_CXX_FLAGS "-ggdb ${EXTREMECUNIT_CFLAGS2}")
endif()


IF(CYGWIN)
	set(CMAKE_C_FLAGS "-Wl,--export-all-symbols ${CMAKE_C_FLAGS}" )
	set(CMAKE_CXX_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_FLAGS}" )
	set(CMAKE_CXX_LINK_FLAGS "-Wl,--export-all-symbols ${CMAKE_CXX_LINK_FLAG}" )

	set (PC_LIBRARY_FLAGS "-Wl,--export-all-symbols -Wl,--rpath,.,--rpath,dist,--rpath,${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib/libExtremeCMock.dll.a -ldl ")
	set (PC_C_FLAGS "-DUNIT_TEST -pie ")
	set(CMAKE_EXE_LINKER_FLAGS "-pie -ldl -lstdc++")
ELSE()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC " )
	set(CMAKE_EXE_LINKER_FLAGS "-pie -rdynamic -ldl -lstdc++")
	set (PC_LIBRARY_FLAGS "-pie -Wl,--rpath,.,--rpath,dist -lExtremeCMock -ldl" )
	set (PC_C_FLAGS "-DUNIT_TEST -fPIC -pie -rdynamic -lExtremeCMock" )
ENDIF(CYGWIN)


configure_file (
	"${PROJECT_SOURCE_DIR}/ExtremeCMock.pc.in"
   "${PROJECT_BINARY_DIR}/ExtremeCMock.pc"
)

set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_C_COMPILER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

add_executable(test test.c testpp.cpp ) 

target_link_libraries(test ExtremeCMock dl stdc++)
set_target_properties(test PROPERTIES CMAKE_C_LINK_FLAGS "${CMAKE_C_FLAGS} -DUNIT_TEST")
set_target_properties(test PROPERTIES CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_FLAGS}  -DUNIT_TEST")

add_custom_target(run ./test)
add_custom_target(runv ./test -v)
ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)

install(TARGETS ExtremeCMock DESTINATION "lib")
install(FILES ExtremeCMock.h DESTINATION "include")
install(FILES ExtremeCMock.pc DESTINATION "lib/pkgconfig")

