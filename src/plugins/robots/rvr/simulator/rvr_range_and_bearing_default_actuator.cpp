/**
 * @file <argos3/plugins/robots/rvr/simulator/rvr_range_and_bearing_default_actuator.cpp>
 *
 * @author Miquel Kegeleirs <miquel.kegeleirs@ulb.be>
 */

#include "rvr_range_and_bearing_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

/****************************************/
/****************************************/

void CRVRRABDefaultActuator::SetRobot(CComposableEntity& c_entity) {
    m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRVRRABEquippedEntity>("rab[rab_0]");
    m_pcRangeAndBearingEquippedEntity->Enable();
    m_pcRangeAndBearingEquippedEntity->SetDataSize(m_uDataSize);
}

/****************************************/
/****************************************/

void CRVRRABDefaultActuator::Init(TConfigurationNode& t_tree) {
    CCI_RVRRangeAndBearingActuator::Init(t_tree);
    Real fRange=0.8f;
    GetNodeAttributeOrDefault(t_tree, "range", fRange,fRange);
    m_pcRangeAndBearingEquippedEntity->SetRange(fRange);
}

/****************************************/
/****************************************/

void CRVRRABDefaultActuator::Update() {
    m_pcRangeAndBearingEquippedEntity->SetDataToSend(m_sDesiredData);
}

/****************************************/
/****************************************/





/****************************************/
/****************************************/

REGISTER_ACTUATOR(CRVRRABDefaultActuator,
                  "rvr_range_and_bearing", "medium",
                  "Gianpiero Francesca - [gianpiero.francesca@ulb.ac.be]"
                  "Lorenzo Garattoni- [lgaratto@ulb.ac.be]"
                  ,
                  "2.0",
                  "The range-and-bearing actuator of the rvr.",
                  "This actuator allows rvrs to perform situated communication, i.e., a form of\n"
                  "wireless communication whereby the receiver also knows the location of the\n"
                  "sender with respect to its own frame of reference.\n"
                  "This actuator allows a rvrs to send messages. To receive messages, you need\n"
                  "the range-and-bearing sensor.\n"
                  "For usage, refer to [ci_rvr_range_and_bearing_actuator.h]\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "   <controllers>\n"
                  "      ...\n"
                  "      <my_controller>\n"
                  "         ...\n"
                  "         <actuators>\n"
                  "            ...\n"
                  "            <rvr_range_and_bearing implementation=\"medium\" medium=\"rab\" range=\"0.8\"/>\n"
                  "            ...\n"
                  "         <actuators/>\n"
                  "         ...\n"
                  "      <my_controller/>\n"
                  "      ...\n"
                  "   <controllers>\n\n"
                  "The attribute [medium] refers to the [id] of the [range_and_bearing] medium defined\n"
                  "in the [media] xml section.\n"
                  "The attribute [range] defines the communication range of the rab in meters.\n"
                  "   <media>\n"
                  "      ...\n"
                  "      <range_and_bearing id=\"rab\"/>\n"
                  "      ...\n"
                  "   </media>"
                  , "Usable");

}
