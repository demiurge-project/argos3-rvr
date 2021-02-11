/**
 * @file <argos3/plugins/robots/rvr/simulator/dynamics2d_rvr_model.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef DYNAMICS2D_RVR_MODEL_H
#define DYNAMICS2D_RVR_MODEL_H

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include "rvr_entity.h"

namespace argos
{
    class CDynamics2DRVRModel : public CDynamics2DSingleBodyObjectModel
    {

    public:
        CDynamics2DRVRModel(CDynamics2DEngine &c_engine,
                            CRVREntity &c_entity);

        virtual ~CDynamics2DRVRModel();

        virtual void Reset();

        virtual void UpdateFromEntityStatus();

    private:
        CRVREntity &m_cRVREntity;
        CWheeledEntity &m_cWheeledEntity;

        CDynamics2DDifferentialSteeringControl m_cDiffSteering;

        cpFloat m_fMass;
        cpShape *m_ptBaseShape;
        cpBody *m_ptBaseBody;

        const Real *m_fCurrentWheelVelocity;
    };
} // namespace argos

#endif