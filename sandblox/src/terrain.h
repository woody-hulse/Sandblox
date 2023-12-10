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
    float t;
    int blockType;
};

class Terrain : public Primitive
{
public:
    int sizeX = 72;
    int sizeY = sizeX;
    int sizeZ = 36;

    Terrain();
    Terrain(int sizeX, int sizeY, int sizeZ);

    std::vector<float> generateShape() override { return m_vertexData; }
    void updateParams(uint8_t blockType) override { generateTerrainMesh(); };

    std::vector<std::vector<float>> generateHeightMap(int m, int n, float scale);
    void generateTerrain();
    void generateTerrain(std::vector<std::vector<GLint>> a, std::vector<std::vector<GLint>> b);
    void generateTerrainMesh();
    void breakBlock(IntersectData& intersectData);
    void placeBlock(IntersectData& intersectData);

    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  uint8_t blockType);
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void insertVec2(std::vector<float> &data, glm::vec2 v);

    RenderShapeData shapeData;
    uint8_t*** terrain;
    bool*** rendered;
};
