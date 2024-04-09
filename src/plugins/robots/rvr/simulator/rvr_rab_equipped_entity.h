/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_rab_equipped_entity.h>
 *
 * @author Miquel Kegeleirs <miquel.kegeleirs@ulb.be>
 */

#ifndef RVR_RAB_EQUIPPED_ENTITY_H
#define RVR_RAB_EQUIPPED_ENTITY_H

namespace argos {
class CRVRRABEquippedEntity;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include "../control_interface/ci_rvr_range_and_bearing_actuator.h"
namespace argos {

class CRVRRABEquippedEntity : public CRABEquippedEntity {
public:

public:

    ENABLE_VTABLE();

    typedef std::vector<CRVRRABEquippedEntity*> TVector;
    typedef CSet<CRVRRABEquippedEntity*> TSet;
private:
    /** this is the data size */
    UInt8 m_uDataSize;
    CCI_RVRRangeAndBearingActuator::SDataToSend m_cEmitterData;
    CEmbodiedEntity* m_pcEntityBody;
public:

    CRVRRABEquippedEntity(CComposableEntity* pc_parent);

    CRVRRABEquippedEntity(CComposableEntity* pc_parent,
                            const std::string& str_id,
                            size_t un_msg_size,
                            Real f_range,
                            SAnchor& s_anchor,
                            CEmbodiedEntity& c_entity_body,
                            const CVector3& c_position = CVector3(0,0,0),
                            const CQuaternion& c_orientation = CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));


    virtual ~CRVRRABEquippedEntity() {}

    virtual void Reset();

    //virtual void Update();

    inline size_t GetMsgSize() const {
        return m_uDataSize;
    }
    inline void SetDataSize(UInt8 uDataSize){
        m_uDataSize=uDataSize;
    }

    void SetDataToSend(const CCI_RVRRangeAndBearingActuator::SDataToSend& c_data);

    void ClearData();

    inline Real GetRange() const {
        return m_fRange;
    }
    inline void SetRange(Real frange){
        m_fRange=frange;
    }
    inline CEmbodiedEntity& GetReference() {
        return *m_pcEntityBody;
    }

    virtual std::string GetTypeDescription() const {
        return "rab";
    }
    CCI_RVRRangeAndBearingActuator::SEmitter& GetEmitterForAngle(const CRadians& t_angle);

};

/****************************************/
/****************************************/

class CRVRRABEquippedEntitySpaceHashUpdater : public CSpaceHashUpdater<CRVRRABEquippedEntity> {

public:

    virtual void operator()(CAbstractSpaceHash<CRVRRABEquippedEntity>& c_space_hash,
                            CRVRRABEquippedEntity& c_element);

private:

    SInt32 m_nCenterI, m_nCenterJ, m_nCenterK;

};

/****************************************/
/****************************************/

class CRVRRABEquippedEntityGridCellUpdater : public CGrid<CRVRRABEquippedEntity>::CCellOperation {

public:

    CRVRRABEquippedEntityGridCellUpdater(CGrid<CRVRRABEquippedEntity>& c_grid);

    virtual bool operator()(SInt32 n_i,
                            SInt32 n_j,
                            SInt32 n_k,
                            CGrid<CRVRRABEquippedEntity>::SCell& s_cell);

    void SetEntity(CRVRRABEquippedEntity& c_entity);

private:

    CGrid<CRVRRABEquippedEntity>& m_cGrid;
    CRVRRABEquippedEntity* m_pcEntity;
};

class CRVRRABEquippedEntityGridEntityUpdater : public CGrid<CRVRRABEquippedEntity>::COperation {

public:

    CRVRRABEquippedEntityGridEntityUpdater(CGrid<CRVRRABEquippedEntity>& c_grid);
    virtual bool operator()(CRVRRABEquippedEntity& c_entity);

private:

    CGrid<CRVRRABEquippedEntity>& m_cGrid;
    CRVRRABEquippedEntityGridCellUpdater m_cCellUpdater;
};

/****************************************/
/****************************************/

}

#endif
