/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_entity.cpp>
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "rvr_entity.h"

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include "./rvr_quaternion_equipped_entity.h"
#include "./rvr_quaternion_sensor.h"
#include "./rvr_imu_equipped_entity.h"
#include "./rvr_imu_sensor.h"
#include <argos3/core/utility/math/general.h>

namespace argos
{
    const Real CRVREntity::BODY_HEIGHT = 0.114f;
    const Real CRVREntity::BODY_LENGTH = 0.1839f;
    const Real CRVREntity::BODY_WIDTH = 0.2159f;

    /** To be measured */
    const Real CRVREntity::INTERWHEEL_DISTANCE = 0.147f;
    const Real CRVREntity::HALF_INTERWHEEL_DISTANCE = INTERWHEEL_DISTANCE * 0.5f;
    const Real CRVREntity::WHEEL_RADIUS = 0.057f;
    const Real CRVREntity::LENGTH_WHEEL_DISTANCE = 0.1539f;
    const Real CRVREntity::HALF_LENGTH_WHEEL_DISTANCE = LENGTH_WHEEL_DISTANCE * 0.5f;

    /** LEDs */
    const Real CRVREntity::LEDS_ELEVATION = BODY_HEIGHT * 0.3f;

    /** Proximity sensors */
    const Real CRVREntity::PROXIMITY_SENSOR_RING_ELEVATION = 0.032f;
    const Real CRVREntity::PROXIMITY_SENSOR_RING_RADIUS = 0.286f / 2;
    const CRadians CRVREntity::PROXIMITY_SENSOR_RING_START_ANGLE = CRadians((ARGOS_PI / 8.0f) * 0.5f);
    const Real CRVREntity::PROXIMITY_SENSOR_RING_RANGE = 2.0f;
    const CRadians CRVREntity::PROXIMITY_SENSOR_ANGLES[8] = {
        CRadians::PI / 6.0f,
        CRadians::PI / 3.0f,
        CRadians::PI_OVER_TWO,
        CRadians::PI * 0.75f,
        CRadians::PI * 1.25f,
        CRadians::PI * 1.5f,
        CRadians::PI * 1.667f,
        CRadians::PI * 1.833f};

    /** Lidar */
    const Real CRVREntity::LIDAR_ELEVATION = 0.04f;
    const Real CRVREntity::LIDAR_RADIUS = 0.035f;
    const CRadians CRVREntity::LIDAR_START_ANGLE = CRadians((ARGOS_PI / 8.0f) * 0.5f);
    const Real CRVREntity::LIDAR_RANGE = 10.0f;

    /* Ground sensor */
    const Real CRVREntity::GROUND_SENSOR_OFFSET = 0.05f;

    const Real CRVREntity::LEDS_POSITIONS[5][2] = {
        {BODY_LENGTH * 0.4f, BODY_WIDTH * 0.4f * 0.2f},    // front right
        {BODY_LENGTH * 0.4f, -(BODY_WIDTH * 0.4f * 0.2f)}, // front left
        {0.0f, BODY_WIDTH * 0.4f},                         // right
        {0.0f, -BODY_WIDTH * 0.4f},                        // left
        {-BODY_LENGTH * 0.4f, 0.0f}                        // back
    };

    const Real CRVREntity::LEDS_HEIGHT = 0.01f;
    const Real CRVREntity::LED_INNER_RADIUS = 0.01f * 0.8f;
    const Real CRVREntity::LED_OUTER_RADIUS = 0.01f;

    CRVREntity::CRVREntity() : CComposableEntity(NULL),
                               m_pcControllableEntity(NULL),
                               m_pcEmbodiedEntity(NULL),
                               m_pcGroundSensorEquippedEntity(NULL),
                               m_pcLEDEquippedEntity(NULL),
                               m_pcProximitySensorEquippedEntity(NULL),
                               m_pcQuaternionEquippedEntity(NULL),
                               m_pcLightSensorEquippedEntity(NULL),
                               m_pcWheeledEntity(NULL) {}

    CRVREntity::CRVREntity(const std::string &str_id,
                           const std::string &str_controller_id,
                           const CVector3 &c_position,
                           const CQuaternion &c_orientation) :

