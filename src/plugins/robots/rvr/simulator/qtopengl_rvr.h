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

#include <vector>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
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

        void Render();


        /** Material functions */

        /** Sets a white plastic material */
        void SetWhitePlasticMaterial();
        /** Sets a gray plastic material */
        void SetGrayPlasticMaterial();
        /** Sets a red plastic material */
        void SetRedPlasticMaterial();

    private:

        std::vector< argos::CVector3 > vertices;
        std::vector< argos::CVector2 > uvs;
        std::vector< argos::CVector3 > normals;


        /** Start of the display list index */
        GLuint m_unLists;

        /** Body display list */
        GLuint m_unBodyList;


        /** Number of vertices to display the round parts
            (wheels, chassis, etc.) */
        GLuint m_unVertices;
    };
}

#endif