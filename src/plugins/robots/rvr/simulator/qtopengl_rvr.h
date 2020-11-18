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


#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
namespace argos {

    class CQTOpenGLRVR {

    public:
        CQTOpenGLRVR();
        virtual ~CQTOpenGLRVR();
        virtual void Draw(CRVREntity& c_entity);

    protected:

        /** Renders the robot */
        void Render();

    private:


        /** Start of the display list index */
        GLuint m_unLists;

        /** Body display list */
        GLuint m_unBodyList;
    };
}

#endif