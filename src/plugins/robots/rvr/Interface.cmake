#
# RVR control interface headers
#
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_CONTROLINTERFACE
        # Sensors
        control_interface/ci_rvr_proximity_sensor.h
        control_interface/ci_rvr_lidar_sensor.h
        control_interface/ci_rvr_ground_color_sensor.h
        control_interface/ci_rvr_quaternion_sensor.h
        control_interface/ci_rvr_imu_sensor.h
        control_interface/ci_rvr_light_sensor.h
        control_interface/ci_rvr_velocity_sensor.h
        control_interface/ci_rvr_locator_sensor.h
        control_interface/ci_rvr_accelerometer_sensor.h
        control_interface/ci_rvr_gyroscope_sensor.h
	control_interface/ci_rvr_colored_blob_omnidirectional_camera_sensor.h
	control_interface/ci_rvr_range_and_bearing_sensor.h
        # Actuators
        control_interface/ci_rvr_rgb_leds_actuator.h
        control_interface/ci_rvr_wheels_actuator.h
        control_interface/ci_rvr_range_and_bearing_actuator.h)

#
# RVR control interface sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_RVR
        ${ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_CONTROLINTERFACE}
        # Sensors
        control_interface/ci_rvr_proximity_sensor.cpp
        control_interface/ci_rvr_lidar_sensor.cpp
        control_interface/ci_rvr_ground_color_sensor.cpp
        control_interface/ci_rvr_quaternion_sensor.cpp
        control_interface/ci_rvr_imu_sensor.cpp
        control_interface/ci_rvr_light_sensor.cpp
        control_interface/ci_rvr_velocity_sensor.cpp
        control_interface/ci_rvr_locator_sensor.cpp
        control_interface/ci_rvr_accelerometer_sensor.cpp
        control_interface/ci_rvr_gyroscope_sensor.cpp
	control_interface/ci_rvr_colored_blob_omnidirectional_camera_sensor.cpp
	control_interface/ci_rvr_range_and_bearing_sensor.cpp
        # Actuators
        control_interface/ci_rvr_rgb_leds_actuator.cpp
        control_interface/ci_rvr_wheels_actuator.cpp
        control_interface/ci_rvr_range_and_bearing_actuator.cpp)

#
# Install location for the RVR control interface headers
#
install(
        FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_RVR_CONTROLINTERFACE}
        DESTINATION include/argos3/plugins/robots/rvr/control_interface)
