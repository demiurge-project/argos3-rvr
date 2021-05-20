/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_lidar_sensor.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef RVR_LIDAR_SENSOR_H
#define RVR_LIDAR_SENSOR_H

namespace argos
{
    class CRVRLidarSensor;
}

#include "../control_interface/ci_rvr_lidar_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRLidarSensor : public CSimulatedSensor,
                            public CCI_RVRLidarSensor
    {

    public:
        CRVRLidarSensor();

        virtual ~CRVRLidarSensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

        /**
       * Calculates the proximity reading when the closest occluding
       * object is located as the given distance.
       *
       * @param f_distance The distance of the closest occluding
       * object in meters
       *
       * @returns A value in the range [0:1], where 0 means that the
       * object is too far to be sensed, and 1 means the object is so
       * close that it saturates the sensor.
       */
        virtual Real CalculateReading(Real f_distance);

    private:
        /** Reference to embodied entity associated to this sensor */
        CEmbodiedEntity *m_pcEmbodiedEntity;

        /** Reference to proximity sensor equipped entity associated to this sensor */
        CProximitySensorEquippedEntity *m_pcProximityEntity;

        /** Reference to controllable entity associated to this sensor */
        CControllableEntity *m_pcControllableEntity;

        /** Flag to show rays in the simulator */
        bool m_bShowRays;

        /** Random number generator */
        CRandom::CRNG *m_pcRNG;

        /** Whether to add noise or not */
        bool m_bAddNoise;

        /** Noise range */
        CRange<Real> m_cNoiseRange;

        /** Reference to the space */
        CSpace &m_cSpace;
    };

}

#endif
