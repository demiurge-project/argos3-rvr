/**
 * @file <argos3/plugins/robots/rvr/control_interface/ci_rvr_rgb_leds_actuator.h>
 *
 * This file provides the definition of the RVR LEDs actuator.
 *
 *
 * The user can switch on the 5 RGB LEDs on each side, specifying the RGB color.
 *
 * The LEDs are distributed as follows: (View from the top)
 *
 *        front
 *
 *  l    0      1     r
 *  e                 i
 *  f  2          3   g
 *  t                 h
 *          4         t
 *         back
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef CCI_RVR_RGB_LEDS_ACTUATOR_H
#define CCI_RVR_RGB_LEDS_ACTUATOR_H

#include <argos3/core/utility/logging/argos_log.h>

namespace argos
{
    class CCI_RVRRGBLEDsActuator;
}

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/control_interface/ci_actuator.h>

namespace argos
{

}

namespace argos
{
    class CCI_RVRRGBLEDsActuator : public CCI_Actuator
    {
    public:
        /**
         * RGB LED settings data type
         */

        typedef std::vector<CColor> TLEDSettings;

        /**
         * Enum class used to facilitate LED usage.
         */
        enum LEDS
        {
            FRONT_LEFT = 0,
            FRONT_RIGHT = 1,
            LEFT_SIDE = 2,
            RIGHT_SIDE = 3,
            BACK = 4
        };

    public:
        CCI_RVRRGBLEDsActuator();

        virtual ~CCI_RVRRGBLEDsActuator() {}

        /**
         * Sets the color of a single LED.
         * @param un_led_number Number of the LED [0-4]
         * @param c_color Wanted color.
         */
        virtual void SetColor(size_t un_led_number,
                              const CColor &c_color);

        /**
         * Sets the same color for all LEDs.
         * @param c_color The wanted color.
         */
        virtual void SetColors(const CColor &c_color);

        /**
         * Sets all LEDs to the given color.
         */
        virtual void SetColors(const TLEDSettings &c_colors);

#ifdef ARGOS_WITH_LUA
        /**
         *
         * @param pt_lua_state
         */
        virtual void CreateLuaState(lua_State *pt_lua_state);
#endif

    protected:
        /**
         * RGB LEDs settings
         */
        TLEDSettings m_tLEDSettings;
    };
}

#endif // CCI_RVR_RGB_LEDS_ACTUATOR_H