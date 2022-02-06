/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_velocity_sensor.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "ci_rvr_accelerometer_sensor.h"

#ifdef ARGOS_WITH_LUA

#include <argos3/core/wrappers/lua/lua_utility.h>

#endif

namespace argos {

    /****************************************/
    /****************************************/

    CCI_RVRAccelerometerSensor::CCI_RVRAccelerometerSensor() : m_tReading(0.0f, 0.0f, 0.0f),
                                                               m_sInitialOrientation(CQuaternion()),
                                                               m_sPreviousVelocity(0, 0) {
    }

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA

    void CCI_RVRAccelerometerSensor::CreateLuaState(lua_State *pt_lua_state) {
        CLuaUtility::OpenRobotStateTable(pt_lua_state, "velocity");
        CLuaUtility::StartTable(pt_lua_state, 1);
        CLuaUtility::AddToTable(pt_lua_state, "x", m_tReading.Acceleration.GetX());
        CLuaUtility::AddToTable(pt_lua_state, "y", m_tReading.Acceleration.GetY());
        CLuaUtility::AddToTable(pt_lua_state, "z", m_tReading.Acceleration.GetZ());
        CLuaUtility::EndTable(pt_lua_state);
        CLuaUtility::CloseRobotStateTable(pt_lua_state);
    }

#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA

    void CCI_RVRAccelerometerSensor::ReadingsToLuaState(lua_State *pt_lua_state) {
        lua_getfield(pt_lua_state, -1, "velocity");
        lua_pushnumber(pt_lua_state, 1);
        lua_gettable(pt_lua_state, -2);
        lua_pushnumber(pt_lua_state, m_tReading.Acceleration.GetX());
        lua_setfield(pt_lua_state, -2, "x");
        lua_pushnumber(pt_lua_state, m_tReading.Acceleration.GetY());
        lua_setfield(pt_lua_state, -2, "y");
        lua_pushnumber(pt_lua_state, m_tReading.Acceleration.GetZ());
        lua_setfield(pt_lua_state, -2, "y");
        lua_pop(pt_lua_state, 1);
        lua_pop(pt_lua_state, 1);
    }

#endif

    /****************************************/
    /****************************************/

    std::ostream &operator<<(std::ostream &c_os,
                             const CCI_RVRAccelerometerSensor::SReading &s_reading) {
        c_os << "SReading("
             << s_reading.Acceleration.GetX() << ","
             << s_reading.Acceleration.GetY() << ","
             << s_reading.Acceleration.GetZ() << ")";
        return c_os;
    }

    /****************************************/
    /****************************************/
}