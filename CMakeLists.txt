cmake_minimum_required(VERSION 3.29)
project(blockchain_A2)

set(CMAKE_CXX_STANDARD 20)

add_executable(blockchain_A2 main.cpp
        src/version_1/dsa/dsa_main.cpp
        src/version_1/dsa/dsa_main.h
        src/version_1/dsa/dsa_verification.cpp
        src/version_1/dsa/dsa_verification.h
        src/version_1/dsa/dsa_signing.cpp
        src/version_1/dsa/dsa_signing.h
        src/version_1/dsa/dsa_key_generation.cpp
        src/version_1/dsa/dsa_key_generation.h

        src/hash/hash.cpp
        src/hash/hash.h

        src/distributed_ledger/transaction.h
        src/distributed_ledger/transaction.cpp
        src/distributed_ledger/block.h
        src/distributed_ledger/block.cpp
        src/distributed_ledger/inventory.h
        src/distributed_ledger/inventory.cpp
        src/distributed_ledger/ledger.h
        src/distributed_ledger/ledger.cpp
        src/version_1/proof_of_authority/poa.h
        src/version_1/proof_of_authority/poa.cpp

)


# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Include OpenSSL headers
include_directories(${OPENSSL_INCLUDE_DIR})

# Link OpenSSL libraries
target_link_libraries(blockchain_A2 OpenSSL::SSL OpenSSL::Crypto)