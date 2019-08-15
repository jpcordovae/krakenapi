cmake_minimum_required (VERSION 2.6)
project (KRAKENAPI)

add_definitions("-std=c++17")
#add_definitions("-std=gnu++1z")
#-------------------------------------------------------------------------------
IF (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
	add_definitions("-Wno-tautological-compare")

ENDIF()

#-------------------------------------------------------------------------------
IF (APPLE)
	set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-flat_namespace -Wl,-undefined -Wl,suppress")
ENDIF (APPLE)

#-------------------------------------------------------------------------------
IF (POLICY CMP0043)
	cmake_policy(SET CMP0043 OLD)
ENDIF()

#-------------------------------------------------------------------------------
# Control CMAKE_BUILD_TYPE, default: Debug
#-------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE "Debug" CACHE string "Build type" FORCE)
endif (NOT CMAKE_BUILD_TYPE)
message (STATUS "Build type: " ${CMAKE_BUILD_TYPE})

#-------------------------------------------------------------------------------
# Find OpenSSL
#-------------------------------------------------------------------------------
find_package (OpenSSL REQUIRED)
include_directories (${OPENSSL_INCLUDE_DIR})
list(APPEND LIBS ${OPENSSL_LIBRARIES}) 

#-------------------------------------------------------------------------------
# Find CURL
#-------------------------------------------------------------------------------
find_package (CURL REQUIRED)
include_directories (${CURL_INCLUDE_DIR})
list(APPEND LIBS ${CURL_LIBRARIES})

#-------------------------------------------------------------------------------
# Find thread
#-------------------------------------------------------------------------------
find_package (Threads REQUIRED)

#-------------------------------------------------------------------------------
# Add library kapi (krakenapi)
#-------------------------------------------------------------------------------
aux_source_directory(kraken KAPI_FILES)
add_library (kapi STATIC ${KAPI_FILES})

# set some properties when CMAKE_BUILD_TYPE is "Debug"
set_target_properties (kapi PROPERTIES 
		      COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT"
		      DEBUG_POSTFIX "d")

list (INSERT LIBS 0 kapi)

#-------------------------------------------------------------------------------
# Add library libjson
#-------------------------------------------------------------------------------
set (LIBJSON_SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/libjson/_internal/Source)
aux_source_directory (${LIBJSON_SOURCE_DIR} LIBJSON_SOURCE_FILES)
add_library (libjson STATIC ${LIBJSON_SOURCE_FILES})

# remove "lib" prefix from the name of the libjson archive
set_target_properties (libjson PROPERTIES PREFIX "")

# set some properties when CMAKE_BUILD_TYPE is "Debug"
set_target_properties (libjson PROPERTIES 
		      COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT"
		      DEBUG_POSTFIX "d")

list (APPEND LIBS libjson)

#-------------------------------------------------------------------------------
# Add the executable 'kph'
#-------------------------------------------------------------------------------
add_executable (kph kph.cpp)
set_target_properties (kph PROPERTIES 
		      COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries (kph ${LIBS})

#-------------------------------------------------------------------------------
# Add the executable 'krt'
#-------------------------------------------------------------------------------
add_executable (krt krt.cpp  kraken/kutils.cpp)
set_target_properties (krt PROPERTIES 
		      COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries (krt ${LIBS})


#------------------------------------------------------------------------------
#Add the executable 'kpublic_tesst'
#------------------------------------------------------------------------------
add_executable (kpublic_test kpublic_test.cpp kraken/kohlc.cpp kraken/kutils.cpp)
set_target_properties(	kpublic_test PROPERTIES 
			COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries(kpublic_test ${LIBS})

#------------------------------------------------------------------------------
#Add the executable 'kstatistics'
#------------------------------------------------------------------------------
add_executable(kstatistics kstatistics.cpp kraken/kohlc.cpp kraken/kutils.cpp)
set_target_properties(	kstatistics PROPERTIES 
			COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries(kstatistics ${LIBS} ${CMAKE_THREAD_LIBS_INIT} boost_iostreams boost_system boost_filesystem)

#------------------------------------------------------------------------------
#Add the executable 'kprocessing_data'
#------------------------------------------------------------------------------
add_executable(kprocessing_data kprocessing_data.cpp kraken/kclient.cpp kraken/kohlc.cpp kraken/kutils.cpp)
set_target_properties(kprocessing_data PROPERTIES
  COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_IS_STRICT")
target_link_libraries(kprocessing_data ${LIBS} ${CMAKE_THREAD_LIBS_INIT} stdc++fs)
