/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_light_default_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "rvr_light_sensor.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/light_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>

namespace argos
{

    /****************************************/
    /****************************************/

    static CRange<Real> SENSOR_RANGE(0.0f, 120000.0f);

    /****************************************/
    /****************************************/

    static Real ComputeReading(Real f_distance)
    {
        if (f_distance > 2.5f)
        {
            return 0.0f;
        }
        else
        {
            // determined empirically
            return 138.9171f + ::exp(-f_distance * 9.145603f);
        }
    }

    static Real ScaleReading(const CRadians &c_angular_distance)
    {
        if (c_angular_distance > CRadians::PI_OVER_TWO)
        {
            return 0.0f;
        }
        else
        {
            return (1.0f - 2.0f * c_angular_distance / CRadians::PI);
        }
    }

    /****************************************/
    /****************************************/

    CRVRLightDefaultSensor::CRVRLightDefaultSensor() : m_pcEmbodiedEntity(NULL),
                                                       m_bShowRays(false),
                                                       m_pcRNG(NULL),
                                                       m_bAddNoise(false),
                                                       m_cSpace(CSimulator::GetInstance().GetSpace()) {}

    /****************************************/
    /****************************************/

    void CRVRLightDefaultSensor::SetRobot(CComposableEntity &c_entity)
    {
        try
        {
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
            m_pcLightEntity = &(c_entity.GetComponent<CLightSensorEquippedEntity>("light_sensors"));
            m_pcLightEntity->Enable();
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr light default sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRLightDefaultSensor::Init(TConfigurationNode &t_tree)
    {
        try
        {
            /* Show rays? */
            GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
            /* Parse noise level */
            Real fNoiseLevel = 0.0f;
            GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
            if (fNoiseLevel < 0.0f)
            {
                THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                     << " of the rvr light sensor");
            }
            else if (fNoiseLevel > 0.0f)
            {
                m_bAddNoise = true;
                m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
                m_pcRNG = CRandom::CreateRNG("argos");
            }
        }
        catch (CARGoSException &ex)
        {
            THROW_ARGOSEXCEPTION_NESTED("Initialization error in rvr default light sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRLightDefaultSensor::Update()
    {
        /* Erase reading */
        m_tReading.Value = 0.0f;
        /* Get rvr orientation */
        CRadians cTmp1, cTmp2, cOrientationZ;
        m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
        /* Ray used for scanning the environment for obstacles */
        CRay3 cOcclusionCheckRay;
        cOcclusionCheckRay.SetStart(m_pcEmbodiedEntity->GetOriginAnchor().Position);
        CVector3 cRobotToLight;
        /* Buffer for the angle of the light wrt to the rvr */
        CRadians cAngleLightWrtRVR;
        /* Buffers to contain data about the intersection */
        SEmbodiedEntityIntersectionItem sIntersection;
        /* List of light entities */
        CSpace::TMapPerType &mapLights = m_cSpace.GetEntitiesByType("light");
        /* Buffer for angle between a sensor and light */
        CRadians cAngularDistanceFromOptimalLightReceptionPoint;
        /*
       * 1. go through the list of light entities in the scene
       * 2. check if a light is occluded
       * 3. if it isn't, distribute the reading across the sensor
       *    NOTE: the readings are additive
       * 4. clamp sensor value
       */
        for (CSpace::TMapPerType::iterator it = mapLights.begin();
             it != mapLights.end();
             ++it)
        {
            /* Get a reference to the light */
            CLightEntity &cLight = *(any_cast<CLightEntity *>(it->second));
            /* Consider the light only if it has non zero intensity */
            if (cLight.GetIntensity() > 0.0f)
            {
                /* Set the ray end */
                cOcclusionCheckRay.SetEnd(cLight.GetPosition());
                /* Check occlusion between the rvr and the light */
                if (!GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                              cOcclusionCheckRay,
                                                              *m_pcEmbodiedEntity))
                {
                    /* The light is not occluded */
                    if (m_bShowRays)
                    {
                        m_pcControllableEntity->AddCheckedRay(false, cOcclusionCheckRay);
                    }
                    /* Get the distance between the light and the rvr */
                    cOcclusionCheckRay.ToVector(cRobotToLight);

                    /*
                * Linearly scale the distance with the light intensity
                * The greater the intensity, the smaller the distance
                */
                    cRobotToLight /= cLight.GetIntensity();
                    /* Get the angle wrt to rvr rotation */
                    cAngleLightWrtRVR = cRobotToLight.GetZAngle();
                    cAngleLightWrtRVR -= cOrientationZ;
                    /* Set the actual readings */
                    Real fReading = cRobotToLight.Length();
                    /*
                * we process sensors that are within 180 degrees of intersection
                * of light ray with robot body
                */
                    cAngularDistanceFromOptimalLightReceptionPoint = Abs((cAngleLightWrtRVR).SignedNormalize());
                    /* Verify that the angle between the sensor and light is under PI_OVER_TWO */
                    //if (cAngularDistanceFromOptimalLightReceptionPoint < CRadians::PI_OVER_TWO)
                    /*
                    * ComputeReading gives value as if sensor was perfectly in line with
                    * light ray. We then linearly decrease actual reading from 1 (dist
                    * 0) to 0 (dist PI/2)
                    */
                    m_tReading.Value += ComputeReading(fReading); // * ScaleReading(cAngularDistanceFromOptimalLightReceptionPoint);
                }
                else
                {
                    /* The ray is occluded */
                    if (m_bShowRays)
                    {
                        m_pcControllableEntity->AddCheckedRay(true, cOcclusionCheckRay);
                        m_pcControllableEntity->AddIntersectionPoint(cOcclusionCheckRay, sIntersection.TOnRay);
                    }
                }
            }
        }
        /* Apply noise to the sensors */
        if (m_bAddNoise)
        {
            m_tReading.Value += m_pcRNG->Uniform(m_cNoiseRange);
        }
        /* Trunc the reading between 0 and 120 000 */
        SENSOR_RANGE.TruncValue(m_tReading.Value);
    }

    /****************************************/
    /****************************************/

    void CRVRLightDefaultSensor::Reset()
    {
        m_tReading.Value = 0.0f;
    }

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRLightDefaultSensor,
                    "rvr_light", "default",
                    "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                    "1.0",
                    "The rvr light sensor",
                    "This sensor accesses a set of light sensors. The sensors all return a value\n"
                    "between 0 and 1, where 0 means nothing within range and 1 means the perceived\n"
                    "light saturates the sensor. Values between 0 and 1 depend on the distance of\n"
                    "the perceived light. Each reading R is calculated with R=(I/x)^2, where x is the\n"
                    "distance between a sensor and the light, and I is the reference intensity of the\n"
                    "perceived light. The reference intensity corresponds to the minimum distance at\n"
                    "which the light saturates a sensor. The reference intensity depends on the\n"
                    "individual light, and it is set with the \"intensity\" attribute of the light\n"
                    "entity. In case multiple lights are present in the environment, each sensor\n"
                    "reading is calculated as the sum of the individual readings due to each light.\n"
                    "In other words, light wave interference is not taken into account.\n"
                    "For usage, refer to [ci_epuck_light_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <epuck_light implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n"
                    "OPTIONAL XML CONFIGURATION\n\n"
                    "It is possible to draw the rays shot by the light sensor in the OpenGL\n"
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
                    "            <epuck_light implementation=\"default\" show_rays=\"true\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n"
                    "It is possible to add uniform noise to the sensors, thus matching the\n"
                    "characteristics of a real robot better. This can be done with the attribute\n"
                    "\"noise_level\", whose allowed range is in [-1,1] and is added to the calculated\n"
                    "reading. The final sensor reading is always normalized in the [0-1] range.\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <epuck_light implementation=\"default\" noise_level=\"0.1\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>",
                    "Usable");
}