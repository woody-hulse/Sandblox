#include "terrain.h"

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>


Terrain::Terrain()
{
    m_vertexData = std::vector<float>();

    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int z = 0; z < sizeZ; z++) {
                terrain[x][y][z] = 0;
            }
        }
    }
}



std::vector<std::vector<float>> Terrain::generateHeightMap(int m, int n, float scale) {
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    std::vector<std::vector<glm::vec2>> gradients(m + 1, std::vector<glm::vec2>(n + 1));
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            gradients[i][j] = glm::normalize(glm::vec2(distribution(generator), distribution(generator)));
        }
    }

    std::vector<std::vector<float>> noise(m, std::vector<float>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
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
    std::vector<std::vector<float>> heightMap = generateHeightMap(sizeX, sizeY, 1.f);

    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            std::cout << heightMap[x][y] << std::endl;
            for (int z = 0; z < sizeZ; z++) {
                if (z < (heightMap[x][y] + 0.7) * sizeZ)
                    terrain[x][y][z] = 1;
            }
        }
    }
}

void Terrain::generateTerrainMesh() {
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int z = 0; z < sizeZ; z++) {
                if (terrain[x][y][z] == 0) continue;

                glm::vec3 shift(x, z, y);
                for (int i = -1; i <= 1; i+=2) {
                    bool drawFace = false;
                    if (x + i >= 0 && x + i < sizeX) {
                        if (terrain[x + i][y][z] == 0) { drawFace = true; }
                    } else drawFace = true;

                    if (drawFace && i == -1)
                        makeFace(glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f,  0.5f) + shift);

                    if (drawFace && i == 1)
                        makeFace(glm::vec3( 0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift);
                }

                for (int j = -1; j <= 1; j+=2) {
                    bool drawFace = false;
                    if (z + j >= 0 && z + j < sizeZ) {
                        if (terrain[x][y][z + j] == 0) { drawFace = true; }
                    } else drawFace = true;

                    if (drawFace && j == -1)
                        makeFace(glm::vec3(-0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift);

                    if (drawFace && j == 1)
                        makeFace(glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f,  0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f,  0.5f) + shift);
                }

                for (int k = -1; k <= 1; k+=2) {
                    bool drawFace = false;
                    if (y + k >= 0 && y + k < sizeY) {
                        if (terrain[x][y + k][z] == 0) { drawFace = true; }
                    } else drawFace = true;

                    if (drawFace && k == -1)
                        makeFace(glm::vec3( 0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f,  0.5f, -0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, -0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, -0.5f) + shift);

                    if (drawFace && k == 1)
                        makeFace(glm::vec3(-0.5f,  0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f,  0.5f, 0.5f) + shift,
                                 glm::vec3(-0.5f, -0.5f, 0.5f) + shift,
                                 glm::vec3( 0.5f, -0.5f, 0.5f) + shift);
                }
            }
        }
    }
}

void Terrain::makeFace(glm::vec3 topLeft,
                       glm::vec3 topRight,
                       glm::vec3 bottomLeft,
                       glm::vec3 bottomRight) {

    glm::vec3 normal1 = -glm::normalize(glm::cross(topLeft - topRight, topLeft - bottomLeft));
    glm::vec3 normal2 = -glm::normalize(glm::cross(bottomRight - bottomLeft, bottomRight - topRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal2);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
}

void Terrain::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
