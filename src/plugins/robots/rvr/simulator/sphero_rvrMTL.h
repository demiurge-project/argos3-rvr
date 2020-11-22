/**
 * @file <argos3/plugins/robots/rvr/simulator/sphero_rvrMTL.h>
 *
 * @author Raffaele Todesco - Raffaele Todesco - <raffaele.todesco@ulb.be>
 * */

 /**
  * This class represents the 3 materials used to draw the RVR.
  * Each array represents the diffuse, ambient, specular and shininess of each material.
  * This was generated with mtl2opengl - https://github.com/ricardo-rendoncepeda/mtl2opengl
  */

  /*
  source files: ./sphero rvr.obj, ./sphero rvr.mtl
  materials: 3

  Name: Blue
  Ka: 1.000, 1.000, 1.000
  Kd: 0.077, 0.158, 0.976
  Ks: 0.500, 0.500, 0.500
  Ns: 225.000

  Name: Gray
  Ka: 1.000, 1.000, 1.000
  Kd: 0.123, 0.123, 0.123
  Ks: 0.500, 0.500, 0.500
  Ns: 225.000

  Name: White
  Ka: 1.000, 1.000, 1.000
  Kd: 1.000, 1.000, 1.000
  Ks: 0.500, 0.500, 0.500
  Ns: 225.000

  */


  /** Number of materials */
int sphero_rvrMTLNumMaterials = 3;

/** Beginning offset of each material */
int sphero_rvrMTLFirst[3] = {
0,
240,
2064,
};

/** Number of vertices for each material */
int sphero_rvrMTLCount[3] = {
240,
1824,
8064,
};

/** Ambient values */
float sphero_rvrMTLAmbient[3][3] = {
1.000,1.000,1.000,
1.000,1.000,1.000,
1.000,1.000,1.000,
};

/** Diffuse values */
float sphero_rvrMTLDiffuse[3][3] = {
0.077,0.158,0.976,
0.123,0.123,0.123,
1.000,1.000,1.000,
};

/** Specular values */
float sphero_rvrMTLSpecular[3][3] = {
0.500,0.500,0.500,
0.500,0.500,0.500,
0.500,0.500,0.500,
};

/** Shininess values */
float sphero_rvrMTLExponent[3] = {
225.000,
225.000,
225.000,
};

