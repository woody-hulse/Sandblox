#pragma once

#include "primitive.h"

class Terrain : public Primitive
{
public:
    Terrain();

    std::vector<float> generateShape() override { return m_vertexData; }
    void updateParams(int param1, int param2) override { generateTerrainMesh(); };

    std::vector<std::vector<float>> generateHeightMap(int m, int n, float scale);
    void generateTerrain();
    void generateTerrainMesh();

    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void insertVec3(std::vector<float> &data, glm::vec3 v);

    static const int sizeX = 100;
    static const int sizeY = 100;
    static const int sizeZ = 20;

    int terrain[sizeX][sizeY][sizeZ];
};
