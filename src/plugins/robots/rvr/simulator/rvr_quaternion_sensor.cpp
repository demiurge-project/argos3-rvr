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

    CRVRQuaternionSensor::CRVRQuaternionSensor()
    {
    }

    /****************************************/
    /****************************************/

    void CRVRQuaternionSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRQuaternionSensor::Init(t_tree);
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
        m_sReading = m_sInitialOrientation * m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Normalize();
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