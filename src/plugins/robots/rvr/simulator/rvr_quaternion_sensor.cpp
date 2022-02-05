#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include "rvr_quaternion_equipped_entity.h"

#include "rvr_quaternion_sensor.h"

namespace argos
{

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRQuaternionSensor::CRVRQuaternionSensor() : m_pcRNG(NULL), m_bAddNoise(false)
    {
    }

    /****************************************/
    /****************************************/

    void CRVRQuaternionSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRQuaternionSensor::Init(t_tree);
        Real fNoiseLevel = 0.0f;
        GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
        if (fNoiseLevel < 0.0f)
        {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                 << " of the rvr proximity sensor");
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

    void CRVRQuaternionSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcQuaternionEntity = &(c_entity.GetComponent<CRVRQuaternionEquippedEntity>("rvr_quaternion"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            // immediately store inverse to compute it once
            // since only inverse will be used to compute relative rotation
            m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse().Normalize();
            m_pcQuaternionEntity->Enable();
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr quaternion sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRQuaternionSensor::Update()
    {
        //m_sReading.Orientation = m_pcQuaternionEntity->GetOrientation();
        m_sReading.Orientation = m_sInitialOrientation * m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Normalize();
        if (m_bAddNoise)
        {
            m_sReading.Orientation.Set(
                m_sReading.Orientation.GetW() + m_pcRNG->Uniform(m_cNoiseRange),
                m_sReading.Orientation.GetX() + m_pcRNG->Uniform(m_cNoiseRange),
                m_sReading.Orientation.GetY() + m_pcRNG->Uniform(m_cNoiseRange),
                m_sReading.Orientation.GetZ() + m_pcRNG->Uniform(m_cNoiseRange));
        }
    }

    /****************************************/
    /****************************************/

    void CRVRQuaternionSensor::Reset()
    {
        m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse();
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRQuaternionSensor,
                    "rvr_quaternion", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr quaternion sensor",
                    "The sensor returns a quaternion\n"
                    "that represents the relative rotation of the robot with respect to its starting orientation\n"
                    "For usage, refer to [ci_rvr_quaternion_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_quaternion implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}