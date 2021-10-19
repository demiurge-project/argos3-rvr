/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_imu_sensor.h>
 *
 * @brief This file provides the definition of the RVR IMU sensor.
 *
 * This sensor provides the robot pitch, roll and yaw as absolute measures.
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_IMU_SENSOR_H
#define CCI_RVR_IMU_SENSOR_H

namespace argos
{
  class CCI_RVRIMUSensor;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/config.h>

namespace argos
{
  class CCI_RVRIMUSensor : public CCI_Sensor
  {

  private:
    public:
    /**
       * Single quaternion reading
       */
    struct SReading
    {
      /**
     * Robot pitch
     */
      CRadians Pitch;

      /**
       * Robot roll
       */
      CRadians Roll;

      /**
       * Robot yaw
       */
      CRadians Yaw;

      /**
          * Empty constructor, every angle is 0 at start
          */
      SReading() : Pitch(), Roll(), Yaw() {}
    };

  public:
    /**
       * Constructor
       */
    CCI_RVRIMUSensor();

    /**
       * Destructor
       */
    virtual ~CCI_RVRIMUSensor() {}

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
  };

  /**
    * Redefine the "<<" operator for one reading
    */
  std::ostream &operator<<(std::ostream &c_os,
                           const CCI_RVRIMUSensor::SReading &s_reading);
}
#endif
