/**
 * @file <argos3/plugins/robots/rvr/control_interface/rvr_ground_color_sensor.cpp>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

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
        CLuaUtility::AddToTable(pt_lua_state, "color", m_sColor);
        CLuaUtility::EndTable(pt_lua_state);
    }
#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRGroundColorSensor::ReadingsToLuaState(lua_State *pt_lua_state)
    {
        lua_getfield(pt_lua_state, -1, "ground");
        lua_pushnumber(pt_lua_state, m_sColor);
        lua_setfield(pt_lua_state, -2, "color");
        lua_pop(pt_lua_state, 1);
    }
#endif

    /****************************************/
    /****************************************/

} // namespace argos