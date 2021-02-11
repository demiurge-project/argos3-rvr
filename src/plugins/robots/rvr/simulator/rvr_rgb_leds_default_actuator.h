/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_rgb_leds_default_actuator.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef RVR_RGB_LEDS_DEFAULT_ACTUATOR_H
#define RVR_RGB_LEDS_DEFAULT_ACTUATOR_H

namespace argos
{
    class CRVRRGBLEDsDefaultActuator;
}

#include "../control_interface/ci_rvr_rgb_leds_actuator.h"
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos
{
    class CRVRRGBLEDsDefaultActuator : public CSimulatedActuator, public CCI_RVRRGBLEDsActuator
    {

    public:
        CRVRRGBLEDsDefaultActuator();
        virtual ~CRVRRGBLEDsDefaultActuator();
        void Init(TConfigurationNode &t_tree);
        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Update();
        virtual void Reset();

    private:
        CLEDEquippedEntity *m_pcLEDEquippedEntity;
        CLEDMedium *m_pcLEDMedium;
    };
} // namespace argos

#endif // RVR_RGB_LEDS_DEFAULT_ACTUATOR_H