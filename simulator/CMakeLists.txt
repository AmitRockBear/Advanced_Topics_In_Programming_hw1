project(Simulator CXX)

add_executable(myrobot
        ../common/AlgorithmRegistrar.cpp
        ../common/BatteryMeterImpl.cpp
        ../common/Config.cpp
        ../common/Direction.cpp
        ../common/DirtSensorImpl.cpp
        FileDataExtractor.cpp
        House.cpp
        ../common/Logger.cpp
        myrobot.cpp
        MySimulator.cpp
        ../common/Point.cpp
        ../common/Utils.cpp
        VacuumCleaner.cpp
        ../common/WallsSensorImpl.cpp
        AlgorithmWrapper.cpp
        HouseWrapper.cpp
        ThreadController.cpp        
)

set_target_properties(myrobot PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_options(myrobot PUBLIC "-rdynamic")

target_link_libraries(myrobot PRIVATE dl)
