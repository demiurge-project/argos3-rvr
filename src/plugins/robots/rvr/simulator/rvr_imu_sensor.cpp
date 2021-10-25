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

    CRVRIMUSensor::CRVRIMUSensor()
    {
    }

    /****************************************/
    /****************************************/

    void CRVRIMUSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRIMUSensor::Init(t_tree);
    }

    /****************************************/
    /****************************************/

    void CRVRIMUSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcImuEntity = &(c_entity.GetComponent<CRVRIMUEquippedEntity>("rvr_imu"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            m_pcImuEntity->Enable();
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
