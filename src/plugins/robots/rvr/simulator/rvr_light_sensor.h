/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_light_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef RVR_LIGHT_DEFAULT_SENSOR_H
#define RVR_LIGHT_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos
{
    class CRVRLightDefaultSensor;
    class CLightSensorEquippedEntity;
}

#include "../control_interface/ci_rvr_light_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRLightDefaultSensor : public CSimulatedSensor,
                                   public CCI_RVRLightSensor
    {

    public:
        CRVRLightDefaultSensor();
        virtual ~CRVRLightDefaultSensor(){};

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    private:
        CEmbodiedEntity *m_pcEmbodiedEntity;
        CLightSensorEquippedEntity *m_pcLightEntity;
        CControllableEntity *m_pcControllableEntity;
        bool m_bShowRays;
        CRandom::CRNG *m_pcRNG;
        bool m_bAddNoise;
        CRange<Real> m_cNoiseRange;
        CSpace &m_cSpace;
    };

}

#endif