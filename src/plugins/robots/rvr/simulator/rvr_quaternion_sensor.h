/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_quaternion_sensor.h>
 *
 */

#ifndef RVR_QUATERNION_SENSOR_H
#define RVR_QUATERNION_SENSOR_H

namespace argos
{
    class CRVRQuaternionSensor;
    class CRVRQuaternionEquippedEntity;
}

#include "../control_interface/ci_rvr_quaternion_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRQuaternionSensor : public CSimulatedSensor,
                                 public CCI_RVRQuaternionSensor
    {

    public:
        CRVRQuaternionSensor();

        virtual ~CRVRQuaternionSensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    private:
        /** Reference to quaternion sensor equipped entity associated to this sensor */
        CRVRQuaternionEquippedEntity *m_pcQuaternionEntity;

        /** Reference to the embodied entity */
        CEmbodiedEntity *m_pcEmbodiedEntity;

        /** Random number generator */
        CRandom::CRNG *m_pcRNG;

        /** Whether to add noise or not */
        bool m_bAddNoise;

        /** Noise range */
        CRange<Real> m_cNoiseRange;
    };

}

#endif