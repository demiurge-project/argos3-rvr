/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.cpp>
 * 
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "qtopengl_rvr.h"
#include "rvr_entity.h"
#include "sphero_rvrMTL.h"
#include "sphero_rvrOBJ.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

    static const UInt8 RVR_COMPONENTS_NUMBER = 1; // only 1 component : full body

    CQTOpenGLRVR::CQTOpenGLRVR() {
        m_unLists = glGenLists(RVR_COMPONENTS_NUMBER);
        m_unBodyList = m_unLists;
        glNewList(m_unBodyList, GL_COMPILE);
        Render();
        glEndList();
    }

    CQTOpenGLRVR::~CQTOpenGLRVR() {
        glDeleteLists(m_unLists, RVR_COMPONENTS_NUMBER);
    }

    void CQTOpenGLRVR::Render() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &sphero_rvrOBJVerts[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &sphero_rvrOBJTexCoords[0]);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, sphero_rvrOBJNormals);
        for (int i = 0; i < sphero_rvrMTLNumMaterials; i++)
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&sphero_rvrMTLAmbient[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&sphero_rvrMTLDiffuse[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&sphero_rvrMTLSpecular[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat*)&sphero_rvrMTLExponent[i]);

            // Draw robot by material group
            glDrawArrays(GL_TRIANGLES, sphero_rvrMTLFirst[i], sphero_rvrMTLCount[i]);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }



    void CQTOpenGLRVR::Draw(CRVREntity& c_entity) {
        /* Place the body */
        glCallList(m_unBodyList);

    }


    class CQTOpenGLOperationDrawRVRNormal : public CQTOpenGLOperationDrawNormal {
    public:
        void ApplyTo(CQTOpenGLWidget& c_visualization,
            CRVREntity& c_entity) {
            static CQTOpenGLRVR m_cModel;
            c_visualization.DrawRays(c_entity.GetControllableEntity());
            c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
            m_cModel.Draw(c_entity);
        }
    };

    class CQTOpenGLOperationDrawRVRSelected : public CQTOpenGLOperationDrawSelected {
    public:
        void ApplyTo(CQTOpenGLWidget& c_visualization,
            CRVREntity& c_entity) {
            c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
        }
    };

    REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawRVRNormal, CRVREntity);

    REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawRVRSelected, CRVREntity);

}