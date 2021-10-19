/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_imu_equipped_entity.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef RVR_IMU_EQUIPPED_ENTITY_H
#define RVR_IMU_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/angles.h>

namespace argos
{

    class CRVRIMUEquippedEntity : public CEntity
    {
    protected:
        CRadians Pitch, Roll, Yaw;

    public:
        ENABLE_VTABLE();

        CRVRIMUEquippedEntity(CComposableEntity *pc_parent);

        CRVRIMUEquippedEntity(CComposableEntity *pc_parent,
                              const std::string &str_id);

        virtual void Init(TConfigurationNode &t_tree)
        {
            /*
           * Parse basic entity stuff
           */
            CEntity::Init(t_tree);
        }
        virtual std::string GetTypeDescription() const
        {
            return "rvr_imu";
        }

        CRadians GetPitch() const;
        CRadians GetRoll() const;
        CRadians GetYaw() const;

        void setPitch(const CRadians &fPitch);
        void setRoll(const CRadians &fRoll);
        void setYaw(const CRadians &fYaw);
    };

}

#endif