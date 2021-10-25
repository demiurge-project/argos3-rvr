/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_velocity_sensor.h>
 *
 */

#ifndef RVR_VELOCITY_SENSOR_H
#define RVR_VELOCITY_SENSOR_H

namespace argos
{
    class CRVRVelocitySensor;
    class CEmbodiedEntity;
    class CWheeledEntity;
}

#include "../control_interface/ci_rvr_velocity_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRVelocitySensor : public CSimulatedSensor,
                               public CCI_RVRVelocitySensor
    {

    public:
        CRVRVelocitySensor();

        virtual ~CRVRVelocitySensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    private:
        /** Reference to wheeled entity */
        CWheeledEntity *m_pcWheeledEntity;

        /** Reference to the embodied entity */
        CEmbodiedEntity *m_pcEmbodiedEntity;
    };

}

#endif