/**
 * @file <argos3/plugins/robots/rvr/simulator/qtopengl_rvr.h>
 *
 * @author Raffaele Todesco - <rtodesco@ulb.be>
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
        void RenderBody();
        /** Sets a white plastic material */
        void SetWhitePlasticMaterial();

    private:
        /** Start of the display list index */
        GLuint m_unLists;

        /** Body display list */
        GLuint m_unBodyList;
    };
}

#endif