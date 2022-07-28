/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_proximity_default_sensor.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>

#include "rvr_proximity_default_sensor.h"

namespace argos
{

    /****************************************/
    /****************************************/

    static CRange<Real> UNIT(0.0f, 1.0f);

    /****************************************/
    /****************************************/

    CRVRProximityDefaultSensor::CRVRProximityDefaultSensor() : m_pcEmbodiedEntity(NULL),
                                                               m_bShowRays(false),
                                                               m_pcRNG(NULL),
                                                               m_bAddNoise(false),
                                                               m_cSensorRange(2.0f),
                                                               m_cSpace(CSimulator::GetInstance().GetSpace())
    {
    }

    /****************************************/
    /****************************************/

    void CRVRProximityDefaultSensor::Init(TConfigurationNode &t_tree)
    {
        try
        {
            CCI_RVRProximitySensor::Init(t_tree);
            /* Show rays? */
            GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
            /* Parse noise level */
            Real fNoiseMean = 0.0f;
            Real fNoiseStd = 0.0f;
            GetNodeAttributeOrDefault(t_tree, "noise_mean", fNoiseMean, fNoiseMean);
            GetNodeAttributeOrDefault(t_tree, "noise_std", fNoiseStd, fNoiseStd);
            if (fNoiseStd < 0.0f)
            {
                THROW_ARGOSEXCEPTION("Can't specify a negative value for the deviation"
                                     << " of the rvr proximity sensor");
            }
            else if (fNoiseStd > 0.0f)
            {
                m_bAddNoise = true;
                m_cNoiseMean = fNoiseMean;
                m_cNoiseStd = fNoiseStd;
                m_pcRNG = CRandom::CreateRNG("argos");
            }
            /* Parse sensor range */
            GetNodeAttributeOrDefault(t_tree, "range", m_cSensorRange, m_cSensorRange);
            if (m_cSensorRange > 2.0f)
            {
                THROW_ARGOSEXCEPTION("Can't specify a value greater than 2.0 for the"
                                     << " rvr proximity sensor range");
            }
            else if (m_cSensorRange < 0.0f)
            {
                THROW_ARGOSEXCEPTION("Can't specify a value less than 0.0 for the"
                                     << " rvr proximity sensor range");
            }
            m_tReadings.resize(8); // take only the first 8 sensors which is the sensor ring
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Initialization error in rvr default proximity sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRProximityDefaultSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
            m_pcProximityEntity = &(c_entity.GetComponent<CProximitySensorEquippedEntity>("proximity_sensors"));
            m_pcProximityEntity->Enable();
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr proximity default sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRProximityDefaultSensor::Update()
    {
        /* Ray used for scanning the environment for obstacles */
        CRay3 cScanningRay;
        CVector3 cRayStart, cRayEnd;
        /* Buffers to contain data about the intersection */
        SEmbodiedEntityIntersectionItem sIntersection;
        /* Go through the sensors */
        for (UInt32 i = 0; i < m_tReadings.size(); ++i)
        {
            /* Compute ray for sensor i */
            cRayStart = m_pcProximityEntity->GetSensor(i).Offset;
            cRayStart.Rotate(m_pcEmbodiedEntity->GetOriginAnchor().Orientation);
            cRayStart += m_pcEmbodiedEntity->GetOriginAnchor().Position;
            cRayEnd = m_pcProximityEntity->GetSensor(i).Offset;
            cRayEnd += m_pcProximityEntity->GetSensor(i).Direction;
            cRayEnd.Rotate(m_pcEmbodiedEntity->GetOriginAnchor().Orientation);
            cRayEnd += m_pcEmbodiedEntity->GetOriginAnchor().Position;
            cScanningRay.Set(cRayStart, cRayEnd);
            /* Compute reading */
            /* Get the closest intersection */
            if (GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                         cScanningRay,
                                                         *m_pcEmbodiedEntity))
            {
                /* There is an intersection */
                if (m_bShowRays)
                {
                    m_pcControllableEntity->AddIntersectionPoint(cScanningRay,
                                                                 sIntersection.TOnRay);
                    m_pcControllableEntity->AddCheckedRay(true, cScanningRay);
                }
                m_tReadings[i].Value = CalculateReading(cScanningRay.GetDistance(sIntersection.TOnRay));
            }
            else
            {
                /* No intersection */
                m_tReadings[i].Value = 0.0f;
                if (m_bShowRays)
                {
                    m_pcControllableEntity->AddCheckedRay(false, cScanningRay);
                }
            }
            /* Apply noise to the sensor */
            if (m_bAddNoise)
            {
                m_tReadings[i].Value += m_pcRNG->Gaussian(m_cNoiseStd, m_cNoiseMean);
            }
            /* Divide the value by the sensor range to scale it between 0 and 1 */
            m_tReadings[i].Value /= m_cSensorRange;
        }
    }

    /****************************************/
    /****************************************/

    void CRVRProximityDefaultSensor::Reset()
    {
        for (UInt32 i = 0; i < GetReadings().size(); ++i)
        {
            m_tReadings[i].Value = 0.0f;
        }
    }

    /****************************************/
    /****************************************/

    Real CRVRProximityDefaultSensor::CalculateReading(Real f_distance)
    {
        Real value = 0.0f;
        if (0.05 <= f_distance && f_distance <= m_cSensorRange) // default range of terabee : 0.05 - 2 meters
        {
            value = f_distance;
        }
        else
        {
            // in order to get a value between 0 and 1, we need to divide the distance by the sensor range
            // and not use -inf and inf
            if (f_distance <= 0.05)
            {
                value = 0.0f;
            }
            else
            {
                value = m_cSensorRange;
            }
        }
        return value;
    }

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRProximityDefaultSensor,
                    "rvr_proximity", "default",
                    "Raffaele Todesco [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr proximity sensor",
                    "This sensor accesses a set of proximity sensors. The sensors all return a value\n"
                    "between 0 and 1, where 1 means nothing within range and 0 means an external\n"
                    "object is touching the sensor. Values between 0.05 and the sensor range 2.0 are the distance between\n"
                    "the occluding object and the sensor.\n"
                    "For usage, refer to [ci_rvr_proximity_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_proximity implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n"
                    "OPTIONAL XML CONFIGURATION\n\n"
                    "It is possible to draw the rays shot by the proximity sensor in the OpenGL\n"
                    "visualization. This can be useful for sensor debugging but also to understand\n"
                    "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                    "they are not obstructed and in purple when they are. In case a ray is\n"
                    "obstructed, a black dot is drawn where the intersection occurred.\n"
                    "To turn this functionality on, add the attribute \"show_rays\".\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_proximity implementation=\"default\" show_rays=\"true\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n"
                    "It is possible to add Gaussian noise to the sensors, thus matching the\n"
                    "characteristics of a real robot better. This can be done with the attribute\n"
                    "\"noise_mean\" and \"noise_std\", and is added to the calculated\n"
                    "reading.\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_proximity implementation=\"default\" noise_mean=\"0\" noise_std=\"0.2\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n"
                    "It is possible to specify the sensor range. This can be done with the attribute\n"
                    "\"range\". However, the potentially shown rays will be displayed with the maximum range of the sensor.\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_proximity implementation=\"default\" range=\"1.5\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n"
                    "The range can not exceed the maximum range of the sensor, 2.0.\n\n",
                    "Usable");
}
