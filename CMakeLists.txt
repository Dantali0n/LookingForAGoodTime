cmake_minimum_required(VERSION 2.8.11)
#LookingForAGoodTime (LFGT)
project(lookingforagoodtime)

# Add custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(macros)

# Enable c++11 even when cmake < 3.1
use_cxx11()

# Enable pedantic errors and warnings for c++
use_cxx_warning_pedantic()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find qt widgets
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(sqlite3 REQUIRED)

set(LFGT_SOURCE
	highscoredto.cpp
	highscoremodel.cpp
	highscoreservice.cpp
	lookingforagoodtime.cpp
	main.cpp
)

set(LFGT_HEADERS
	highscoredto.h
	highscoremodel.h
	highscoreservice.h
	lookingforagoodtime.h
)

include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_wrap_ui(UI_HEADERS
	lookingforagoodtime.ui
)

qt5_wrap_cpp(MOC_SOURCE
	${LFGT_SOURCE}
	${LFGT_HEADERS}
)

qt5_add_resources(RESOURCES_RCC ${RESOURCE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(lookingforagoodtime ${LFGT_SOURCE} ${UI_HEADERS} ${MOC_SOURCE})

qt5_use_modules(lookingforagoodtime Widgets)

target_link_libraries(lookingforagoodtime Qt5::Widgets ${SQLITE3_LIBRARY})
