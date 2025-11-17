cmake_minimum_required(VERSION 3.10)
project(tig C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)

SET(BREW_DIR "/opt/homebrew")
set(PROTOBUF_C_INCLUDE_DIRS "${BREW_DIR}/include")
set(PROTOBUF_C_LIBRARIES "${BREW_DIR}/lib/libprotobuf-c.a")

set(PROTO_NAME tig)
set(PROTO_FILE "${PROTO_NAME}.proto")
set(PROTO_OUT_DIR "${PROJECT_SOURCE_DIR}/proto")
set(PROTO_C_SRC "${PROTO_OUT_DIR}/${PROTO_NAME}.pb-c.c")
set(PROTO_C_HDR "${PROTO_OUT_DIR}/${PROTO_NAME}.pb-c.h")

find_program(PROTOC_C_EXECUTABLE protoc-c
    PATHS "${BREW_DIR}/bin"
    REQUIRED
)

add_custom_command(
    OUTPUT ${PROTO_C_SRC} ${PROTO_C_HDR}
    COMMAND ${PROTOC_C_EXECUTABLE} --c_out=${PROTO_OUT_DIR} -I${PROJECT_SOURCE_DIR} ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating C source from ${PROTO_FILE}"
)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include ${PROTO_OUT_DIR})
file(GLOB C_SRCS ${PROTO_C_SRC} ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(tig ${C_SRCS})
target_link_libraries(tig PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL
    ${PROTOBUF_C_LIBRARIES}
)

target_include_directories(tig PRIVATE
    ${PROTOBUF_C_INCLUDE_DIRS}
    ${INCLUDE_DIR}
)
