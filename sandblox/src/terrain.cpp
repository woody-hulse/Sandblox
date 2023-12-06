#include "terrain.h"

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <random>

Terrain::Terrain()
{
    m_vertexData = std::vector<float>();

    terrain = new uint8_t**[sizeX];
    rendered = new bool**[sizeX];
    for (int x = 0; x < sizeX; x++) {
        terrain[x] = new uint8_t*[sizeY];
        rendered[x] = new bool*[sizeY];
        for (int y = 0; y < sizeY; y++) {
            terrain[x][y] = new uint8_t[sizeZ];
            rendered[x][y] = new bool[sizeZ];
            for (int z = 0; z < sizeZ; z++) {
                terrain[x][y][z] = 0;
                rendered[x][y][z] = false;
            }
        }
    }
}



std::vector<std::vector<float>> Terrain::generateHeightMap(int m, int n, float scale) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> distribution(-1.f, 1.f);

    std::vector<std::vector<glm::vec2>> gradients(m + 1, std::vector<glm::vec2>(n + 1));
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            gradients[i][j] = glm::normalize(glm::vec2(distribution(generator), distribution(generator)));
        }
    }

    std::vector<std::vector<float>> noise(m, std::vector<float>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float x = static_cast<float>(i) / m * scale;
            float y = static_cast<float>(j) / n * scale;

            int x0 = static_cast<int>(x);
            int x1 = x0 + 1;
            int y0 = static_cast<int>(y);
            int y1 = y0 + 1;

            float u = x - x0;
            float v = y - y0;

            float a = glm::dot(gradients[x0][y0], glm::vec2(u, v));
            float b = glm::dot(gradients[x1][y0], glm::vec2(u - 1.0f, v));
            float c = glm::mix(a, b, glm::smoothstep(0.0f, 1.0f, u));

            float d = glm::dot(gradients[x0][y1], glm::vec2(u, v - 1.0f));
            float e = glm::dot(gradients[x1][y1], glm::vec2(u - 1.0f, v - 1.0f));
            float f = glm::mix(d, e, glm::smoothstep(0.0f, 1.0f, u));

            noise[i][j] = glm::mix(c, f, glm::smoothstep(0.0f, 1.0f, v));
        }
    }

    return noise;
}

void Terrain::generateTerrain() {
    int numLayers = 3;
    float baseScale = 1.0f;

    for (int layer = 0; layer < numLayers; layer++) {
        float scale = baseScale * std::pow(2, layer);
        std::vector<std::vector<float>> heightMap = generateHeightMap(sizeX, sizeY, scale);

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {
                for (int z = 0; z < sizeZ; z++) {
                    if (z < (heightMap[x][y] + 0.3) * sizeZ / 2.f)
                        terrain[x][y][z] = 1;
                }
            }
        }
    }
}

void Terrain::generateTerrainFromHeightMap(float heightMap[sizeX][sizeY]) {
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            int depth = 0;
            for (int z = sizeZ - 1; z >= 0; z--) {
                // std::cout << x << " " << y << " " << z << ": " << heightMap[x][y] << std::endl;
                if (z < (heightMap[x][y] + 0.3) * sizeZ / 2.f) {
                    if (depth == 0)
                        terrain[x][y][z] = 1;
                    else if (depth < 3)
                        terrain[x][y][z] = 2;
                    else
                        terrain[x][y][z] = 3;
                    depth += 1;
                } else {
                    depth = 0;
                    terrain[x][y][z] = 0;
                }
            }
        }
    }
}

void Terrain::generateTerrainMesh() {
    m_vertexData.clear();
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int z = 0; z < sizeZ; z++) {
                if (terrain[x][y][z] == 0) {
                    rendered[x][y][z] = false;
                    continue;
                }

                bool drawBlock = false;

                glm::vec3 shift(x, z, y);
                for (int i = -1; i <= 1; i+=2) {
                    bool drawFace = false;
                    if (x + i >= 0 && x + i < sizeX) {
                        if (terrain[x + i][y][z] == 0) { drawFace = true; }
                    } else drawFace = true;
                    drawBlock = drawBlock | drawFace;

                    if (drawFace && i == -1)
                        makeFace(glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f,  0.5f) + shift,
                                 terrain[x][y][z]);

                    if (drawFace && i == 1)
                        makeFace(glm::vec3( 0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift,
                                 terrain[x][y][z]);
                }

                for (int j = -1; j <= 1; j+=2) {
                    bool drawFace = false;
                    if (z + j >= 0 && z + j < sizeZ) {
                        if (terrain[x][y][z + j] == 0) { drawFace = true; }
                    } else drawFace = true;
                    drawBlock = drawBlock | drawFace;

                    if (drawFace && j == -1)
                        makeFace(glm::vec3(-0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift,
                                 terrain[x][y][z]);

                    if (drawFace && j == 1)
                        makeFace(glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f,  0.5f) + shift,
                                 terrain[x][y][z]);
                }

                for (int k = -1; k <= 1; k+=2) {
                    bool drawFace = false;
                    if (y + k >= 0 && y + k < sizeY) {
                        if (terrain[x][y + k][z] == 0) { drawFace = true; }
                    } else drawFace = true;
                    drawBlock = drawBlock | drawFace;

                    if (drawFace && k == -1)
                        makeFace(glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift,
                                 terrain[x][y][z]);

                    if (drawFace && k == 1)
                        makeFace(glm::vec3(-0.5f,  0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, 0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, 0.5f) + shift,
                                 terrain[x][y][z]);
                }

                if (drawBlock) rendered[x][y][z] = true;
                else rendered[x][y][z] = false;
            }
        }
    }
}

void Terrain::breakBlock(IntersectData& intersectData) {
    terrain[intersectData.x][intersectData.y][intersectData.z] = 0;
    generateTerrainMesh();
}

void Terrain::placeBlock(IntersectData& intersectData) {

    if (intersectData.face == 0) intersectData.x += 1;
    if (intersectData.face == 1) intersectData.x -= 1;
    if (intersectData.face == 2) intersectData.z += 1;
    if (intersectData.face == 3) intersectData.z -= 1;
    if (intersectData.face == 4) intersectData.y += 1;
    if (intersectData.face == 5) intersectData.y -= 1;

    if (intersectData.x >= 0 && intersectData.x < sizeX) {
        if (intersectData.y >= 0 && intersectData.y < sizeY) {
            if (intersectData.z >= 0 && intersectData.z < sizeZ) {
                terrain[intersectData.x][intersectData.y][intersectData.z] = intersectData.blockType;
                generateTerrainMesh();
            }
        }
    }
}

void Terrain::makeFace(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight,
                       uint8_t blockType) {

    glm::vec3 normal1 = -glm::normalize(glm::cross(topLeft - topRight, topLeft - bottomLeft));
    glm::vec3 normal2 = -glm::normalize(glm::cross(bottomRight - bottomLeft, bottomRight - topRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(0.f, 1.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(0.f, 0.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal1);
    insertVec2(m_vertexData, glm::vec2(1.f, 1.f));
    m_vertexData.push_back(blockType);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(1.f, 1.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(0.f, 0.f));
    m_vertexData.push_back(blockType);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
    insertVec2(m_vertexData, glm::vec2(1.f, 0.f));
    m_vertexData.push_back(blockType);
}

void Terrain::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void Terrain::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

