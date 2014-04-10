cmake_minimum_required(VERSION 2.8)

SET ( EXECUTABLE geneticAlgo )

#This is for heterogenous groups
#add_definitions(-D_HETERO_)

# To add c++11 support
if (MSVC) # MSVC compiler (Win32 only)
	# Display more warnings
	SET (CMAKE_CXX_FLAGS "/W3")
elseif (UNIX OR CMAKE_COMPILER_IS_GNUCXX) # Clang OR Gcc (Linux, Mac OS or Win32 with MingW)
	# Enable C++11 and displays all warnings
	SET (CMAKE_CXX_FLAGS "-std=c++11")
	if (APPLE) # Clang / Mac OS only
		# Required on OSX to compile c++11
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -mmacosx-version-min=10.7")
	endif(APPLE)
endif()

FILE ( GLOB SOURCES ./src/*.cpp ./example/*.cpp )  
FILE ( GLOB HEADERS ./include/*.h ./example/*.h ) 
FILE ( GLOB UIS ./*.ui ) 


INCLUDE_DIRECTORIES( /usr/include/ /usr/local/include/ ${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE( ${EXECUTABLE} ${SOURCES})
TARGET_LINK_LIBRARIES( ${EXECUTABLE} "-L/sw/lib -lm" )
