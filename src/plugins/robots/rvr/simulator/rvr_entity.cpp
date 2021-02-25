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

#include <argos3/core/utility/math/general.h>

namespace argos
{
    const Real CRVREntity::BODY_HEIGHT = 0.114f;
    const Real CRVREntity::BODY_LENGTH = 0.1839f;
    const Real CRVREntity::BODY_WIDTH = 0.2159f;

    /** To be measured */
    const Real CRVREntity::INTERWHEEL_DISTANCE = 0.1159f;
    const Real CRVREntity::HALF_INTERWHEEL_DISTANCE = INTERWHEEL_DISTANCE * 0.5f;
    const Real CRVREntity::WHEEL_RADIUS = 0.057f;
    const Real CRVREntity::LENGTH_WHEEL_DISTANCE = 0.1539f;
    const Real CRVREntity::HALF_LENGTH_WHEEL_DISTANCE = LENGTH_WHEEL_DISTANCE * 0.5f;

    /** LEDs */
    const Real CRVREntity::LEDS_ELEVATION = BODY_HEIGHT * 0.3f;

    const Real CRVREntity::LEDS_POSITIONS[5][2] = {
        {BODY_LENGTH * 0.5f, BODY_WIDTH * 0.5f * 0.2f},    // front right
        {BODY_LENGTH * 0.5f, -(BODY_WIDTH * 0.5f * 0.2f)}, // front left
        {0.0f, BODY_WIDTH * 0.5f},                         // right
        {0.0f, -BODY_WIDTH * 0.5f},                        // left
        {-BODY_LENGTH * 0.5f, 0.0f}                        // back
    };

    const Real CRVREntity::LEDS_HEIGHT = 0.01f;
    const Real CRVREntity::LED_INNER_RADIUS = 0.01f * 0.8f;
    const Real CRVREntity::LED_OUTER_RADIUS = 0.01f;

    CRVREntity::CRVREntity() : CComposableEntity(NULL),
                               m_pcControllableEntity(NULL),
                               m_pcEmbodiedEntity(NULL),
                               m_pcGroundSensorEquippedEntity(NULL),
                               m_pcLEDEquippedEntity(NULL),
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

            /* Ground color sensor entity */
            m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,
                                                                             "ground_0");
            AddComponent(*m_pcGroundSensorEquippedEntity);
            // TODO : update real position
            m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.1f, 0.0f),
                                                      CGroundSensorEquippedEntity::TYPE_GRAYSCALE, m_pcEmbodiedEntity->GetOriginAnchor());
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

            /* Ground color sensor entity */
            m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,
                                                                             "ground_0");
            AddComponent(*m_pcGroundSensorEquippedEntity);
            // TODO : update real position
            m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.1f, 0.0f),
                                                      CGroundSensorEquippedEntity::TYPE_GRAYSCALE, m_pcEmbodiedEntity->GetOriginAnchor());

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