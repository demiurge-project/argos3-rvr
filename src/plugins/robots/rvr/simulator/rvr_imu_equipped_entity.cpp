/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_quaternion_equipped_entity.cpp>
 *
 */

#include "rvr_imu_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos
{

    CRadians CRVRIMUEquippedEntity::GetPitch() const
    {
        return Pitch;
    }

    CRadians CRVRIMUEquippedEntity::GetRoll() const
    {
        return Roll;
    }

    CRadians CRVRIMUEquippedEntity::GetYaw() const
    {
        return Yaw;
    }

    void CRVRIMUEquippedEntity::setPitch(const CRadians &fPitch)
    {
        Pitch = fPitch;
    }

    void CRVRIMUEquippedEntity::setRoll(const CRadians &fRoll)
    {
        Roll = fRoll;
    }

    void CRVRIMUEquippedEntity::setYaw(const CRadians &fYaw)
    {
        Yaw = fYaw;
    }

    /****************************************/
    /****************************************/

    CRVRIMUEquippedEntity::CRVRIMUEquippedEntity(CComposableEntity *pc_parent) : CEntity(pc_parent),
                                                                                 Pitch(CRadians()), Roll(CRadians()), Yaw(CRadians())
    {
    }

    /****************************************/
    /****************************************/

    CRVRIMUEquippedEntity::CRVRIMUEquippedEntity(CComposableEntity *pc_parent,
                                                 const std::string &str_id) : CEntity(pc_parent, str_id), Pitch(CRadians()), Roll(CRadians()), Yaw(CRadians())
    {
    }

    /****************************************/
    /****************************************/

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CRVRIMUEquippedEntity);

    /****************************************/
    /****************************************/

}