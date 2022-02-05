/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_locator_sensor.cpp>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_locator_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos
{

    /****************************************/
    /****************************************/

    CCI_RVRLocatorSensor::CCI_RVRLocatorSensor() : m_sReading(0, 0), m_sInitialPosition(0, 0) {}

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRLocatorSensor::CreateLuaState(lua_State *pt_lua_state)
    {
        CLuaUtility::StartTable(pt_lua_state, "position");
        CLuaUtility::AddToTable(pt_lua_state, "x_pos", m_sReading.Position.GetX());
        CLuaUtility::AddToTable(pt_lua_state, "y_pos", m_sReading.Position.GetY());
        CLuaUtility::EndTable(pt_lua_state);
    }
#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_RVRLocatorSensor::ReadingsToLuaState(lua_State *pt_lua_state)
    {
        lua_getfield(pt_lua_state, -1, "position");
        CLuaUtility::AddToTable(pt_lua_state, "x_pos", m_sReading.Position.GetX());
        CLuaUtility::AddToTable(pt_lua_state, "y_pos", m_sReading.Position.GetY());
        lua_pop(pt_lua_state, 1);
    }
#endif

    /****************************************/
    /****************************************/

}