/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_wheels_actuator.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_WHEELS_ACTUATOR_H
#define CCI_RVR_WHEELS_ACTUATOR_H

namespace argos {
    class CCI_RVRWheelsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {
    class CCI_RVRWheelsActuator : virtual public CCI_Actuator {

    public:
        /**
         * Maximum linear speed for the wheels in cm/sec
         */
        static const Real MAX_VELOCITY_CM_SEC;

        /**
         * Minimum linear speed for the wheels in cm/sec
         */
        static const Real MIN_VELOCITY_CM_SEC;

    public:
        /**
         * This method set the velocity of the wheels
         *
         * @param f_left_velocity velocity of the left wheel
         * @param f_right_velocity velocity of the right wheel
         */
        virtual void SetLinearVelocity(Real f_left_velocity,
            Real f_right_velocity) = 0;

#ifdef ARGOS_WITH_LUA
        /**
         *
         * @param pt_lua_state
         */
        virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
    };
}

#endif