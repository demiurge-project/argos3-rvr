/** 
 * @file <argos3/plugins/robots/rvr/simulator/rvr_ground_color_sensor.h>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
*/

#ifndef RVR_GROUND_COLOR_SENSOR_H
#define RVR_GROUND_COLOR_SENSOR_H

namespace argos
{
    class CRVRGroundColorSensor;
    class CGroundSensorEquippedEntity;
    class CFloorEntity;
} // namespace argos

#include "../control_interface/ci_rvr_ground_color_sensor.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos
{
    class CRVRGroundColorSensor : public CSimulatedSensor,
                                  public CCI_RVRGroundColorSensor
    {
    public:
        CRVRGroundColorSensor();
        virtual ~CRVRGroundColorSensor() {}
        virtual void SetRobot(CComposableEntity &c_entity);
        virtual void Init(TConfigurationNode &t_tree);
        virtual void Update();
        virtual void Reset();

    protected:
        /** Reference to the embodied entity associated to this sensor */
        CEmbodiedEntity *m_pcEmbodiedEntity;

        /** Reference to the floor entity */
        CFloorEntity *m_pcFloorEntity;

        /** Reference to the ground sensor equipped entity associated to this sensor */
        CGroundSensorEquippedEntity *m_pcGroundSensorEntity;

        /** Reference to the space */
        CSpace &m_cSpace;
    };
} // namespace argos

#endif