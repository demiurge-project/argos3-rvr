/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_proximity_sensor.h>
 *
 * @brief This file provides the definition of the rvr proximity
 * sensor.
 *
 * This file provides the definition of the rvr proximity sensor.
 * The sensors are spaced on a ring around the base of the rvr.
 * Therefore, they do not turn with the turret. The readings are in
 * the following order (seeing the RVR from TOP, blue button is
 * the FRONT):
 *
 *              front
 *
 *              0   7
 *     l     1         6    r
 *     e                    i
 *     f    2           5   g
 *     t                    h
 *             3     4      t
 *
 *              back
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_PROXIMITY_SENSOR_H
#define CCI_RVR_PROXIMITY_SENSOR_H

namespace argos
{
    class CCI_RVRProximitySensor;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/config.h>

namespace argos
{
    class CCI_RVRProximitySensor : public CCI_Sensor
    {

    public:
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
          * Angle of the reading
          */
            CRadians Angle;

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
            SReading(Real f_value, const CRadians &c_angle) : Value(f_value), Angle(c_angle) {}
        };

        /**
       * Sensor ring readings
       */
        typedef std::vector<SReading> TReadings;

    public:
        /**
       * Constructor
       */
        CCI_RVRProximitySensor();

        /**
       * Destructor
       */
        virtual ~CCI_RVRProximitySensor() {}

        /**
       * Returns the sensor readings.
       * @return The sensor readings
       */
        inline const TReadings &GetReadings() const
        {
            return m_tReadings;
        }

        /**
       * Returns a single sensor reading.
       * @param i The id of the wanted sensor
       * @return The reading of the wanted sensor
       */
        inline const SReading &GetReading(UInt16 i) const
        {
            /* TODO: maybe an exception or smthg if i < 0 or i > 7 ? */
            return m_tReadings[i];
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
        TReadings m_tReadings;
    };

    /**
    * Redefine the "<<" operator for one reading
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRProximitySensor::SReading &s_reading);

    /**
    * Redefine the "<<" operator for multiple readings
    */
    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRProximitySensor::TReadings &t_readings);
}
#endif
