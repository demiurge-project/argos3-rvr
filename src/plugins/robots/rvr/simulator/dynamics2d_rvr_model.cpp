/**
 * @file <argos3/plugins/robots/rvr/simulator/dynamics2d_rvr_model.cpp>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "dynamics2d_rvr_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos
{
    static const Real RVR_HEIGHT = CRVREntity::BODY_HEIGHT;
    static const Real RVR_BODY_LENGTH = CRVREntity::BODY_LENGTH;
    static const Real RVR_WIDTH = CRVREntity::BODY_WIDTH;

    static const Real RVR_INTERWHEEL_DISTANCE = CRVREntity::INTERWHEEL_DISTANCE;

    static const Real RVR_MAX_FORCE = 1.5f;
    static const Real RVR_MAX_TORQUE = 1.5f;

    /**
     * Enum facilitating wheels access.
     * As the 4 wheels of the RVR are linked in pairs with the treads,
     * we treat it as 2 wheels
     */
    enum RVR_WHEELS
    {
        RVR_LEFT_WHEEL = 0,
        RVR_RIGHT_WHEEL = 1
    };

    CDynamics2DRVRModel::CDynamics2DRVRModel(CDynamics2DEngine &c_engine,
                                             CRVREntity &c_entity) : CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
                                                                     m_cRVREntity(c_entity),
                                                                     m_cWheeledEntity(m_cRVREntity.GetWheeledEntity()),
                                                                     m_cDiffSteering(c_engine,
                                                                                     RVR_MAX_FORCE,
                                                                                     RVR_MAX_TORQUE,
                                                                                     RVR_INTERWHEEL_DISTANCE),
                                                                     m_fMass(2.0f),
                                                                     m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities())
    { // 1kg mass multiplied by 2
        // Create vertices for box shape
        cpVect tVertices[] = {
            cpv(-RVR_WIDTH * 0.5f, -RVR_BODY_LENGTH * 0.5f),
            cpv(-RVR_WIDTH * 0.5f, RVR_BODY_LENGTH * 0.5f),
            cpv(RVR_WIDTH * 0.5f, RVR_BODY_LENGTH * 0.5f),
            cpv(RVR_WIDTH * 0.5f, -RVR_BODY_LENGTH * 0.5f)};
        // Create body
        m_ptBaseBody =
            cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                           cpBodyNew(m_fMass,
                                     cpMomentForPoly(m_fMass,
                                                     4,
                                                     tVertices,
                                                     cpvzero)));
        const CVector3 &cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
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

        m_cDiffSteering.AttachTo(m_ptBaseBody);

        SetBody(m_ptBaseBody, RVR_HEIGHT);
    }

    CDynamics2DRVRModel::~CDynamics2DRVRModel()
    {
        m_cDiffSteering.Detach();
    }

    void CDynamics2DRVRModel::Reset()
    {
        CDynamics2DSingleBodyObjectModel::Reset();
        m_cDiffSteering.Reset();
    }

    void CDynamics2DRVRModel::UpdateFromEntityStatus()
    {
        if ((m_fCurrentWheelVelocity[RVR_LEFT_WHEEL] != 0.0f) ||
            (m_fCurrentWheelVelocity[RVR_RIGHT_WHEEL] != 0.0f))
        {
            m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[RVR_LEFT_WHEEL],
                                             m_fCurrentWheelVelocity[RVR_RIGHT_WHEEL]);
        }
        else
        {
            /* No, we don't want to move - zero all speeds */
            m_cDiffSteering.Reset();
        }
    }

    REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CRVREntity, CDynamics2DRVRModel);
}