#ifndef TERRAIN4_H
#define TERRAIN4_H

#include "terrain.h"

class Terrain4 : public Terrain
{
public:
    Terrain4();

    std::vector<std::vector<std::vector<float>>> generateHeightMap(float scale);
    std::vector<std::vector<std::vector<std::vector<float>>>> generateHeightMap4(float scale);
    void addHeightMap(
        std::vector<std::vector<std::vector<float>>>& hm1,
        std::vector<std::vector<std::vector<float>>>& hm2);
    void addHeightMap4(
        std::vector<std::vector<std::vector<std::vector<float>>>>& hm1,
        std::vector<std::vector<std::vector<std::vector<float>>>>& hm2);
    void generateTerrain4();
    void generateTerrain();
    void breakBlock(IntersectData& intersectData);
    void placeBlock(IntersectData& intersectData);
    std::vector<IntersectData> terrainRayIntersect();
    void rotateCrossSection(float theta, float t);

    Ray crossSection;

    static const int sizeW = sizeX;

    uint8_t**** terrain4;
    uint8_t**** terrain_p;
    bool**** rendered4;
    std::vector<std::vector<std::vector<float>>> heightMap;
    std::vector<std::vector<std::vector<std::vector<float>>>> heightMap4;
};

#endif // TERRAIN4_H
