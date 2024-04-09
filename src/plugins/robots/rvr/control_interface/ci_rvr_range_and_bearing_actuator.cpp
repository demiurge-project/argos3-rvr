/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_range_and_bearing_default_actuator.cpp>
 *
 * @author Miquel Kegeleirs <miquel.kegeleirs@ulb.be>
 */

#include "ci_rvr_range_and_bearing_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack should have one array of size GetDataSize()
    * and values in the range [0,255]
    */
   int LuaRABSetData(lua_State* pt_lua_state) {
      if(lua_gettop(pt_lua_state) == 1) {
         /* Check parameters */
         luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
         /* Get reference to actuator */
         CCI_RVRRangeAndBearingActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_RVRRangeAndBearingActuator>(pt_lua_state, "range_and_bearing");
         /* Check whether sizes match */
         if(CCI_RVRRangeAndBearingActuator::MAX_BYTES_SENT != lua_rawlen(pt_lua_state, -1)) {
            return luaL_error(pt_lua_state, "robot.range_and_bearing.set_data(array) expects an array of %d numbers", CCI_RVRRangeAndBearingActuator::MAX_BYTES_SENT);
         }
         /* Fill up a byte array, checking that all elements are numbers */
         CCI_RVRRangeAndBearingActuator::TData tBuf;
         for(size_t i = 0; i < CCI_RVRRangeAndBearingActuator::MAX_BYTES_SENT ; ++i) {
            lua_pushnumber(pt_lua_state, i+1);
            lua_gettable(pt_lua_state, -2);
            if(lua_type(pt_lua_state, -1) == LUA_TNUMBER) {
               tBuf[i] = static_cast<UInt8>(lua_tonumber(pt_lua_state, -1));
               lua_pop(pt_lua_state, 1);
            }
            else {
               return luaL_error(pt_lua_state, "element #%d of the array is not a number", i+1);
            }
         }
         /* Perform action */
         pcAct->SetData(tBuf);
         return 0;
      }
      else {
         return luaL_error(pt_lua_state, "robot.range_and_bearing.set_data() expects one arguments");
      }
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RVRRangeAndBearingActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "range_and_bearing");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_data", &LuaRABSetData);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif
   
   /****************************************/
   /****************************************/

   void CCI_RVRRangeAndBearingActuator::Init(TConfigurationNode& t_tree) {
      try {
         /* get the data size from the configuration file or set it to the data struct size */
         /* sizeof return the number of byte */

           GetNodeAttributeOrDefault(t_tree, "data_size", m_uDataSize, sizeof(TData));
         if (m_uDataSize > sizeof(TData)) {
            LOGERR << "[WARNING] RAB actuator data_size=" << m_uDataSize
                   << ", value incorrect, back to default value data_size=" << sizeof(TData) << std::endl;
            m_uDataSize = sizeof(TData);
         }
      } catch (CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing range-and-bearing actuator", ex);
      }
   }
   /****************************************/
   /****************************************/

   void CCI_RVRRangeAndBearingActuator::SetData(const TData t_data) {
      m_sDesiredData.State = STATE_ALL_EMITTERS_SAME_DATA;
      for(size_t i = 0; i < 12; ++i) {
         m_sDesiredData.Emitter[i].Enabled = true;
         m_sDesiredData.Emitter[i].Data[0] = t_data[0];
         m_sDesiredData.Emitter[i].Data[1] = t_data[1];
         m_sDesiredData.Emitter[i].Data[2] = t_data[2];
         m_sDesiredData.Emitter[i].Data[3] = t_data[3];
      }
   }

   /****************************************/
   /****************************************/

   void CCI_RVRRangeAndBearingActuator::SetDataForEmitter(size_t un_idx,
                                                             const TData t_data) {
      m_sDesiredData.State = STATE_EMITTERS_DIFFERENT;
      m_sDesiredData.Emitter[un_idx].Enabled = true;
      m_sDesiredData.Emitter[un_idx].Data[0] = t_data[0];
      m_sDesiredData.Emitter[un_idx].Data[1] = t_data[1];
      m_sDesiredData.Emitter[un_idx].Data[2] = t_data[2];
      m_sDesiredData.Emitter[un_idx].Data[3] = t_data[3];
   }

   /****************************************/
   /****************************************/

   void CCI_RVRRangeAndBearingActuator::Disable() {
      m_sDesiredData.State = STATE_ALL_EMITTERS_DISABLED;
      for(size_t i = 0; i < 12; ++i) {
         m_sDesiredData.Emitter[i].Enabled = false;
      }
   }

   /****************************************/
   /****************************************/

   void CCI_RVRRangeAndBearingActuator::DisableEmitter(size_t un_idx) {
      m_sDesiredData.State = STATE_EMITTERS_DIFFERENT;
      m_sDesiredData.Emitter[un_idx].Enabled = false;
   }


   /****************************************/
   /****************************************/
   CCI_RVRRangeAndBearingActuator::SDataToSend::SDataToSend() :
      State(STATE_ALL_EMITTERS_DISABLED) {
      for(size_t i = 0; i < 12; ++i) {
         Emitter[i].Enabled = false;
         ::memset(&(Emitter[i].Data), 0, sizeof(Emitter[i].Data));
      }
   }

   CCI_RVRRangeAndBearingActuator::SDataToSend::SDataToSend(const CCI_RVRRangeAndBearingActuator::SDataToSend& s_data) :
      State(s_data.State) {
      for(size_t i = 0; i < 12; ++i) {
         Emitter[i].Enabled = s_data.Emitter[i].Enabled;
         Emitter[i].Data[0] = s_data.Emitter[i].Data[0];
         Emitter[i].Data[1] = s_data.Emitter[i].Data[1];
         Emitter[i].Data[2] = s_data.Emitter[i].Data[2];
         Emitter[i].Data[3] = s_data.Emitter[i].Data[3];
      }
   }

   CCI_RVRRangeAndBearingActuator::SDataToSend&
   CCI_RVRRangeAndBearingActuator::SDataToSend::operator=(
           const CCI_RVRRangeAndBearingActuator::SDataToSend& s_data) {
      if(this != &s_data) {
         State = s_data.State;
         for(size_t i = 0; i < 12; ++i) {
            Emitter[i].Enabled = s_data.Emitter[i].Enabled;
            Emitter[i].Data[0] = s_data.Emitter[i].Data[0];
            Emitter[i].Data[1] = s_data.Emitter[i].Data[1];
            Emitter[i].Data[2] = s_data.Emitter[i].Data[2];
            Emitter[i].Data[3] = s_data.Emitter[i].Data[3];

         }
      }
      return *this;
   }

   /****************************************/
   /****************************************/
}
