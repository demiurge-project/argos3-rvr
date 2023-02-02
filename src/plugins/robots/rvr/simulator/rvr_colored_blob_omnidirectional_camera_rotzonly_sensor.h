#ifndef RVR_COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H
#define RVR_COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H

namespace argos {
   class CRVRColoredBlobOmnidirectionalCameraRotZOnlySensor;
   class COmnidirectionalCameraEquippedEntity;
   class CLEDEntity;
   class CControllableEntity;
   class COmnidirectionalCameraLEDCheckOperation;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include "../control_interface/ci_rvr_colored_blob_omnidirectional_camera_sensor.h"

namespace argos {

  class CRVRColoredBlobOmnidirectionalCameraRotZOnlySensor : public CCI_RVRColoredBlobOmnidirectionalCameraSensor,
                                                          public CSimulatedSensor {

   public:

      CRVRColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual ~CRVRColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

      virtual void Enable();
      
      virtual void Disable();
			
   protected:

      bool                                     m_bEnabled;
      COmnidirectionalCameraEquippedEntity*    m_pcOmnicamEntity;
      CControllableEntity*                     m_pcControllableEntity;
      CEmbodiedEntity*                         m_pcEmbodiedEntity;
      CPositionalIndex<CLEDEntity>*            m_pcLEDIndex;
      CPositionalIndex<CEmbodiedEntity>*       m_pcEmbodiedIndex;
      COmnidirectionalCameraLEDCheckOperation* m_pcOperation;
      bool                                     m_bShowRays;

   };
}

#endif
