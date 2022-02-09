/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_gyroscope_sensor.h>
 *
 * @brief This file provides the definition of the rvr gyroscope sensor.
 *
 * This sensor provides robot angulaer velocity around X, Y and Z axis in radians/s
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_GYROSCOPE_SENSOR_H
#define CCI_RVR_GYROSCOPE_SENSOR_H

namespace argos {
    class CCI_RVRGyroscopeSensor;
}

#include <vector>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/control_interface/ci_sensor.h>


namespace argos {

    class CCI_RVRGyroscopeSensor : virtual public CCI_Sensor {

    public:
        /**
       * Constructor
       */
        CCI_RVRGyroscopeSensor();

        /**
       * Destructor
       */
        ~CCI_RVRGyroscopeSensor() override {}

        /**
       * Single sensor reading
       */
        struct SReading {
            /** Acceleration vector */
            CVector3 AngularVelocity;

            /**
            * Empty constructor
            */
            SReading() : AngularVelocity(0, 0, 0) {}

            /**
           * Filled constructor
           *
           */
            SReading(Real x_vel, Real y_vel, Real z_vel) : AngularVelocity(x_vel,
                                                                           y_vel,
                                                                           z_vel) {}
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

        /** Store previous angular position */
        CQuaternion m_sPreviousAngularPosition;
    };

    /**
    * Redefine the "<<" operator with one reading.
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRGyroscopeSensor::SReading &s_reading);
}


#endif //CCI_RVR_GYROSCOPE_SENSOR_H
