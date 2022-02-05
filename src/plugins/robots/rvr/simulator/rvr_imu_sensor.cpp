#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include "rvr_imu_equipped_entity.h"

#include "rvr_imu_sensor.h"

namespace argos
{

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRIMUSensor::CRVRIMUSensor() : m_bAddNoise(false), m_pcRNG(NULL)
    {
    }

    /****************************************/
    /****************************************/

    void CRVRIMUSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRIMUSensor::Init(t_tree);
        Real fNoiseLevel = 0.0f;
        GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
        if (fNoiseLevel < 0.0f)
        {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                 << " of the rvr IMU sensor");
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

    void CRVRIMUSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            //m_pcImuEntity = &(c_entity.GetComponent<CRVRIMUEquippedEntity>("rvr_imu"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr imu sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRIMUSensor::Update()
    {
        CRadians cOrientationX, cOrientationY, cOrientationZ;
        m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ, cOrientationY, cOrientationX);
        m_sReading.Pitch = cOrientationX;
        m_sReading.Roll = cOrientationY;
        m_sReading.Yaw = cOrientationZ;
        if (m_bAddNoise)
        {
            m_sReading.Pitch += CRadians(m_pcRNG->Uniform(m_cNoiseRange));
            m_sReading.Roll += CRadians(m_pcRNG->Uniform(m_cNoiseRange));
            m_sReading.Yaw += CRadians(m_pcRNG->Uniform(m_cNoiseRange));
        }
    }

    /****************************************/
    /****************************************/

    void CRVRIMUSensor::Reset()
    {
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRIMUSensor,
                    "rvr_imu", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr IMU sensor",
                    "The sensor returns the pitch, roll and yaw of the robot\n"
                    "that represents the rotation of the robot.\n"
                    "For usage, refer to [ci_rvr_imu_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_imu implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}
