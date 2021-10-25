/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_velocity_sensor.h>
 *
 * @brief This file provides the definition of the rvr velocity sensor.
 * 
 * This sensor provides robot velocity relative X and Y axis 
 * (relative to its booting position, resetting the robot heading does not modify this sensor behaviour)
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_VELOCITY_SENSOR_H
#define CCI_RVR_VELOCITY_SENSOR_H

namespace argos
{
    class CCI_RVRVelocitySensor;
}

#include <vector>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos
{

    class CCI_RVRVelocitySensor : virtual public CCI_Sensor
    {

    public:
        /**
       * Constructor
       */
        CCI_RVRVelocitySensor();

        /**
       * Destructor
       */
        virtual ~CCI_RVRVelocitySensor() {}

        /**
       * Single sensor reading
       */
        struct SReading
        {
            /**
          * Velocity along X axis
          */
            Real XVelocity;

            /**
             * Velocity along Y axis
             * */
            Real YVelocity;
            /**
          * Empty constructor
          */
            SReading() : XVelocity(0.0f), YVelocity(0.0f) {}

            /**
       * Filled constructor
       *
       */
            SReading(Real x_velocity, Real y_velocity) : XVelocity(x_velocity), YVelocity(y_velocity) {}
        };

        /**
       * Returns the sensor reading.
       * @return The reading of the sensor
       */
        inline const SReading &GetReading() const
        {
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
    };

    /**
    * Redefine the "<<" operator with one reading.
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRVelocitySensor::SReading &s_reading);
}
#endif