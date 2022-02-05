#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "rvr_locator_sensor.h"

namespace argos
{

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRLocatorSensor::CRVRLocatorSensor() : m_pcRNG(NULL), m_bAddNoise(false)
    {
    }

    /****************************************/
    /****************************************/

    void CRVRLocatorSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRLocatorSensor::Init(t_tree);
        Real fNoiseLevel = 0.0f;
        GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
        if (fNoiseLevel < 0.0f)
        {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                 << " of the rvr locator sensor");
        }
        else if (fNoiseLevel > 0.0f)
        {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
        }
    }

    /****************************************/
    /****************************************/

    void CRVRLocatorSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            m_sInitialPosition = CVector2(m_pcEmbodiedEntity->GetOriginAnchor().Position.GetX(), m_pcEmbodiedEntity->GetOriginAnchor().Position.GetY());
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr locator sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRLocatorSensor::Update()
    {
        m_sReading.Position = CVector2(m_pcEmbodiedEntity->GetOriginAnchor().Position.GetX(), m_pcEmbodiedEntity->GetOriginAnchor().Position.GetY()) - m_sInitialPosition;
        if (m_bAddNoise)
        {
            m_sReading.Position.Set(
                m_sReading.Position.GetX() + m_pcRNG->Uniform(m_cNoiseRange),
                m_sReading.Position.GetY() + m_pcRNG->Uniform(m_cNoiseRange));
        }
    }

    /****************************************/
    /****************************************/

    void CRVRLocatorSensor::Reset()
    {
        m_sInitialPosition = CVector2(m_pcEmbodiedEntity->GetOriginAnchor().Position.GetX(), m_pcEmbodiedEntity->GetOriginAnchor().Position.GetY());
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRLocatorSensor,
                    "rvr_locator", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr locator sensor",
                    "The sensor returns an (x,y)\n"
                    "that represents the relative position of the robot with respect to its starting position\n"
                    "For usage, refer to [ci_rvr_position_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_locator implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}