                                                               CComposableEntity(NULL, str_id),
                                                               m_pcControllableEntity(NULL),
                                                               m_pcEmbodiedEntity(NULL),
                                                               m_pcGroundSensorEquippedEntity(NULL),
                                                               m_pcLEDEquippedEntity(NULL),
                                                               m_pcProximitySensorEquippedEntity(NULL),
                                                               m_pcQuaternionEquippedEntity(NULL),
                                                               m_pcLightSensorEquippedEntity(NULL),
                                                               m_pcWheeledEntity(NULL)
    {

        try
        {
            /* Create and init components */
            /** Embodied entity */
            m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
            AddComponent(*m_pcEmbodiedEntity);

            /** Wheeled entity */
            /* We have 4 wheels but 2 axis*/
            m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
            AddComponent(*m_pcWheeledEntity);
            /* Set each whel position */
            m_pcWheeledEntity->SetWheel(0, CVector3(0.0f, HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
            m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);

            /** LED entity */
            /* We have 5 LEDs :
            - 2 front (left and right)
            - 1 back
            - 1 for each side of the robot */
            m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
            AddComponent(*m_pcLEDEquippedEntity);
            for (int i = 0; i < 5; ++i)
            {
                m_pcLEDEquippedEntity->AddLED(CVector3(LEDS_POSITIONS[i][0], LEDS_POSITIONS[i][1], LEDS_ELEVATION), m_pcEmbodiedEntity->GetOriginAnchor());
            }

            m_pcLEDEquippedEntity->Disable();

            /* Proximity sensor equipped entity */
            m_pcProximitySensorEquippedEntity =
                new CProximitySensorEquippedEntity(this,
                                                   "proximity_0");
            AddComponent(*m_pcProximitySensorEquippedEntity);
            // add each proximity sensor
            CRadians cAngle;
            CVector3 cOff, cDir;
            CVector3 c_center = CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION);
            for (int i = 0; i < 8; ++i)
            {
                cAngle = PROXIMITY_SENSOR_ANGLES[i];
                // cAngle = CRadians(0.0f);
                cAngle.SignedNormalize();
                cOff.Set(PROXIMITY_SENSOR_RING_RADIUS, 0.0f, 0.0f);
                cOff.RotateZ(cAngle);
                cOff += c_center;
                cDir.Set(PROXIMITY_SENSOR_RING_RANGE, 0.0f, 0.0f);
                cDir.RotateZ(cAngle);
                m_pcProximitySensorEquippedEntity->AddSensor(cOff, cDir, PROXIMITY_SENSOR_RING_RANGE, m_pcEmbodiedEntity->GetOriginAnchor());
            }
            // m_pcProximitySensorEquippedEntity->AddSensorRing(
            //     CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            //     PROXIMITY_SENSOR_RING_RADIUS,
            //     PROXIMITY_SENSOR_RING_START_ANGLE,
            //     PROXIMITY_SENSOR_RING_RANGE,
            //     8,
            //     m_pcEmbodiedEntity->GetOriginAnchor());
            m_pcProximitySensorEquippedEntity->AddSensorRing(CVector3(0.0f, 0.0f, LIDAR_ELEVATION),
                                                             LIDAR_RADIUS,
                                                             LIDAR_START_ANGLE,
                                                             LIDAR_RANGE,
                                                             719,
                                                             m_pcEmbodiedEntity->GetOriginAnchor());

            /* Ground color sensor entity */
            m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,
                                                                             "ground_0");
            AddComponent(*m_pcGroundSensorEquippedEntity);
            // TODO : update real position
            m_pcGroundSensorEquippedEntity->AddSensor(CVector2(GROUND_SENSOR_OFFSET, 0.0f),
                                                      CGroundSensorEquippedEntity::TYPE_GRAYSCALE, m_pcEmbodiedEntity->GetOriginAnchor());

            m_pcQuaternionEquippedEntity = new CRVRQuaternionEquippedEntity(this, "rvr_quaternion_0");
            AddComponent(*m_pcQuaternionEquippedEntity);

            /* Light sensor equipped entity */
            m_pcLightSensorEquippedEntity =
                new CLightSensorEquippedEntity(this,
                                               "light_0");
            AddComponent(*m_pcLightSensorEquippedEntity);
            m_pcLightSensorEquippedEntity->AddSensorRing(CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                                                         0.05f,
                                                         CRadians::PI / 0.5456,
                                                         2.5f,
                                                         1,
                                                         m_pcEmbodiedEntity->GetOriginAnchor());
            /** Controllable entity
             * Must be added last for sensors and actuators to link correctly
             */
            m_pcControllableEntity = new CControllableEntity(this, "controller_0");
            AddComponent(*m_pcControllableEntity);
            m_pcControllableEntity->SetController(str_controller_id);

            /** Update components */
            UpdateComponents();
        }
        catch (CARGoSException &ex)
        {
            LOG << ex.what() << "\n";
            THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
        }
    }

    void CRVREntity::Init(TConfigurationNode &t_tree)
    {
        try
        {
            CComposableEntity::Init(t_tree);
            /** Embodied entity */
            m_pcEmbodiedEntity = new CEmbodiedEntity(this);
            AddComponent(*m_pcEmbodiedEntity);
            m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));

            /** Wheeled entity */
            m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
            AddComponent(*m_pcWheeledEntity);
            /* Set each whel position */
            m_pcWheeledEntity->SetWheel(0, CVector3(0.0f, HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
            m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);

            /** LED entity */
            /* We have 5 LEDs :
            - 2 front (left and right)
            - 1 back
            - 1 for each side of the robot */
            m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
            AddComponent(*m_pcLEDEquippedEntity);
            for (int i = 0; i < 5; ++i)
            {
                m_pcLEDEquippedEntity->AddLED(CVector3(LEDS_POSITIONS[i][0], LEDS_POSITIONS[i][1], LEDS_ELEVATION), m_pcEmbodiedEntity->GetOriginAnchor());
            }

            m_pcLEDEquippedEntity->Disable();

            /* Proximity sensor equipped entity */
            m_pcProximitySensorEquippedEntity =
                new CProximitySensorEquippedEntity(this,
                                                   "proximity_0");
            AddComponent(*m_pcProximitySensorEquippedEntity);
            // add each proximity sensor
            CRadians cAngle;
            CVector3 cOff, cDir;
            CVector3 c_center = CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION);
            for (int i = 0; i < 8; ++i)
            {
                cAngle = PROXIMITY_SENSOR_ANGLES[i];
                // cAngle = CRadians(0.0f);
                cAngle.SignedNormalize();
                cOff.Set(PROXIMITY_SENSOR_RING_RADIUS, 0.0f, 0.0f);
                cOff.RotateZ(cAngle);
                cOff += c_center;
                cDir.Set(PROXIMITY_SENSOR_RING_RANGE, 0.0f, 0.0f);
                cDir.RotateZ(cAngle);
                m_pcProximitySensorEquippedEntity->AddSensor(cOff, cDir, PROXIMITY_SENSOR_RING_RANGE, m_pcEmbodiedEntity->GetOriginAnchor());
            }
            // m_pcProximitySensorEquippedEntity->AddSensorRing(
            //     CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            //     PROXIMITY_SENSOR_RING_RADIUS,
            //     PROXIMITY_SENSOR_RING_START_ANGLE,
            //     PROXIMITY_SENSOR_RING_RANGE,
            //     8,
            //     m_pcEmbodiedEntity->GetOriginAnchor());

            m_pcProximitySensorEquippedEntity->AddSensorRing(CVector3(0.0f, 0.0f, LIDAR_ELEVATION),
                                                             LIDAR_RADIUS,
                                                             LIDAR_START_ANGLE,
                                                             LIDAR_RANGE,
                                                             719,
                                                             m_pcEmbodiedEntity->GetOriginAnchor());
            /* Ground color sensor entity */
            m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,
                                                                             "ground_0");
            AddComponent(*m_pcGroundSensorEquippedEntity);
            // TODO : update real position
            m_pcGroundSensorEquippedEntity->AddSensor(CVector2(GROUND_SENSOR_OFFSET, 0.0f),
                                                      CGroundSensorEquippedEntity::TYPE_GRAYSCALE, m_pcEmbodiedEntity->GetOriginAnchor());

            m_pcQuaternionEquippedEntity = new CRVRQuaternionEquippedEntity(this, "rvr_quaternion_0");
            AddComponent(*m_pcQuaternionEquippedEntity);

            /* Light sensor equipped entity */
            m_pcLightSensorEquippedEntity =
                new CLightSensorEquippedEntity(this,
                                               "light_0");
            AddComponent(*m_pcLightSensorEquippedEntity);
            m_pcLightSensorEquippedEntity->AddSensorRing(CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                                                         0.5f,
                                                         CRadians::PI / 0.5456,
                                                         2.5f,
                                                         1,
                                                         m_pcEmbodiedEntity->GetOriginAnchor());
            /** Controllable entity
             * Must be added last for sensors and actuators to link correctly
             */
            m_pcControllableEntity = new CControllableEntity(this);
            AddComponent(*m_pcControllableEntity);
            m_pcControllableEntity->Init(GetNode(t_tree, "controller"));

            /** Update components */
            UpdateComponents();
        }
        catch (CARGoSException &ex)
        {
            LOG << ex.what() << '\n';
            THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
        }
    }

    void CRVREntity::Reset()
    {
        /* Reset all components */
        CComposableEntity::Reset();
        /* Update components */
        UpdateComponents();
    }

    void CRVREntity::Destroy()
    {
        CComposableEntity::Destroy();
    }

#define UPDATE(COMPONENT)       \
    if (COMPONENT->IsEnabled()) \
        COMPONENT->Update();

    void CRVREntity::UpdateComponents()
    {
        UPDATE(m_pcLEDEquippedEntity);
    }

    REGISTER_ENTITY(CRVREntity,
                    "rvr",
                    "Raffaele Todesco [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The Sphero RVR robot.",
                    "Sphero Description",
                    "Under development");

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CRVREntity);
} // namespace argos