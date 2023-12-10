#ifndef TERRAIN4_H
#define TERRAIN4_H

#include "terrain.h"

class Terrain4 : public Terrain
{
public:
    Terrain4();
    Terrain4(int sizeX, int sizeY, int sizeZ, int sizeW);

    std::vector<std::vector<std::vector<float>>> generateHeightMap(float scale);
    std::vector<std::vector<std::vector<std::vector<float>>>> generateHeightMap4(float scale);
    std::vector<std::vector<std::vector<std::vector<float>>>> generateSimplexHeightMap4(float scale);
    void addHeightMap(
        std::vector<std::vector<std::vector<float>>>& hm1,
        std::vector<std::vector<std::vector<float>>>& hm2);
    void addHeightMap4(
        std::vector<std::vector<std::vector<std::vector<float>>>>& hm1,
        std::vector<std::vector<std::vector<std::vector<float>>>>& hm2);
    void generateTerrain4();
    void generateTerrain();
    void generateTerrain(Terrain4& terrain2);
    void generateTerrain(bool test);
    void breakBlock(IntersectData& intersectData);
    void placeBlock(IntersectData& intersectData);
    std::vector<IntersectData> terrainRayIntersect(Ray ray);
    std::vector<std::vector<IntersectData>> terrainPlaneIntersect(glm::vec3 a, glm::vec3 b, glm::vec3 pos);
    void rotateCrossSection(float theta, float t);
    void rotateCrossSection(float theta, float t, Terrain4& terrain2);

    Ray crossSection;

    int sizeW = sizeX;

    uint8_t**** terrain4;
    uint8_t**** terrain_p;
    bool**** rendered4;
    std::vector<std::vector<std::vector<float>>> heightMap;
    std::vector<std::vector<std::vector<std::vector<float>>>> heightMap4;

    unsigned char permutation[512];

    void generatePermutation(unsigned char permutation[], int size);

    // Simplex4 implementation inspired by Stefan Gustavson's 4D extension of 3D simplex noise
    float simplex4(float x, float y, float z, float w);
};

#endif // TERRAIN4_H
