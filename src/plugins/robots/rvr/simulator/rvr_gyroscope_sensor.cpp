#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "rvr_gyroscope_sensor.h"

#define G 9.81

namespace argos {

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRGyroscopeSensor::CRVRGyroscopeSensor() : m_bAddNoise(false), m_pcRNG(NULL) {};

    /****************************************/
    /****************************************/

    void CRVRGyroscopeSensor::Init(TConfigurationNode &t_tree) {
        CCI_RVRGyroscopeSensor::Init(t_tree);
        Real fNoiseLevel = 0.0f;
        GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
        if (fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                         << " of the rvr gyroscope sensor");
        } else if (fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
        }
    }

    /****************************************/
    /****************************************/

    void CRVRGyroscopeSensor::SetRobot(CComposableEntity &c_entity) {
        try {
            m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            // immediately store inverse to compute it once
            // since only inverse will be used to compute relative rotation
            m_sPreviousAngularPosition = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse().Normalize();
        }
        catch (CARGoSException &ex) {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr accelerometer sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRGyroscopeSensor::Update() {
        // compute relative orientation
        auto currentRotation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Normalize();
        CQuaternion relativeOrientation =
                currentRotation * m_sPreviousAngularPosition;
        // convert to z angle for rotation
        CRadians cRotX, cRotY, cRotZ;
        relativeOrientation.ToEulerAngles(cRotZ, cRotY, cRotX);
        // Get inverse deltatime
        auto inverseDt = CPhysicsEngine::GetInverseSimulationClockTick();
        // compute speed along all axis
        auto angularSpeed = CVector3(cRotX.GetValue(), cRotY.GetValue(), cRotZ.GetValue()) * inverseDt;
        if (m_bAddNoise) {
            auto noise = CVector3(m_pcRNG->Uniform(m_cNoiseRange), m_pcRNG->Uniform(m_cNoiseRange),
                                  m_pcRNG->Uniform(m_cNoiseRange));
            angularSpeed += noise;
        }
        // write reading
        m_tReading.AngularVelocity = angularSpeed;
        // update previous position
        m_sPreviousAngularPosition = currentRotation.Inverse();
    }

    /****************************************/
    /****************************************/

    void CRVRGyroscopeSensor::Reset() {
        m_sPreviousAngularPosition = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse().Normalize();
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRGyroscopeSensor,
                    "rvr_gyroscope", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr gyroscope sensor",
                    "The sensor returns 3 angular velocities in radians/s.\n"
                    "For usage, refer to [ci_rvr_accelerometer_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_gyroscope implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}
