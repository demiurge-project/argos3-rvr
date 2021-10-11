/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_quaternion_equipped_entity.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef RVR_QUATERNION_EQUIPPED_ENTITY_H
#define RVR_QUATERNION_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos
{

    class CRVRQuaternionEquippedEntity : public CEntity
    {
    protected:
        CQuaternion m_fOrientation;

    public:
        ENABLE_VTABLE();

        CRVRQuaternionEquippedEntity(CComposableEntity *pc_parent);

        CRVRQuaternionEquippedEntity(CComposableEntity *pc_parent,
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
            return "rvr_quaternion";
        }

        CQuaternion GetOrientation() const;
        void setOrientation(const CQuaternion &fRotation);
    };

}

#endif