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
project(VehicleData LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.16)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

idlcxx_generate(TARGET VehicleData FILES ../idl/VehicleData.idl WARNINGS no-implicit-extensibility)

# VehicleDataReaderListener.hpp VehicleTrackingInfo.hpp
add_executable(Vehicle publisher.cpp )
add_executable(VehicleTracker subscriber.cpp VehicleDataReaderListener.cpp VehicleTrackingInfo.cpp)

# Link both executables to idl data type library and ddscxx.
target_link_libraries(Vehicle CycloneDDS-CXX::ddscxx VehicleData)
target_link_libraries(VehicleTracker CycloneDDS-CXX::ddscxx VehicleData)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(Vehicle PRIVATE -fno-analyzer)
    target_compile_options(VehicleTracker PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET Vehicle PROPERTY CXX_STANDARD 11)
set_property(TARGET VehicleTracker PROPERTY CXX_STANDARD 11)
