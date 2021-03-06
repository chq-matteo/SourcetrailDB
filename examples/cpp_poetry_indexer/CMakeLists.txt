cmake_minimum_required (VERSION 2.6)

set(EXAMPLE_NAME "poetry_indexer")

# --- Configure Build ---

set(EXAMPLE_SRC_FILES
	src/main.cpp
)

set(EXAMPLE_HDR_FILES
)

add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC_FILES} ${EXAMPLE_HDR_FILES})

target_include_directories(${EXAMPLE_NAME} PUBLIC
	"${CORE_SOURCE_DIR}/include"
)

target_link_libraries(${EXAMPLE_NAME} ${LIB_CORE} "-ldl")


# --- Configure Sourcetrail Project File ---

add_custom_command(
	TARGET ${EXAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXAMPLE_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE_NAME}
)

set(POETRY_INDEXER_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}.srctrlprj.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE_NAME}.srctrlprj"
)
