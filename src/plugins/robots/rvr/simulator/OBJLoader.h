#ifndef MODEL_H
#define MODEL_H

#include <cstddef>
#include <vector>
#include <string>
#include <unistd.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>

using namespace std;

class OBJLoader
{
private:
public:
    OBJLoader();
    ~OBJLoader();


    static bool loadModelData(const char* path, vector <argos::CVector3>& out_vertices,
        vector <argos::CVector2>& out_uvs,
        vector <argos::CVector3>& out_normals) {
        std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
        vector <argos::CVector3> tmp_vertices;
        vector <argos::CVector2> tmp_uvs;
        vector <argos::CVector3> tmp_normals;

        /* Open file */
        FILE* file = fopen(path, "r");
        if (file == NULL) {
            printf("Impossible to open the file !\n");
            return false;
        }
        while (1) {
            char lineHeader[128]; // expect lines not to exceed 128 bytes
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;
            /* Check for vertices */
            if (strcmp(lineHeader, "v") == 0) {
                argos::CVector3 vertex;
                argos::Real coordinates[3];
                fscanf(file, "%lf %lf %lf\n", &coordinates[0], &coordinates[1], &coordinates[2]);
                vertex.Set(coordinates[0], coordinates[1], coordinates[2]);
                tmp_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0) { // UV texture for vertex
                argos::CVector2 uv;
                argos::Real coordinates[2];
                fscanf(file, "%lf %lf\n", &coordinates[0], &coordinates[1]);
                uv.Set(coordinates[0], coordinates[1]);
                tmp_uvs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0) { // normal maps
                argos::CVector3 normal;
                argos::Real coordinates[3];
                fscanf(file, "%lf %lf %lf\n", &coordinates[0], &coordinates[1], &coordinates[2]);
                tmp_normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0) {
                string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
        /* Format indices to CVector3 */
        for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
            unsigned int vertexIndex = vertexIndices[i];
            argos::CVector3 vertex = tmp_vertices[vertexIndex - 1]; // -1 because OBJ indices start at 1
            out_vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < uvIndices.size(); ++i) {
            unsigned int uvIndex = uvIndices[i];
            argos::CVector2 uv = tmp_uvs[uvIndex - 1]; // -1 because OBJ indices start at 1
            out_uvs.push_back(uv);
        }
        for (unsigned int i = 0; i < normalIndices.size(); ++i) {
            unsigned int normalIndex = normalIndices[i];
            argos::CVector3 normal = tmp_normals[normalIndex - 1]; // -1 because OBJ indices start at 1
            out_normals.push_back(normal);
        }
        return true;
    }
};

#endif
