/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_rgb_leds_default_actuator.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "rvr_rgb_leds_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos
{

    CRVRRGBLEDsDefaultActuator::CRVRRGBLEDsDefaultActuator() : m_pcLEDEquippedEntity(NULL)
    {
    }

    CRVRRGBLEDsDefaultActuator::~CRVRRGBLEDsDefaultActuator(){};

    void CRVRRGBLEDsDefaultActuator::Init(TConfigurationNode &t_tree)
    {
        try
        {
            CCI_RVRRGBLEDsActuator::Init(t_tree);
            std::string strMedium;
            GetNodeAttribute(t_tree, "medium", strMedium);
            m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
            m_pcLEDEquippedEntity->AddToMedium(*m_pcLEDMedium);
            m_tLEDSettings[5] = CColor::RED;
        }
        catch (CARGoSException &e)
        {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing the RVR RGB LEDs default actuator", e);
        }
    }

    void CRVRRGBLEDsDefaultActuator::SetRobot(CComposableEntity &c_entity)
    {
        m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
        m_pcLEDEquippedEntity->Enable();
    }

    void CRVRRGBLEDsDefaultActuator::Update()
    {
        for (uint i = 0; i < 6; ++i)
        {
            m_pcLEDEquippedEntity->SetLEDColor(i, m_tLEDSettings[i]);
        }
    }

    void CRVRRGBLEDsDefaultActuator::Reset()
    {
        for (uint i = 0; i < 5; ++i)
        {
            m_tLEDSettings[i] = CColor::BLACK;
        }
        m_tLEDSettings[5] = CColor::RED;
    }

    REGISTER_ACTUATOR(CRVRRGBLEDsDefaultActuator,
                      "rvr_rgb_leds", "default",
                      "Raffaele Todesco [raffaele.todesco@ulb.be]",
                      "1.0",
                      "The rvr rgb leds actuator.",
                      "This actuator controls a group of RGB LEDs.\n"
                      "For usage, refer to [ci_rvr_rgb_leds_actuator.h]\n\n"
                      "REQUIRED XML CONFIGURATION\n\n"
                      "   <controllers>\n"
                      "      ...\n"
                      "      <my_controller>\n"
                      "         ...\n"
                      "         <actuators>\n"
                      "            ...\n"
                      "            <rvr_rgb_leds implementation=\"default\" medium=\"leds\"/>\n"
                      "            ...\n"
                      "         <actuators/>\n"
                      "         ...\n"
                      "      <my_controller/>\n"
                      "      ...\n"
                      "   <controllers>\n\n"
                      "The attribute [medium] refers to the [id] of the [led] medium defined\n"
                      "in the [media] xml section\n\n"
                      "   <media>\n"
                      "      ...\n"
                      "      <led id=\"leds\" grid_size=\"1,1,1\"/>\n"
                      "      ...\n"
                      "   </media>\n\n"
                      "OPTIONAL XML CONFIGURATION\n\n"
                      "None.\n",
                      "Usable");
}