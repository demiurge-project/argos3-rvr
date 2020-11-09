/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.cpp>
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "qtopengl_rvr.h"
#include "rvr_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {
    static const Real BODY_HEIGHT = 0.114f;
    static const Real HALF_BODY_HEIGHT = BODY_HEIGHT * 0.5f;
    static const Real BODY_LENGTH = 0.1839f;
    static const Real HALF_BODY_LENGTH = BODY_LENGTH * 0.5f;
    static const Real BODY_WIDTH = 0.2159f;
    static const Real HALF_BODY_WIDTH = BODY_WIDTH * 0.5f;

    static const Real BODY_ELEVATION = 0.0f;

    static const UInt8 RVR_COMPONENTS_NUMBER = 1; // only body for now

    CQTOpenGLRVR::CQTOpenGLRVR() {
        m_unLists = glGenLists(RVR_COMPONENTS_NUMBER);
        m_unBodyList = m_unLists;

        /* Creates the body display list */
        glNewList(m_unBodyList, GL_COMPILE);
        RenderBody();
        glEndList();

    }

    CQTOpenGLRVR::~CQTOpenGLRVR() {
        glDeleteLists(m_unLists, RVR_COMPONENTS_NUMBER);
    }

    void CQTOpenGLRVR::Draw(CRVREntity& c_entity) {
        /* Place the body */
        glCallList(m_unBodyList);
        glPushMatrix();
    }

    void CQTOpenGLRVR::SetWhitePlasticMaterial() {
        const GLfloat pfColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        const GLfloat pfSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        const GLfloat pfShininess[] = { 100.0f };
        const GLfloat pfEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
    }

    void CQTOpenGLRVR::RenderBody() {
        /* Set material */
        SetWhitePlasticMaterial();
        /* Bottom face */
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_HEIGHT);
        glVertex3f(HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_HEIGHT);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_HEIGHT);
        glVertex3f(-HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_HEIGHT);
        glEnd();

        /* Side faces */
        glBegin(GL_QUAD_STRIP);
        /* First face */
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_HEIGHT + BODY_ELEVATION);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION);
        /* South face */
        glVertex3f(HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_HEIGHT + BODY_ELEVATION);
        glVertex3f(HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION);
        /* East face */
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_HEIGHT + BODY_ELEVATION);
        glVertex3f(HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION);
        /* North face */
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_HEIGHT + BODY_ELEVATION);
        glVertex3f(-HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION);
        /* West face */
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_HEIGHT + BODY_ELEVATION);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION);
        glEnd();

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