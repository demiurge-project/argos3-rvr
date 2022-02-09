#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "rvr_accelerometer_sensor.h"

#define G 9.81

namespace argos {

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    CRVRAccelerometerSensor::CRVRAccelerometerSensor() : m_bAddNoise(false), m_pcRNG(NULL) {};

    /****************************************/
    /****************************************/

    void CRVRAccelerometerSensor::Init(TConfigurationNode &t_tree) {
        CCI_RVRAccelerometerSensor::Init(t_tree);
        Real fNoiseLevel = 0.0f;
        GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
        if (fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level"
                                         << " of the rvr accelerometer sensor");
        } else if (fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
        }
    }

    /****************************************/
    /****************************************/

    void CRVRAccelerometerSensor::SetRobot(CComposableEntity &c_entity) {
        try {
            m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
            m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
            // immediately store inverse to compute it once
            // since only inverse will be used to compute relative rotation
            m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse().Normalize();
        }
        catch (CARGoSException &ex) {
            THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the rvr accelerometer sensor", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CRVRAccelerometerSensor::Update() {
        // get average wheel velocity as our robot velocity
        Real velocity = (m_pcWheeledEntity->GetWheelVelocity(0) + m_pcWheeledEntity->GetWheelVelocity(1)) / 2;
        // compute relative orientation
        CQuaternion relativeOrientation =
                m_sInitialOrientation * m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Normalize();
        // convert to z angle
        CRadians cTmp1, cTmp2, cRotZ;
        relativeOrientation.ToEulerAngles(cRotZ, cTmp1, cTmp2);
        // add 90 degrees since Y is forward
        cRotZ += CRadians::PI_OVER_TWO;
        // Project on X and Y to get readings
        auto currentVelocity = CVector2(velocity * Cos(cRotZ), velocity * Sin(cRotZ));
        // Get inverse deltatime
        auto inverseDt = CPhysicsEngine::GetInverseSimulationClockTick();
        // compute acceleration in X and Y
        auto xyAcceleration = (currentVelocity - m_sPreviousVelocity) * inverseDt;
        // write reading
        m_tReading.Acceleration = CVector3(xyAcceleration.GetX(), xyAcceleration.GetY(), G);
        // convert acceleration from m/s^2 to g
        m_tReading.Acceleration /= G;
        if (m_bAddNoise) {
            auto noise = CVector3(m_pcRNG->Uniform(m_cNoiseRange), m_pcRNG->Uniform(m_cNoiseRange),
                                  m_pcRNG->Uniform(m_cNoiseRange));
            m_tReading.Acceleration += noise;
        }
        // update previous speed
        m_sPreviousVelocity = currentVelocity;

    }

    /****************************************/
    /****************************************/

    void CRVRAccelerometerSensor::Reset() {
        m_sInitialOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation.Inverse();
        m_sPreviousVelocity = CVector2(0, 0);
    }

    /****************************************/
    /****************************************/

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CRVRAccelerometerSensor,
                    "rvr_accelerometer", "default",
                    "Raffaele Todesco   [raffaele.todesco@ulb.be]",
                    "1.0",
                    "The rvr accelerometer sensor",
                    "The sensor returns 3 acceleration in g\n"
                    "that represents the speed of the robot in the cartesian system that is defined at boot time.\n"
                    "For usage, refer to [ci_rvr_accelerometer_sensor.h]\n\n"
                    "REQUIRED XML CONFIGURATION\n\n"
                    "   <controllers>\n"
                    "      ...\n"
                    "      <my_controller>\n"
                    "         ...\n"
                    "         <sensors>\n"
                    "            ...\n"
                    "            <rvr_accelerometer implementation=\"default\"/>\n"
                    "            ...\n"
                    "         <sensors/>\n"
                    "         ...\n"
                    "      <my_controller/>\n"
                    "      ...\n"
                    "   <controllers>\n\n",
                    "Usable");
}