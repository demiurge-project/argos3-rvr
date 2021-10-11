/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_quaternion_sensor.cpp>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_quaternion_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos
{

   /****************************************/
   /****************************************/

   CCI_RVRQuaternionSensor::CCI_RVRQuaternionSensor() : m_sReading(CQuaternion()), m_sInitialOrientation(CQuaternion()) {}

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RVRQuaternionSensor::CreateLuaState(lua_State *pt_lua_state)
   {
      CLuaUtility::StartTable(pt_lua_state, "quaternion");
      CLuaUtility::AddToTable(pt_lua_state, "orientation", m_sReading.Orientation);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RVRQuaternionSensor::ReadingsToLuaState(lua_State *pt_lua_state)
   {
      lua_getfield(pt_lua_state, -1, "quaternion");
      CLuaUtility::AddToTable(pt_lua_state, "orientation", m_sReading.Orientation);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}