/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_accelerometer_sensor.h>
 *
 * @brief This file provides the definition of the rvr accelerometer sensor.
 *
 * This sensor provides robot velocity in X, Y and Z axis in g
 * (relative to its booting position, resetting the robot heading does not modify this sensor behaviour)
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_ACCELEROMETER_SENSOR_H
#define CCI_RVR_ACCELEROMETER_SENSOR_H

namespace argos {
    class CCI_RVRAccelerometerSensor;
}

#include <vector>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/control_interface/ci_sensor.h>


namespace argos {

    class CCI_RVRAccelerometerSensor : virtual public CCI_Sensor {

    public:
        /**
       * Constructor
       */
        CCI_RVRAccelerometerSensor();

        /**
       * Destructor
       */
        ~CCI_RVRAccelerometerSensor() override {}

        /**
       * Single sensor reading
       */
        struct SReading {
            /** Acceleration vector */
            CVector3 Acceleration;

            /**
            * Empty constructor
            */
            SReading() : Acceleration(0, 0, 0) {}

            /**
           * Filled constructor
           *
           */
            SReading(Real x_acceleration, Real y_acceleration, Real z_acceleration) : Acceleration(x_acceleration,
                                                                                                   y_acceleration,
                                                                                                   z_acceleration) {}
        };

        /**
       * Returns the sensor reading.
       * @return The reading of the sensor
       */
        inline const SReading &GetReading() const {
            return m_tReading;
        }

#ifdef ARGOS_WITH_LUA

        /**
       *
       * @param pt_lua_state
       */
        virtual void CreateLuaState(lua_State *pt_lua_state);

        /**
       *
       * @param pt_lua_state
       */
        virtual void ReadingsToLuaState(lua_State *pt_lua_state);

#endif

    protected:
        /**
       * Store the readings
       */
        SReading m_tReading;

        /** Store initial orientation */
        CQuaternion m_sInitialOrientation;

        /** Store previous velocity */
        CVector2 m_sPreviousVelocity;
    };

    /**
    * Redefine the "<<" operator with one reading.
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRAccelerometerSensor::SReading &s_reading);
}


#endif //CCI_RVR_ACCELEROMETER_SENSOR_H
