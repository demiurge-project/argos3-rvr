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
int sphero_rvrMTLNumMaterials = 4;

int sphero_rvrMTLFirst[4] = {
0,
240,
2064,
9348,
};

int sphero_rvrMTLCount[4] = {
240,
1824,
7284,
780,
};

float sphero_rvrMTLAmbient[4][3] = {
1.000,1.000,1.000,
1.000,1.000,1.000,
1.000,1.000,1.000,
1.000,1.000,1.000,
};

float sphero_rvrMTLDiffuse[4][3] = {
0.077,0.158,0.976,
0.123,0.123,0.123,
1.000,1.000,1.000,
0.216,0.800,0.015,
};

float sphero_rvrMTLSpecular[4][4] = {
0.9f, 0.9f, 0.9f, 1.0f,
0.9f, 0.9f, 0.9f, 1.0f,
0.9f, 0.9f, 0.9f, 1.0f,
0.9f, 0.9f, 0.9f, 1.0f
};

float sphero_rvrMTLColors[4][4] = {
0.000, 0.000, 1.000, 1.0f, // blue RGBA over 255
0.5f, 0.5f, 0.5f, 1.0f,// gray RGBA (for trails)
1.0f, 1.0f, 1.0f, 1.0f,// white
0.0f, 1.0f, 0.0f, 1.0f,// green
};

float sphero_rvrMTLShininess[4] = {
100.000,
100.000,
100.000,
100.000,
};

float sphero_rvrMTLEmission[4][4] = {
0.0f, 0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 0.0f, 1.0f,
};

