cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 99)
project(rtv1)
set(CMAKE_MODULE_PATH	${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/find_sdl2/")
#set(CMAKE_C_FLAGS		"${CMAKE_C_FLAGS} -O3")
#set(CMAKE_C_FLAGS		"${CMAKE_C_FLAGS} -fsanitize=\"address\" -g3")
set(CMAKE_C_FLAGS		"${CMAKE_C_FLAGS} -Ipthreads -Weverything -Wno-cast-qual")
#set(CMAKE_C_FLAGS		"${CMAKE_C_FLAGS} ")
set(LIB_PATH			libraries)
set(SRC_PATH			sources)
set(INC_PATH			includes)
set(LFT_PATH			${LIB_PATH}/libft)
set(LFT_NAME			ft)
set(LFT_INC_PATH		${LFT_PATH}/)
set(LFT_LIB				${LFT_PATH}
						)
include_directories(	${INC_PATH}
						${LFT_INC_PATH}
						${SDL2_INCLUDE_DIR}
						${SDL2_TTF_INCLUDE_DIR}
						${SDL2_IMAGE_INCLUDE_DIR}
						${SDL2_MIXER_INCLUDE_DIR}
						)
link_directories(		${LFT_PATH}
						${SDL2_LIBRARIES}
						${SDL2_TTF_LIBRARIES}
						${SDL2_IMAGE_LIBRARIES}
						${SDL2_MIXER_LIBRARIES}
						)
aux_source_directory(	${LFT_LIB} LFT_SRC)
aux_source_directory(	${SRC_PATH} SOURCE_FILES)
add_library(			${LFT_NAME} STATIC ${LFT_SRC})
find_package(SDL2		REQUIRED)
find_package(SDL2_ttf	REQUIRED)
find_package(SDL2_image	REQUIRED)
find_package(SDL2_mixer	REQUIRED)
add_executable(			${CMAKE_PROJECT_NAME} ${SOURCE_FILES} sources/normal_base_obj.c)
target_link_libraries(	${CMAKE_PROJECT_NAME}
						${LFT_NAME}
						${SDL2_LIBRARY}
						${SDL2_TTF_LIBRARIES}
						${SDL2_IMAGE_LIBRARIES}
						${SDL2_MIXER_LIBRARIES}
						)