cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME bump_cars)
set(PLAYDATE_GAME_DEVICE bump_cars_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

if (TOOLCHAIN STREQUAL "armgcc")
	add_executable(${PLAYDATE_GAME_DEVICE} src/main.c
			src/game.h
			src/game.c
			src/objects/car.c
			src/objects/visual.h
			src/objects/car.h
			src/utils/graphicsUtils.c
			src/utils/graphicsUtils.h
			src/objects/arena.c
			src/objects/arena.h
			src/objects/objects.c
			src/objects/objects.h
			src/physics/physicsFunctions.c
			src/physics/physicsFunctions.h
			src/objects/carFunctions.h
			src/physics/vectors.h
	)
else()
	add_library(${PLAYDATE_GAME_NAME} SHARED src/main.c
			src/game.h
			src/game.c
			src/objects/car.c
			src/objects/visual.h
			src/objects/car.h
            src/utils/graphicsUtils.c
            src/utils/graphicsUtils.h
			src/objects/arena.c
			src/objects/arena.h
			src/objects/objects.c
			src/objects/objects.h
			src/physics/physicsFunctions.c
			src/physics/physicsFunctions.h
			src/objects/carFunctions.h
			src/physics/vectors.h
	)
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)

