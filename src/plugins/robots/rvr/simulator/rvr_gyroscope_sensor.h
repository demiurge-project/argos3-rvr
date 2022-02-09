/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_gyroscope_sensor.h>
 *
 */

#ifndef RVR_GYROSCOPE_SENSOR_H
#define RVR_GYROSCOPE_SENSOR_H

namespace argos {
    class CRVRGyroscopeSensor;

    class CEmbodiedEntity;

    class CWheeledEntity;
}

#include "../control_interface/ci_rvr_gyroscope_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

    class CRVRGyroscopeSensor : public CSimulatedSensor,
                                public CCI_RVRGyroscopeSensor {

    public:
        CRVRGyroscopeSensor();

        virtual ~CRVRGyroscopeSensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);

        virtual void Init(TConfigurationNode &t_tree);

        virtual void Update();

        virtual void Reset();

    private:
        /** Reference to wheeled entity */
        CWheeledEntity *m_pcWheeledEntity{};

        /** Reference to the embodied entity */
        CEmbodiedEntity *m_pcEmbodiedEntity{};

        /** Whether to add noise or not */
        bool m_bAddNoise;

        /** Random number generator */
        CRandom::CRNG *m_pcRNG;

        /** Noise range */
        CRange<Real> m_cNoiseRange;
    };

}

#endif