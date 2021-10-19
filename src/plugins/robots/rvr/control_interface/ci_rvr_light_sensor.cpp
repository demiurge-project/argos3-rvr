/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_light_sensor.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_light_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos
{

    /****************************************/
    /****************************************/

    CCI_RVRLightSensor::CCI_RVRLightSensor() : m_tReading(0.0f)
    {
    }

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRLightSensor::CreateLuaState(lua_State *pt_lua_state)
    {
        CLuaUtility::OpenRobotStateTable(pt_lua_state, "light");
        CLuaUtility::StartTable(pt_lua_state, 1);
        CLuaUtility::AddToTable(pt_lua_state, "value", m_tReading.Value);
        CLuaUtility::EndTable(pt_lua_state);
        CLuaUtility::CloseRobotStateTable(pt_lua_state);
    }
#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRLightSensor::ReadingsToLuaState(lua_State *pt_lua_state)
    {
        lua_getfield(pt_lua_state, -1, "light");
        lua_pushnumber(pt_lua_state, 1);
        lua_gettable(pt_lua_state, -2);
        lua_pushnumber(pt_lua_state, m_tReading.Value);
        lua_setfield(pt_lua_state, -2, "value");
        lua_pop(pt_lua_state, 1);
        lua_pop(pt_lua_state, 1);
    }
#endif

    /****************************************/
    /****************************************/

    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRLightSensor::SReading &s_reading)
    {
        c_os << "SReading("
             << s_reading.Value << ")";
        return c_os;
    }

    /****************************************/
    /****************************************/
}