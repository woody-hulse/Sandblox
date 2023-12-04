#ifndef TERRAIN4_H
#define TERRAIN4_H

#include "terrain.h"

class Terrain4 : public Terrain
{
public:
    Terrain4();

    std::vector<std::vector<std::vector<float>>> generateHeightMap(float scale);

    Ray crossSection;

    static const int sizeX = 30;
    static const int sizeY = 30;
    static const int sizeZ = 30;
    static const int sizeW = 30;

    int terrain[sizeX][sizeY][sizeZ][sizeW];
    bool rendered[sizeX][sizeY][sizeZ][sizeW];
};

#endif // TERRAIN4_H
