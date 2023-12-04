#pragma once

#include "utils/sceneparser.h"
#include "primitive.h"


struct Ray {
    glm::vec4 origin;
    glm::vec4 direction;
};

struct IntersectData {
    bool intersection;
    int x;
    int y;
    int z;
    int face;
};

class Terrain : public Primitive
{
public:
    Terrain();

    std::vector<float> generateShape() override { return m_vertexData; }
    void updateParams(int param1, int param2) override { generateTerrainMesh(); };

    std::vector<std::vector<float>> generateHeightMap(int m, int n, float scale);
    void generateTerrain();
    void generateTerrainMesh();
    void breakBlock(IntersectData& intersectData);
    void placeBlock(IntersectData& intersectData);

    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void insertVec3(std::vector<float> &data, glm::vec3 v);

    static const int sizeX = 100;
    static const int sizeY = 100;
    static const int sizeZ = 50;

    RenderShapeData shapeData;
    uint8_t terrain[sizeX][sizeY][sizeZ];
    bool rendered[sizeX][sizeY][sizeZ];
};
