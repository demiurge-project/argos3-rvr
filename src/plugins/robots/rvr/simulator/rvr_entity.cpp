/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_entity.cpp>
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "rvr_entity.h"

#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {
    const Real CRVREntity::BODY_HEIGHT = 0.114f;
    const Real CRVREntity::BODY_LENGTH = 0.1839f;
    const Real CRVREntity::BODY_WIDTH = 0.2159f;

    /** To be measured */
    const Real CRVREntity::INTERWHEEL_DISTANCE = 0.1159f;
    const Real CRVREntity::HALF_INTERWHEEL_DISTANCE = INTERWHEEL_DISTANCE * 0.5f;
    const Real CRVREntity::WHEEL_RADIUS = 0.057f;
    const Real CRVREntity::LENGTH_WHEEL_DISTANCE = 0.1539f;
    const Real CRVREntity::HALF_LENGTH_WHEEL_DISTANCE = LENGTH_WHEEL_DISTANCE * 0.5f;

    CRVREntity::CRVREntity() :
        CComposableEntity(NULL),
        m_pcControllableEntity(NULL),
        m_pcEmbodiedEntity(NULL),
        m_pcWheeledEntity(NULL) {}

    CRVREntity::CRVREntity(const std::string& str_id,
        const std::string& str_controller_id,
        const CVector3& c_position,
        const CQuaternion& c_orientation) :

        CComposableEntity(NULL, str_id),
        m_pcControllableEntity(NULL),
        m_pcEmbodiedEntity(NULL),
        m_pcWheeledEntity(NULL) {

        try {
            /* Create and init components */
            /** Embodied entity */
            m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
            AddComponent(*m_pcEmbodiedEntity);

            /** Wheeled entity */
            /* We have 4 wheels */
            m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
            AddComponent(*m_pcWheeledEntity);
            /* Set each whel position */
            m_pcWheeledEntity->SetWheel(0, CVector3(0.0f, HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
            m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);

            /** Controllable entity
             * Must be added last for sensors and actuators to link correctly
             */
            m_pcControllableEntity = new CControllableEntity(this, "controller_0");
            AddComponent(*m_pcControllableEntity);
            m_pcControllableEntity->SetController(str_controller_id);

            /** Update components */
            UpdateComponents();

        }
        catch (CARGoSException& ex) {
            LOG << ex.what() << "\n";
            THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
        }
    }

    void CRVREntity::Init(TConfigurationNode& t_tree) {
        try {
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

            /** Controllable entity
             * Must be added last for sensors and actuators to link correctly
             */
            m_pcControllableEntity = new CControllableEntity(this);
            AddComponent(*m_pcControllableEntity);
            m_pcControllableEntity->Init(GetNode(t_tree, "controller"));

            /** Update components */
            UpdateComponents();
        }
        catch (CARGoSException& ex) {
            LOG << ex.what() << '\n';
            THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
        }


    }

    void CRVREntity::Reset() {
        /* Reset all components */
        CComposableEntity::Reset();
        /* Update components */
        UpdateComponents();
    }

    void CRVREntity::Destroy() {
        CComposableEntity::Destroy();
    }

#define UPDATE(COMPONENT) if(COMPONENT->IsEnabled()) COMPONENT->Update();

    void CRVREntity::UpdateComponents() {
        // no components to update for now
    }


    REGISTER_ENTITY(CRVREntity,
        "rvr",
        "Raffaele Todesco [raffaele.todesco@ulb.be]",
        "1.0",
        "The Sphero RVR robot.",
        "Sphero Description",
        "Under development"
    );

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CRVREntity);
}