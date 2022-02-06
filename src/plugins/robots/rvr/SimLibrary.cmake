#
# RVR simulator code
#
# Headers
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_SIMULATOR
        simulator/dynamics2d_rvr_model.h
        simulator/rvr_entity.h
        simulator/rvr_rgb_leds_default_actuator.h
        simulator/rvr_wheels_default_actuator.h
        simulator/rvr_proximity_default_sensor.h
        simulator/rvr_lidar_sensor.h
        simulator/rvr_quaternion_sensor.h
        simulator/rvr_quaternion_equipped_entity.h
        simulator/rvr_imu_sensor.h
        # simulator/rvr_imu_equipped_entity.h
        simulator/rvr_light_sensor.h
        simulator/rvr_velocity_sensor.h
        simulator/rvr_locator_sensor.h
        simulator/rvr_accelerometer_sensor.h
        simulator/rvr_ground_color_sensor.h)
if (ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_SIMULATOR
            ${ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_SIMULATOR}
            simulator/qtopengl_rvr.h)
endif (ARGOS_COMPILE_QTOPENGL)

# Install location for the RVR simulator headers
install(
        FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_SIMULATOR}
        DESTINATION include/argos3/plugins/robots/rvr/simulator)

# Sources
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR
        ${ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR}
        ${ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_SIMULATOR}
        simulator/dynamics2d_rvr_model.cpp
        simulator/rvr_entity.cpp
        simulator/rvr_rgb_leds_default_actuator.cpp
        simulator/rvr_wheels_default_actuator.cpp
        simulator/rvr_proximity_default_sensor.cpp
        simulator/rvr_lidar_sensor.cpp
        simulator/rvr_quaternion_sensor.cpp
        simulator/rvr_quaternion_equipped_entity.cpp
        simulator/rvr_imu_sensor.cpp
        # simulator/rvr_imu_equipped_entity.cpp
        simulator/rvr_light_sensor.cpp
        simulator/rvr_velocity_sensor.cpp
        simulator/rvr_locator_sensor.cpp
        simulator/rvr_accelerometer_sensor.cpp
        simulator/rvr_ground_color_sensor.cpp)
if (ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR
            ${ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR}
            simulator/qtopengl_rvr.cpp)
endif (ARGOS_COMPILE_QTOPENGL)

#
# Create rvr plugin
#
# Create target
add_library(argos3plugin_${ARGOS_BUILD_FOR}_rvr SHARED
        ${ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR})
# Link libraries
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_rvr
        argos3core_${ARGOS_BUILD_FOR}
        argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
        argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
if (ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_rvr
            argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif (ARGOS_COMPILE_QTOPENGL)

# # Install location for the rvr plugin
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_rvr
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/argos3
        ARCHIVE DESTINATION lib/argos3)
