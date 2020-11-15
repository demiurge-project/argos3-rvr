/**
 * @file <argos3/plugins/robots/rvr/simulator/dynamics2d_rvr_model.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "dynamics2d_rvr_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {
    static const Real RVR_HEIGHT = CRVREntity::BODY_HEIGHT;
    static const Real RVR_BODY_LENGTH = CRVREntity::BODY_LENGTH;
    static const Real RVR_WIDTH = CRVREntity::BODY_WIDTH;

    CDynamics2DRVRModel::CDynamics2DRVRModel(CDynamics2DEngine& c_engine,
        CRVREntity& c_entity) :
        CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
        m_cRVREntity(c_entity),
        m_fMass(0.4f) {
        // Create vertices for box shape
        cpVect tVertices[] = {
         cpv(-RVR_WIDTH * 0.5f, -RVR_HEIGHT * 0.5f),
         cpv(-RVR_WIDTH * 0.5f,  RVR_HEIGHT * 0.5f),
         cpv(RVR_WIDTH * 0.5f,  RVR_HEIGHT * 0.5f),
         cpv(RVR_WIDTH * 0.5f, -RVR_HEIGHT * 0.5f)
        };
        // Create body
        m_ptBaseBody =
            cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                cpBodyNew(m_fMass,
                    cpMomentForPoly(m_fMass,
                        4,
                        tVertices,
                        cpvzero)));
        const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
        m_ptBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
        CRadians cXAngle, cYAngle, cZAngle;
        GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
        cpBodySetAngle(m_ptBaseBody, cZAngle.GetValue());
        // Create shape
        m_ptBaseShape =
            cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                cpPolyShapeNew(m_ptBaseBody,
                    4,
                    tVertices,
                    cpvzero));
        m_ptBaseShape->e = 0.0; // elasticity
        m_ptBaseShape->u = 0.7; // friction with the ground

        SetBody(m_ptBaseBody, RVR_HEIGHT);
    }

    CDynamics2DRVRModel::~CDynamics2DRVRModel() {
        // detach steering
    }

    void CDynamics2DRVRModel::Reset() {
        CDynamics2DSingleBodyObjectModel::Reset();
        // reset steering
    }

    void CDynamics2DRVRModel::UpdateFromEntityStatus() {
        // manage with steerings
        // TODO 
    }

    REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CRVREntity, CDynamics2DRVRModel);
}