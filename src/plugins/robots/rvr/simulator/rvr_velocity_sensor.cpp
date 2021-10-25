#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "rvr_velocity_sensor.h"

namespace argos
{

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRVelocitySensor::CRVRVelocitySensor()
    {
    }

    /****************************************/
    /****************************************/

    void CRVRVelocitySensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RVRVelocitySensor::Init(t_tree);
    }

    /****************************************/
    /****************************************/

    void CRVRVelocitySensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            // immediately store inverse to compute it once
            // since only inverse will be used to compute relative rotation
            m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse().Normalize();
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr velocity sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRVelocitySensor::Update()
    {
        // get average wheel velocity as our robot velocity
        Real velocity = (m_pcWheeledEntity->GetWheelVelocity(0) + m_pcWheeledEntity->GetWheelVelocity(1)) / 2;
        // compute relative orientation
        CQuaternion relativeOrientation = m_sInitialOrientation * m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Normalize();
        // convert to z angle
        CRadians cTmp1, cTmp2, cRotZ;
        relativeOrientation.ToEulerAngles(cRotZ, cTmp1, cTmp2);
        // add 90 degrees since Y is forward
        cRotZ += CRadians::PI_OVER_TWO;
        // Project on X and Y to get readings
        m_tReading.XVelocity = velocity * Cos(cRotZ);
        m_tReading.YVelocity = velocity * Sin(cRotZ);
    }

    /****************************************/
    /****************************************/

    void CRVRVelocitySensor::Reset()
    {
        m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse();
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRVelocitySensor,
                    "rvr_velocity", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr velocity sensor",
                    "The sensor returns 2 velocities in meters/sec\n"
                    "that represents the speed of the robot in the cartesian system that is defined at boot time.\n"
                    "For usage, refer to [ci_rvr_velocity_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_velocity implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}