/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_quaternion_sensor.h>
 *
 * @brief This file provides the definition of the RVR quaternion orientation sensor.
 *
 * This sensor provides 4 measures : W, X, Y, and Z as a quaternion that describes 
 * the relative robot orientation with respect to its initial (booting) orientation.
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_QUATERNION_SENSOR_H
#define CCI_RVR_QUATERNION_SENSOR_H

namespace argos
{
  class CCI_RVRQuaternionSensor;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/config.h>

namespace argos
{
  class CCI_RVRQuaternionSensor : public CCI_Sensor
  {

  private:
    public:
    /**
       * Single quaternion reading
       */
    struct SReading
    {
      /**
          * Relative orientation of the robot
          */
      CQuaternion Orientation;

      /**
          * Empty constructor that is initialized at (1,0,0,0) which means no rotation
          */
      SReading() : Orientation() {}

      /**
          * Filled constructor
          *
          * @param c_angle the angle of the reading
          */
      SReading(const CQuaternion &c_angle) : Orientation(c_angle) {}
    };

  public:
    /**
       * Constructor
       */
    CCI_RVRQuaternionSensor();

    /**
       * Destructor
       */
    virtual ~CCI_RVRQuaternionSensor() {}

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

    /** Store initial orientation */
    CQuaternion m_sInitialOrientation;
  };

  /**
    * Redefine the "<<" operator for one reading
    */
  std::ostream &operator<<(std::ostream &c_os,
                           const CCI_RVRQuaternionSensor::SReading &s_reading);
}
#endif
