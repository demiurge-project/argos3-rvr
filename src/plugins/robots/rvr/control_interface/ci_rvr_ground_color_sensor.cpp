/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_ground_color_sensor.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_ground_color_sensor.h"

#ifdef ARGOS_WITH_LUA

#include <argos3/core/wrappers/lua/lua_utility.h>

#endif

namespace argos
{

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA

    void CCI_RVRGroundColorSensor::CreateLuaState(lua_State *pt_lua_state)
    {
        CLuaUtility::StartTable(pt_lua_state, "ground");
        CLuaUtility::AddToTable(pt_lua_state, "color", m_sReading.Color);
        CLuaUtility::EndTable(pt_lua_state);
    }

#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA

    void CCI_RVRGroundColorSensor::ReadingsToLuaState(lua_State *pt_lua_state)
    {
        lua_getfield(pt_lua_state, -1, "ground");
        lua_pushnumber(pt_lua_state, m_sReading.Color.GetRed());
        lua_setfield(pt_lua_state, -2, "red");
        lua_pushnumber(pt_lua_state, m_sReading.Color.GetGreen());
        lua_setfield(pt_lua_state, -2, "green");
        lua_pushnumber(pt_lua_state, m_sReading.Color.GetBlue());
        lua_setfield(pt_lua_state, -2, "blue");
        lua_pop(pt_lua_state, 1);
    }

#endif

    /****************************************/
    /****************************************/

} // namespace argos