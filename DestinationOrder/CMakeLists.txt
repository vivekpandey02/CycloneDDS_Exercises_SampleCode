#
# Copyright(c) 2023 ZettaScale Technology and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#

cmake_minimum_required(VERSION 3.16)
project(CycloneDDS-Examples_VehicleData VERSION 0.11.0 LANGUAGES CXX)

find_package(CycloneDDS-CXX REQUIRED)

set(CMAKE_CXX_STANDARD 17)
#get_target_property(CMAKE_CXX_STANDARD CycloneDDS-CXX::ddscxx CXX_STANDARD)

set(cyclonedds_cpp_std_to_use ${CMAKE_CXX_STANDARD})

idlcxx_generate(TARGET VehicleData_DestinationOrder FILES ../idl/VehicleData.idl WARNINGS no-implicit-extensibility)

add_executable(ddscxxDestinationOrder_VehicleDataPublisher publisher.cpp )
add_executable(ddscxxDestinationOrder_VehicleDataSubscriber subscriber.cpp VehicleDataReaderListener.hpp VehicleDataReaderListener.cpp)

# Link both executables to idl data type library and ddscxx.
target_link_libraries(ddscxxDestinationOrder_VehicleDataPublisher CycloneDDS-CXX::ddscxx VehicleData_DestinationOrder)
target_link_libraries(ddscxxDestinationOrder_VehicleDataSubscriber CycloneDDS-CXX::ddscxx VehicleData_DestinationOrder)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(ddscxxDestinationOrder_VehicleDataPublisher PRIVATE -fno-analyzer)
    target_compile_options(ddscxxDestinationOrder_VehicleDataSubscriber PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET ddscxxDestinationOrder_VehicleDataPublisher PROPERTY CXX_STANDARD ${cyclonedds_cpp_std_to_use})
set_property(TARGET ddscxxDestinationOrder_VehicleDataSubscriber PROPERTY CXX_STANDARD ${cyclonedds_cpp_std_to_use})
