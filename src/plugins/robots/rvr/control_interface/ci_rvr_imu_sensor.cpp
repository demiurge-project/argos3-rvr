/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_imu_sensor.cpp>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_imu_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos
{

   /****************************************/
   /****************************************/

   CCI_RVRIMUSensor::CCI_RVRIMUSensor() : m_sReading() {}

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RVRIMUSensor::CreateLuaState(lua_State *pt_lua_state)
   {
      CLuaUtility::StartTable(pt_lua_state, "imu");
      CLuaUtility::AddToTable(pt_lua_state, "pitch", m_sReading.Pitch);
      CLuaUtility::AddToTable(pt_lua_state, "roll", m_sReading.Roll);
      CLuaUtility::AddToTable(pt_lua_state, "yaw", m_sReading.Yaw);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RVRIMUSensor::ReadingsToLuaState(lua_State *pt_lua_state)
   {
      lua_getfield(pt_lua_state, -1, "imu");
      CLuaUtility::AddToTable(pt_lua_state, "pitch", m_sReading.Pitch);
      CLuaUtility::AddToTable(pt_lua_state, "roll", m_sReading.Roll);
      CLuaUtility::AddToTable(pt_lua_state, "yaw", m_sReading.Yaw);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}