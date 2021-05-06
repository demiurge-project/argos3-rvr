/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef QTOPENGL_RVR_H
#define QTOPENGL_RVR_H

namespace argos
{
    class CQTOpenGLRVR;
    class CRVREntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos
{

    class CQTOpenGLRVR
    {

    public:
        CQTOpenGLRVR();
        virtual ~CQTOpenGLRVR();
        virtual void Draw(CRVREntity &c_entity);

    protected:
        /** Renders the robot */
        void Render();

        /** Renders the LEDs */
        void RenderLEDs();

        /** Sets a colored LED material */
        void SetLEDMaterial(GLfloat f_red,
                            GLfloat f_green,
                            GLfloat f_blue);

    private:
        /** Start of the display list index */
        GLuint m_unLists;

        /** Body display list */
        GLuint m_unBodyList;

        /** LED display list */
        GLuint m_unLEDList;

        /** Number of vertices to display the round parts
    (LEDs,...) */
        GLuint m_unVertices;
    };
}

#endif