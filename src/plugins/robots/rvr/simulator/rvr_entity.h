/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_entity.h>
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef RVR_ENTITY_H
#define RVR_ENTITY_H

namespace argos
{
    class CControllableEntity;
    class CRVREntity;
    class CEmbodiedEntity;
    class CLEDEquippedEntity;
    class CGroundSensorEquippedEntity;
    class CProximitySensorEquippedEntity;
    class CRVRQuaternionEquippedEntity;
    class CLightSensorEquippedEntity;
} // namespace argos

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>

namespace argos
{
    class CRVREntity : public CComposableEntity
    {
    public:
        ENABLE_VTABLE();

    public:
        CRVREntity();

        CRVREntity(const std::string &str_id,
                   const std::string &str_controller_id,
                   const CVector3 &c_position = CVector3(),
                   const CQuaternion &c_orientation = CQuaternion());

        virtual void Init(TConfigurationNode &t_tree);
        virtual void Reset();
        virtual void Destroy();
        virtual void UpdateComponents();

        inline CControllableEntity &GetControllableEntity()
        {
            return *m_pcControllableEntity;
        }

        inline CEmbodiedEntity &GetEmbodiedEntity()
        {
            return *m_pcEmbodiedEntity;
        }

        inline CLEDEquippedEntity &GetLEDEquippedEntity()
        {
            return *m_pcLEDEquippedEntity;
        }

        inline CWheeledEntity &GetWheeledEntity()
        {
            return *m_pcWheeledEntity;
        }

        inline CProximitySensorEquippedEntity &GetProximitySensorEquippedEntity()
        {
            return *m_pcProximitySensorEquippedEntity;
        }

        inline CRVRQuaternionEquippedEntity &GetRVRQuaternionEquippedEntity()
        {
            return *m_pcQuaternionEquippedEntity;
        }

        inline CLightSensorEquippedEntity &GetLightSensorEquippedEntity()
        {
            return *m_pcLightSensorEquippedEntity;
        }

        virtual std::string GetTypeDescription() const
        {
            return "rvr";
        }

    private:
        CControllableEntity *m_pcControllableEntity;
        CEmbodiedEntity *m_pcEmbodiedEntity;
        CGroundSensorEquippedEntity *m_pcGroundSensorEquippedEntity;
        CLEDEquippedEntity *m_pcLEDEquippedEntity;
        CProximitySensorEquippedEntity *m_pcProximitySensorEquippedEntity;
        CRVRQuaternionEquippedEntity *m_pcQuaternionEquippedEntity;
        CLightSensorEquippedEntity *m_pcLightSensorEquippedEntity;
        CWheeledEntity *m_pcWheeledEntity;

    public:
        /** Body properties */
        static const Real BODY_LENGTH;
        static const Real BODY_HEIGHT;
        static const Real BODY_WIDTH;

        /** Wheel properties */
        static const Real INTERWHEEL_DISTANCE; // distance between wheels of opposite sides (width distance)
        static const Real HALF_INTERWHEEL_DISTANCE;
        static const Real WHEEL_RADIUS;
        static const Real LENGTH_WHEEL_DISTANCE;
        static const Real HALF_LENGTH_WHEEL_DISTANCE;

        /** LEDs properties */
        static const Real LEDS_ELEVATION;
        static const Real LEDS_POSITIONS[5][2];
        static const Real LEDS_HEIGHT;
        static const Real LED_INNER_RADIUS;
        static const Real LED_OUTER_RADIUS;

        /** Proximity sensor properties */
        static const Real PROXIMITY_SENSOR_RING_ELEVATION;
        static const Real PROXIMITY_SENSOR_RING_RADIUS;
        static const CRadians PROXIMITY_SENSOR_RING_START_ANGLE;
        static const Real PROXIMITY_SENSOR_RING_RANGE;
        static const CRadians PROXIMITY_SENSOR_ANGLES[8];

        /** Lidar properties */
        static const Real LIDAR_ELEVATION;
        static const Real LIDAR_RADIUS;
        static const CRadians LIDAR_START_ANGLE;
        static const Real LIDAR_RANGE;

        /* Ground sensor */

        static const Real GROUND_SENSOR_OFFSET;
    };
} // namespace argos

#endif