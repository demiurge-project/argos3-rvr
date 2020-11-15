/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.h>
 *
 * @author Raffaele Todesco - <raffaele.todesco@ulb.be>
 */

#ifndef QTOPENGL_EPUCK_H
#define QTOPENGL_EPUCK_H

namespace argos {
    class CQTOpenGLRVR;
    class CRVREntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

    class CQTOpenGLRVR {

    public:
        CQTOpenGLRVR();
        virtual ~CQTOpenGLRVR();
        virtual void Draw(CRVREntity& c_entity);

    protected:

        /** Functions meant to render each part of the robot */

        /** Renders the body */
        void RenderBody();
        /** Renders a wheel */
        void RenderWheel();


        /** Material functions */

        /** Sets a white plastic material */
        void SetWhitePlasticMaterial();
        /** Sets a gray plastic material */
        void SetGrayPlasticMaterial();
        /** Sets a red plastic material */
        void SetRedPlasticMaterial();

    private:
        /** Start of the display list index */
        GLuint m_unLists;

        /** Body display list */
        GLuint m_unBodyList;

        /** RVR wheels */
        GLuint m_unWheelList;

        /** Number of vertices to display the round parts
            (wheels, chassis, etc.) */
        GLuint m_unVertices;
    };
}

#endif