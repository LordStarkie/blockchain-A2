cmake_minimum_required(VERSION 3.14)
project(blockchain_A2)

set(CMAKE_CXX_STANDARD 20)

add_executable(blockchain_A2
        main.cpp
        src/hash/hash.cpp
        src/dsa/dsa_main.cpp
        src/dsa/dsa_verification.cpp
        src/dsa/dsa_signing.cpp
        src/dsa/dsa_key_generation.cpp
        src/distributed_ledger/transaction.cpp
        src/distributed_ledger/block.cpp
        src/distributed_ledger/inventory.cpp
        src/distributed_ledger/ledger.cpp
        src/proof_of_authority/poa.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        ui.cpp
)

# Include directories for ImGui
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(blockchain_A2 OpenSSL::SSL OpenSSL::Crypto)

# Find OpenGL package
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found: ${OPENGL_FOUND}")
message(STATUS "OpenGL include dirs: ${OPENGL_INCLUDE_DIR}")
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")

# Find GLFW package using pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Find GLEW package
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})

# Link libraries
target_link_libraries(blockchain_A2
        OpenGL::GL
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        OpenSSL::SSL
        OpenSSL::Crypto
)
