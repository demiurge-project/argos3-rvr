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
                                                     m_pcRNG(NULL),
                                                     m_bAddNoise(false),
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
            // noise handling
            Real fNoiseMean = 0.0f;
            Real fNoiseStd = 0.0f;
            GetNodeAttributeOrDefault(t_tree, "noise_mean", fNoiseMean, fNoiseMean);
            GetNodeAttributeOrDefault(t_tree, "noise_std", fNoiseStd, fNoiseStd);
            if (fNoiseStd < 0.0f)
            {
                THROW_ARGOSEXCEPTION("Can't specify a negative value for the deviation"
                                     << " of the rvr color sensor");
            }
            else if (fNoiseStd > 0.0f)
            {
                m_bAddNoise = true;
                m_cNoiseMean = fNoiseMean;
                m_cNoiseStd = fNoiseStd;
                m_pcRNG = CRandom::CreateRNG("argos");
            }
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
        CColor cColor = m_pcFloorEntity->GetColorAtPoint(cSensorPos.GetX(),
                                                         cSensorPos.GetY());
        if (m_bAddNoise)
        {
            int red = (int)cColor.GetRed() + Round(m_pcRNG->Gaussian(m_cNoiseStd, m_cNoiseMean));
            if (red > 255)
                red = 255;
            if (red < 0)
                red = 0;
            cColor.SetRed(red);
            int green = (int)cColor.GetGreen() + Round(m_pcRNG->Gaussian(m_cNoiseStd, m_cNoiseMean));
            if (green > 255)
                green = 255;
            if (green < 0)
                green = 0;
            cColor.SetGreen(green);
            int blue = (int)cColor.GetBlue() + Round(m_pcRNG->Gaussian(m_cNoiseStd, m_cNoiseMean));
            if (blue > 255)
                blue = 255;
            if (blue < 0)
                blue = 0;
            cColor.SetBlue(blue);
        }
        m_sReading.Color = cColor;
    }

    void CRVRGroundColorSensor::Reset()
    {
        // By default, sensor sees white
        m_sReading.Color = CColor::WHITE;
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