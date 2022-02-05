/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_locator_sensor.h>
 *
 * @brief This file provides the definition of the RVR locator positioning sensor.
 *
 * This sensor provides 2 measures : X and Y that describe
 * the relative robot position with respect to its initial (booting) position.
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_LOCATOR_SENSOR_H
#define CCI_RVR_LOCATOR_SENSOR_H

namespace argos
{
    class CCI_RVRLocatorSensor;
}

#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/config.h>

namespace argos
{
    class CCI_RVRLocatorSensor : public CCI_Sensor
    {

    private:
    public:
        /**
       * Single quaternion reading
       */
        struct SReading
        {
            /**
          * Relative position of the robot
          */
            CVector2 Position;

            /**
          * Empty constructor that is initialized at (0,0)
          */
            SReading() : Position(0, 0) {}

            /**
          * Filled constructor
          *
          * @param c_angle the angle of the reading
          */
            SReading(const Real &x, const Real &y) : Position(x, y) {}
        };

    public:
        /**
       * Constructor
       */
        CCI_RVRLocatorSensor();

        /**
       * Destructor
       */
        virtual ~CCI_RVRLocatorSensor() {}

        /**
       * Returns a single sensor reading.
       * @param i The id of the wanted sensor
       * @return The reading of the wanted sensor
       */
        inline const SReading &GetReading() const
        {

            return m_sReading;
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
       * Store the reading
       */
        SReading m_sReading;

        /** Initial position */
        CVector2 m_sInitialPosition;
    };

    /**
    * Redefine the "<<" operator for one reading
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRLocatorSensor::SReading &s_reading);
}
#endif
