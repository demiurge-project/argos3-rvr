/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_range_and_bearing_default_actuator.h>
 *
 * @author Miquel Kegeleirs <miquel.kegeleirs@ulb.be>
 *
 * Range and bearing default actuator implementation for the rvr simulation
 */

#ifndef RVR_RAB_DEFAULT_ACTUATOR_H
#define RVR_RAB_DEFAULT_ACTUATOR_H

namespace argos {
   class CRVRRABDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include "rvr_rab_equipped_entity.h"
#include "../control_interface/ci_rvr_range_and_bearing_actuator.h"

namespace argos {

   class CRVRRABDefaultActuator : public CSimulatedActuator, public CCI_RVRRangeAndBearingActuator {

   public:

      CRVRRABDefaultActuator() {}
      virtual ~CRVRRABDefaultActuator() {}


      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Init(TConfigurationNode& t_tree);

   private:

      CRVRRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;

   };

}

#endif
