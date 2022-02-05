/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_locator_sensor.h>
 *
 */

#ifndef RVR_LOCATOR_SENSOR_H
#define RVR_LOCATOR_SENSOR_H

namespace argos
{
    class CRVRLocatorSensor;
}

#include "../control_interface/ci_rvr_locator_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRLocatorSensor : public CSimulatedSensor,
                              public CCI_RVRLocatorSensor
    {

    public:
        CRVRLocatorSensor();

        virtual ~CRVRLocatorSensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    private:
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