/**
 * @file <argos3/plugins/robots/rvr/simulator/epuck_ground_default_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>

#include "rvr_ground_color_sensor.h"

namespace argos
{

    CRVRGroundColorSensor::CRVRGroundColorSensor() : m_pcEmbodiedEntity(NULL),
                                                     m_pcFloorEntity(NULL),
                                                     m_pcGroundSensorEntity(NULL),
                                                     m_cSpace(CSimulator::GetInstance().GetSpace()) {}

    void CRVRGroundColorSensor::SetRobot(CComposableEntity &c_entity)
    {
        m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
        m_pcGroundSensorEntity = &(c_entity.GetComponent<CGroundSensorEquippedEntity>("ground_sensors"));
        m_pcGroundSensorEntity->Enable();
        m_pcFloorEntity = &m_cSpace.GetFloorEntity();
    }

    void CRVRGroundColorSensor::Init(TConfigurationNode &t_tree)
    {
        try
        {
            CCI_RVRGroundColorSensor::Init(t_tree);
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Initialization error in ground color sensor", ex);
        }
    }

    void CRVRGroundColorSensor::Update()
    {
        /* Get Robot position and orientation */
        const CVector3 &cEntityPos = m_pcEmbodiedEntity->GetOriginAnchor().Position;
        const CQuaternion &cEntityRot = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
        CRadians cRotZ, cRotY, cRotX;
        cEntityRot.ToEulerAngles(cRotZ, cRotY, cRotX);
        /* Set robot center */
        CVector2 cCenterPos(cEntityPos.GetX(), cEntityPos.GetY());
        /* Position of sensor on the ground after rototranslation */
        CVector2 cSensorPos;

        cSensorPos = m_pcGroundSensorEntity->GetSensor(0).Offset;
        cSensorPos.Rotate(cRotZ);
        cSensorPos += cCenterPos;
        const CColor &cColor = m_pcFloorEntity->GetColorAtPoint(cSensorPos.GetX(),
                                                                cSensorPos.GetY());
        m_sColor = cColor;
    }

    void CRVRGroundColorSensor::Reset()
    {
        // By default, sensor sees black
        m_sColor = CColor::BLACK;
    }

    REGISTER_SENSOR(CRVRGroundColorSensor,
                    "rvr_ground", "rot_z_only",
                    "Raffaele Todesco [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr ground color sensor",
                    "This detects the color of the ground under the RVR."
                    "For usage, refer to [ci_rvr_ground_color_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_ground implementation=\"rot_z_only\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");

} // namespace argos