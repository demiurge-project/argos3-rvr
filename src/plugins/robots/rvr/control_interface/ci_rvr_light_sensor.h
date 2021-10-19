/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_light_sensor.h>
 *
 * @brief This file provides the definition of the rvr ambient light sensor.
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_LIGHT_SENSOR_H
#define CCI_RVR_LIGHT_SENSOR_H

namespace argos
{
    class CCI_RVRLightSensor;
}

#include <vector>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos
{

    class CCI_RVRLightSensor : virtual public CCI_Sensor
    {

    public:
        /**
       * Constructor
       */
        CCI_RVRLightSensor();

        /**
       * Destructor
       */
        virtual ~CCI_RVRLightSensor() {}

        /**
       * Single sensor reading
       */
        struct SReading
        {
            /**
          * Value of the reading
          */
            Real Value;

            /**
          * Empty constructor
          */
            SReading() : Value(0.0f) {}

            /**
       * Filled constructor
       *
       * @param f_value the value of the reading
       *
       * @param c_angle the angle of the reading
       */
            SReading(Real f_value) : Value(f_value) {}
        };

        /**
       * Returns a single sensor reading.
       * @param i The id of the wanted sensor
       * @return The reading of the wanted sensor
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
    };

    /**
    * Redefine the "<<" operator with one reading.
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRLightSensor::SReading &s_reading);
}
#endif