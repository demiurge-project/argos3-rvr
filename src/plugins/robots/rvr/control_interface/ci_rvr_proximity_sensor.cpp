/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_rvr_proximity_sensor.cpp>
 *
 * @author Raffaele Todesco- <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_proximity_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos
{

    /****************************************/
    /****************************************/

    CCI_RVRProximitySensor::CCI_RVRProximitySensor() : m_tReadings(8)
    {
        // Set the values for the proximity sensor angles
        // +45 degrees
        m_tReadings[0].Angle = CRadians::PI_OVER_FOUR;
        // 0 degrees
        m_tReadings[1].Angle = CRadians(0.0f);
        // -45 degrees = 315 degrees
        m_tReadings[2].Angle = 7.0f * CRadians::PI_OVER_FOUR;
        // -90 degrees = 270 degrees
        m_tReadings[3].Angle = 6.0f * CRadians::PI_OVER_FOUR;
        // -135 degrees = 225 degrees
        m_tReadings[4].Angle = 5.0f * CRadians::PI_OVER_FOUR;
        // 180 degrees
        m_tReadings[5].Angle = 4.0f * CRadians::PI_OVER_FOUR;
        // 135 degrees
        m_tReadings[6].Angle = 3.0f * CRadians::PI_OVER_FOUR;
        // 90 degrees
        m_tReadings[7].Angle = 2.0f * CRadians::PI_OVER_FOUR;
    }

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRProximitySensor::CreateLuaState(lua_State *pt_lua_state)
    {
        CLuaUtility::OpenRobotStateTable(pt_lua_state, "proximity");
        for (size_t i = 0; i < GetReadings().size(); ++i)
        {
            CLuaUtility::StartTable(pt_lua_state, i + 1);
            CLuaUtility::AddToTable(pt_lua_state, "angle", m_tReadings[i].Angle);
            CLuaUtility::AddToTable(pt_lua_state, "value", m_tReadings[i].Value);
            CLuaUtility::EndTable(pt_lua_state);
        }
        CLuaUtility::CloseRobotStateTable(pt_lua_state);
    }
#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRProximitySensor::ReadingsToLuaState(lua_State *pt_lua_state)
    {
        lua_getfield(pt_lua_state, -1, "proximity");
        for (size_t i = 0; i < GetReadings().size(); ++i)
        {
            lua_pushnumber(pt_lua_state, i + 1);
            lua_gettable(pt_lua_state, -2);
            lua_pushnumber(pt_lua_state, m_tReadings[i].Value);
            lua_setfield(pt_lua_state, -2, "value");
            lua_pop(pt_lua_state, 1);
        }
        lua_pop(pt_lua_state, 1);
    }
#endif

    /****************************************/
    /****************************************/

    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRProximitySensor::SReading &s_reading)
    {
        c_os << "SReading("
             << s_reading.Value << ","
             << s_reading.Angle << ")";
        return c_os;
    }

    /****************************************/
    /****************************************/

    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRProximitySensor::TReadings &t_readings)
    {
        if (!t_readings.empty())
        {
            c_os << t_readings[0];
            for (UInt16 i = 1; i < t_readings.size(); ++i)
            {
                c_os << " " << t_readings[i];
            }
        }
        return c_os;
    }

    /****************************************/
    /****************************************/
}
