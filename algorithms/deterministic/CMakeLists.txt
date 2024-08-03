project(DeterministicAlgorithm CXX)

add_library(206507923_322853813_DeterministicAlgorithm SHARED
        DeterministicAlgorithm.cpp
        ../Algorithm.cpp
        ../../common/Logger.cpp
        ../../common/BatteryMeterImpl.cpp
        ../../common/DirtSensorImpl.cpp
        ../../common/WallsSensorImpl.cpp
        ../../common/Direction.cpp
        ../../common/Point.cpp
        ../../common/Utils.cpp
        ../../common/Config.cpp
)

set_target_properties(206507923_322853813_DeterministicAlgorithm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../.."
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../..")

target_link_options(206507923_322853813_DeterministicAlgorithm PUBLIC "-rdynamic")
