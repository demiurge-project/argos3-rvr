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
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos
{

    static const UInt8 RVR_COMPONENTS_NUMBER = 2;

    CQTOpenGLRVR::CQTOpenGLRVR() : m_unVertices(40)
    {
        m_unLists = glGenLists(RVR_COMPONENTS_NUMBER);
        m_unBodyList = m_unLists;
        m_unLEDList = m_unLists + 1;

        /** Add body */
        glNewList(m_unBodyList, GL_COMPILE);
        Render();
        glEndList();

        /** Add LEDs */
        glNewList(m_unLEDList, GL_COMPILE);
        RenderLEDs();
        glEndList();
    }

    CQTOpenGLRVR::~CQTOpenGLRVR()
    {
        glDeleteLists(m_unLists, RVR_COMPONENTS_NUMBER);
    }

    void CQTOpenGLRVR::Draw(CRVREntity &c_entity)
    {
        /* Place the body */
        glCallList(m_unBodyList);

        /* Place the LEDs */
        glPushMatrix();
        CLEDEquippedEntity &cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
        for (UInt8 i = 0; i < 5; i++)
        {
            const CColor &cColor = cLEDEquippedEntity.GetLED(i).GetColor();
            glTranslatef(CRVREntity::LEDS_POSITIONS[i][0], CRVREntity::LEDS_POSITIONS[i][1], CRVREntity::LEDS_ELEVATION);
            SetLEDMaterial(cColor.GetRed() / 255.0f,
                           cColor.GetGreen() / 255.0f,
                           cColor.GetBlue() / 255.0f);
            glCallList(m_unLEDList);
            glTranslatef(-CRVREntity::LEDS_POSITIONS[i][0], -CRVREntity::LEDS_POSITIONS[i][1], -CRVREntity::LEDS_ELEVATION);
        }
        glPopMatrix();
    }

    void CQTOpenGLRVR::SetLEDMaterial(GLfloat f_red,
                                      GLfloat f_green,
                                      GLfloat f_blue)
    {
        const GLfloat fEmissionFactor = 0.0f;
        const GLfloat pfColor[] = {f_red, f_green, f_blue, 1.0f};
        const GLfloat pfSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        const GLfloat pfShininess[] = {0.0f};
        const GLfloat pfEmission[] = {f_red,
                                      f_green,
                                      f_blue,
                                      1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
    }

    void CQTOpenGLRVR::Render()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &sphero_rvrOBJVerts[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &sphero_rvrOBJTexCoords[0]);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, sphero_rvrOBJNormals);
        for (int i = 0; i < sphero_rvrMTLNumMaterials; i++)
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&sphero_rvrMTLColors[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&sphero_rvrMTLSpecular[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat *)&sphero_rvrMTLShininess[i]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)&sphero_rvrMTLEmission[i]);
            glDrawArrays(GL_TRIANGLES, sphero_rvrMTLFirst[i], sphero_rvrMTLCount[i]);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    void CQTOpenGLRVR::RenderLEDs()
    {
        glBegin(GL_QUAD_STRIP);
        GLfloat x = 0.0;
        GLfloat y = 0.0;
        GLfloat angle = 0.0;
        GLfloat angle_stepsize = 0.1;
        const GLfloat radius = CRVREntity::LED_OUTER_RADIUS;
        const GLfloat height = CRVREntity::LEDS_HEIGHT;
        while (angle < CRadians::TWO_PI.GetValue())
        {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y, height);
            glVertex3f(x, y, 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
        glEnd();
        glBegin(GL_POLYGON);
        angle = 0.0;
        while (angle < CRadians::TWO_PI.GetValue())
        {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y, height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glEnd();
    }
    class CQTOpenGLOperationDrawRVRNormal : public CQTOpenGLOperationDrawNormal
    {
    public:
        void ApplyTo(CQTOpenGLWidget &c_visualization,
                     CRVREntity &c_entity)
        {
            static CQTOpenGLRVR m_cModel;
            c_visualization.DrawRays(c_entity.GetControllableEntity());
            c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
            m_cModel.Draw(c_entity);
        }
    };

    class CQTOpenGLOperationDrawRVRSelected : public CQTOpenGLOperationDrawSelected
    {
    public:
        void ApplyTo(CQTOpenGLWidget &c_visualization,
                     CRVREntity &c_entity)
        {
            c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
        }
    };

    REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawRVRNormal, CRVREntity);

    REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawRVRSelected, CRVREntity);

}