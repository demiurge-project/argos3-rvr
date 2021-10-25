/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_imu_sensor.h>
 *
 */

#ifndef RVR_IMU_SENSOR_H
#define RVR_IMU_SENSOR_H

namespace argos
{
    class CRVRIMUSensor;
    class CRVRIMUEquippedEntity;
}

#include "../control_interface/ci_rvr_imu_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{

    class CRVRIMUSensor : public CSimulatedSensor,
                          public CCI_RVRIMUSensor
    {

    public:
        CRVRIMUSensor();

        virtual ~CRVRIMUSensor() {}

        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    private:
        /** Reference to quaternion sensor equipped entity associated to this sensor */
        //CRVRIMUEquippedEntity *m_pcImuEntity;

        /** Reference to the embodied entity */
        CEmbodiedEntity *m_pcEmbodiedEntity;
    };

}

#endif