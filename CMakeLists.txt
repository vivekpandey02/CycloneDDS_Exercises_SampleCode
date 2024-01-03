cmake_minimum_required(VERSION 3.16)
project(CycloneDDS-Examples_VehicleData VERSION 0.11.0 LANGUAGES CXX)

find_package(CycloneDDS-CXX REQUIRED)

set(CMAKE_CXX_STANDARD 17)
#get_target_property(CMAKE_CXX_STANDARD CycloneDDS-CXX::ddscxx CXX_STANDARD)

set(cyclonedds_cpp_std_to_use ${CMAKE_CXX_STANDARD})

add_subdirectory(SimplePubSub)
add_subdirectory(Listener)    
add_subdirectory(WaitSet)  
add_subdirectory(Durability)  
add_subdirectory(Reliability)
add_subdirectory(Liveliness)
add_subdirectory(Deadline)   
add_subdirectory(Partition)   
add_subdirectory(Ownership)
add_subdirectory(DestinationOrder)
