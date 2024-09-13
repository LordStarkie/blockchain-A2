cmake_minimum_required(VERSION 3.29)
project(blockchain_A2)

set(CMAKE_CXX_STANDARD 20)

add_executable(blockchain_A2 main.cpp
        src/version_1/dsi/digital_signature.h
        src/version_1/dsi/digital_signature.cpp
        src/hash/hash.cpp
        src/hash/hash.h)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Include OpenSSL headers
include_directories(${OPENSSL_INCLUDE_DIR})

# Link OpenSSL libraries
target_link_libraries(blockchain_A2 OpenSSL::SSL OpenSSL::Crypto)