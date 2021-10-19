/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_quaternion_equipped_entity.cpp>
 *
 */

#include "rvr_quaternion_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos
{

    /****************************************/
    /****************************************/

    CQuaternion CRVRQuaternionEquippedEntity::GetOrientation() const
    {
        return m_fOrientation;
    }

    void CRVRQuaternionEquippedEntity::setOrientation(const CQuaternion &fRotation)
    {
        m_fOrientation = fRotation;
    }
    CRVRQuaternionEquippedEntity::CRVRQuaternionEquippedEntity(CComposableEntity *pc_parent) : CEntity(pc_parent),
                                                                                               m_fOrientation(CQuaternion())
    {
    }

    /****************************************/
    /****************************************/

    CRVRQuaternionEquippedEntity::CRVRQuaternionEquippedEntity(CComposableEntity *pc_parent,
                                                               const std::string &str_id) : CEntity(pc_parent, str_id),
                                                                                            m_fOrientation(CQuaternion())
    {
    }

    /****************************************/
    /****************************************/

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CRVRQuaternionEquippedEntity);

    /****************************************/
    /****************************************/

}