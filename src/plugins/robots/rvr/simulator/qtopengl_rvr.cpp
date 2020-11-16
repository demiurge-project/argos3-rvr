/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.cpp>
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#include "qtopengl_rvr.h"
#include "rvr_entity.h"
#include "OBJLoader.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {
    /* Main body properties */
    static const Real BODY_HEIGHT = 0.114f;
    static const Real HALF_BODY_HEIGHT = BODY_HEIGHT * 0.5f;
    static const Real BODY_LENGTH = 0.1839f;
    static const Real HALF_BODY_LENGTH = BODY_LENGTH * 0.5f;
    static const Real BODY_WIDTH = 0.2159f;
    static const Real HALF_BODY_WIDTH = BODY_WIDTH * 0.5f;
    static const Real BODY_ELEVATION = 0.0f;
    /* Wheel properties */
    static const Real WHEEL_RADIUS = 0.041f;
    static const Real WHEEL_WIDTH = 0.05f;
    static const Real HALF_WHEEL_WIDTH = WHEEL_WIDTH * 0.5f;
    static const Real HALF_INTERWHEEL_DISTANCE = HALF_BODY_WIDTH + HALF_WHEEL_WIDTH;

    static const UInt8 RVR_COMPONENTS_NUMBER = 1;

    CQTOpenGLRVR::CQTOpenGLRVR() :
        m_unVertices(40) { // Won't be used at the moment.
        OBJLoader::loadModelData("sphero_rvr.obj", vertices, uvs, normals);
        printf("%lf", vertices[0].GetX());
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
        glDrawElements(GL_ARRAY_BUFFER, vertices.size() * sizeof(argos::CVector3), GL_UNSIGNED_INT, &vertices[0]);
    }



    void CQTOpenGLRVR::Draw(CRVREntity& c_entity) {
        /* Place the body */
        glCallList(m_unBodyList);

        /*
        glPushMatrix();
        glTranslatef(HALF_BODY_LENGTH, HALF_INTERWHEEL_DISTANCE, 0.0f);
        glCallList(m_unWheelList);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(HALF_BODY_LENGTH, -HALF_INTERWHEEL_DISTANCE, 0.0f);
        glCallList(m_unWheelList);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-HALF_BODY_LENGTH, -HALF_INTERWHEEL_DISTANCE, 0.0f);
        glCallList(m_unWheelList);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-HALF_BODY_LENGTH, HALF_INTERWHEEL_DISTANCE, 0.0f);
        glCallList(m_unWheelList);
        glPopMatrix();
        */

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

    void CQTOpenGLRVR::SetGrayPlasticMaterial() {
        const GLfloat pfColor[] = { 0.5f, 0.5f, 0.5f, 0.6f };
        const GLfloat pfSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        const GLfloat pfShininess[] = { 20.0f };
        const GLfloat pfEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
    }

    void CQTOpenGLRVR::SetRedPlasticMaterial() {
        const GLfloat pfColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        const GLfloat pfSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        const GLfloat pfShininess[] = { 100.0f };
        const GLfloat pfEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
    }

    /**
     * Renders RVR Body. Currently modeled as a simple gray box.
     */
    void CQTOpenGLRVR::RenderBody() {
        /* Set material */
        //SetWhitePlasticMaterial();
        SetGrayPlasticMaterial();
        //SetRedPlasticMaterial();
        /* Bottom face */
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION);
        glVertex3f(HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION);
        glVertex3f(-HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION);
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
        /* Upper face */
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION + BODY_HEIGHT);
        glVertex3f(HALF_BODY_LENGTH, -HALF_BODY_WIDTH, BODY_ELEVATION + BODY_HEIGHT);
        glVertex3f(HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION + BODY_HEIGHT);
        glVertex3f(-HALF_BODY_LENGTH, HALF_BODY_WIDTH, BODY_ELEVATION + BODY_HEIGHT);
        glEnd();

    }

    /**
     * Renders an RVR Wheel as a simple white cylinder.
     */
    void CQTOpenGLRVR::RenderWheel() {
        SetWhitePlasticMaterial();
        /* Right side */
        CVector2 cVertex(WHEEL_RADIUS, 0.0f);
        CRadians cAngle(CRadians::TWO_PI / m_unVertices);
        CVector3 cNormal(-1.0f, -1.0f, 0.0f);
        cNormal.Normalize();
        glBegin(GL_POLYGON);
        for (GLuint i = 0; i <= m_unVertices; i++) {
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
            cVertex.Rotate(cAngle);
            cNormal.RotateY(cAngle);
        }
        /* Left side */
        cVertex.Set(WHEEL_RADIUS, 0.0f);
        cNormal.Set(-1.0f, 1.0f, 0.0f);
        cNormal.Normalize();
        cAngle = -cAngle;
        glBegin(GL_POLYGON);
        for (GLuint i = 0; i <= m_unVertices; i++) {
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cVertex.GetX(), HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
            cVertex.Rotate(cAngle);
            cNormal.RotateY(cAngle);
        }
        glEnd();
        /* Tire */
        cNormal.Set(1.0f, 0.0f, 0.0f);
        cVertex.Set(WHEEL_RADIUS, 0.0f);
        cAngle = -cAngle;
        glBegin(GL_QUAD_STRIP);
        for (GLuint i = 0; i <= m_unVertices; i++) {
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
            glVertex3f(cVertex.GetX(), HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
            cVertex.Rotate(cAngle);
            cNormal.RotateY(cAngle);
        }
        glEnd();
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