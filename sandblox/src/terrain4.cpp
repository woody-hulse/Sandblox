#include "terrain4.h"

#include "glm/gtx/string_cast.hpp"

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <random>
#include <algorithm>

static unsigned char simplex[64][4] = {
    {0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
    {0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
    {1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
    {2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};

void Terrain4::generatePermutation(unsigned char permutation[], int size) {
    for (int i = 0; i < size; i++) {
        permutation[i] = (unsigned char)i;
    }

    // fisher-yates shuffle algorithm
    srand((unsigned int)time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int j = arc4random() % (i + 1);

        unsigned char temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }
}


Terrain4::Terrain4() : Terrain()
{
    m_vertexData = std::vector<float>();

    terrain4 = new uint8_t***[sizeX];
    terrain_p = new uint8_t***[sizeX];
    rendered4 = new bool***[sizeX];
    for (int x = 0; x < sizeX; x++) {
        terrain4[x] = new uint8_t**[sizeY];
        terrain_p[x] = new uint8_t**[sizeY];
        rendered4[x] = new bool**[sizeY];
        for (int y = 0; y < sizeY; y++) {
            terrain4[x][y] = new uint8_t*[sizeZ];
            terrain_p[x][y] = new uint8_t*[sizeZ];
            rendered4[x][y] = new bool*[sizeZ];
            for (int z = 0; z < sizeZ; z++) {
                terrain4[x][y][z] = new uint8_t[sizeW];
                rendered4[x][y][z] = new bool[sizeW];
                for (int w = 0; w < sizeW; w++) {
                    terrain4[x][y][z][w] = 0;
                    rendered4[x][y][z][w] = false;
                }
                terrain_p[x][y][z] = &terrain4[x][y][z][0];
            }
        }
    }

    crossSection.origin = glm::vec4(sizeX / 2.f, sizeW / 2.f, 0.f, 1.f);
    crossSection.direction = glm::normalize(glm::vec4(1.f, 0.f, 0.f, 0.f));

    generatePermutation(permutation, 512);
}

Terrain4::Terrain4(int sizeX, int sizeY, int sizeZ, int sizeW) {
    m_vertexData = std::vector<float>();

    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->sizeZ = sizeZ;
    this->sizeW = sizeW;

    m_vertexData = std::vector<float>();

    terrain = new uint8_t**[sizeX];
    rendered = new bool**[sizeX];

    terrain4 = new uint8_t***[sizeX];
    terrain_p = new uint8_t***[sizeX];
    rendered4 = new bool***[sizeX];
    for (int x = 0; x < sizeX; x++) {
        terrain[x] = new uint8_t*[sizeY];
        rendered[x] = new bool*[sizeY];

        terrain4[x] = new uint8_t**[sizeY];
        terrain_p[x] = new uint8_t**[sizeY];
        rendered4[x] = new bool**[sizeY];
        for (int y = 0; y < sizeY; y++) {
            terrain[x][y] = new uint8_t[sizeZ];
            rendered[x][y] = new bool[sizeZ];

            terrain4[x][y] = new uint8_t*[sizeZ];
            terrain_p[x][y] = new uint8_t*[sizeZ];
            rendered4[x][y] = new bool*[sizeZ];
            for (int z = 0; z < sizeZ; z++) {
                terrain[x][y][z] = 0;
                rendered[x][y][z] = false;

                terrain4[x][y][z] = new uint8_t[sizeW];
                rendered4[x][y][z] = new bool[sizeW];
                for (int w = 0; w < sizeW; w++) {
                    terrain[x][y][z] = 0;
                    rendered[x][y][z] = false;
                    terrain4[x][y][z][w] = 0;
                    rendered4[x][y][z][w] = false;
                }
                terrain_p[x][y][z] = &terrain4[x][y][z][0];
            }
        }
    }

    crossSection.origin = glm::vec4(sizeX / 2.f, sizeW / 2.f, 0.f, 1.f);
    crossSection.direction = glm::normalize(glm::vec4(1.f, 0.f, 0.f, 0.f));

    generatePermutation(permutation, 512);
}


std::vector<std::vector<std::vector<float>>> Terrain4::generateHeightMap(float scale) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> distribution(-1.f, 1.f);

    std::vector<std::vector<std::vector<glm::vec3>>> gradients(sizeX + 1,
                                                               std::vector<std::vector<glm::vec3>>(sizeY + 1,
                                                               std::vector<glm::vec3>(sizeZ + 1)));
    for (int i = 0; i <= sizeX; i++) {
        for (int j = 0; j <= sizeY; j++) {
            for (int k = 0; k <= sizeZ; k++) {
                gradients[i][j][k] = glm::normalize(glm::vec3(distribution(generator), distribution(generator), distribution(generator)));
            }
        }
    }

    std::vector<std::vector<std::vector<float>>> noise(sizeX,
                                                       std::vector<std::vector<float>>(sizeY,
                                                       std::vector<float>(sizeZ)));
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                float x = static_cast<float>(i) / sizeX * scale;
                float y = static_cast<float>(j) / sizeY * scale;
                float z = static_cast<float>(k) / sizeZ * scale;

                int x0 = static_cast<int>(x);
                int x1 = x0 + 1;
                int y0 = static_cast<int>(y);
                int y1 = y0 + 1;
                int z0 = static_cast<int>(z);
                int z1 = z0 + 1;

                float u = x - x0;
                float v = y - y0;
                float w = z - z0;

                float x00 = glm::mix(glm::dot(gradients[x0][y0][z0], glm::vec3(u, v, w)),
                                     glm::dot(gradients[x1][y0][z0], glm::vec3(u - 1.0f, v, w)), u);
                float x10 = glm::mix(glm::dot(gradients[x0][y1][z0], glm::vec3(u, v - 1.0f, w)),
                                     glm::dot(gradients[x1][y1][z0], glm::vec3(u - 1.0f, v - 1.0f, w)), u);
                float x01 = glm::mix(glm::dot(gradients[x0][y0][z1], glm::vec3(u, v, w - 1.0f)),
                                     glm::dot(gradients[x1][y0][z1], glm::vec3(u - 1.0f, v, w - 1.0f)), u);
                float x11 = glm::mix(glm::dot(gradients[x0][y1][z1], glm::vec3(u, v - 1.0f, w - 1.0f)),
                                     glm::dot(gradients[x1][y1][z1], glm::vec3(u - 1.0f, v - 1.0f, w - 1.0f)), u);

                float a0 = glm::mix(x00, x10, v);
                float a1 = glm::mix(x01, x11, v);

                noise[i][j][k] = glm::mix(a0, a1, w);
            }
        }
    }
    return noise;
}

float grad(int hash, float x, float y, float z, float t) {
    int h = hash & 31;
    float u = h<24 ? x : y;
    float v = h<16 ? y : z;
    float w = h<8 ? z : t;
    return ((h&1)? -u : u) + ((h&2)? -v : v) + ((h&4)? -w : w);
}

float Terrain4::simplex4(float x, float y, float z, float w) {

    float F4 = (sqrt(5.0) - 1.0) / 4.0;
    float G4 = (5.0 - sqrt(5.0)) / 20.0;

    float X[5]; int i[5];
    float Y[5]; int j[5];
    float Z[5]; int k[5];
    float W[5]; int l[5];

    // Get simplex cell indices
    float s = (x + y + z + w) * F4;
    float xs = x + s;
    float ys = y + s;
    float zs = z + s;
    float ws = w + s;
    int i_ = floor(xs);
    int j_ = floor(ys);
    int k_ = floor(zs);
    int l_ = floor(ws);

    // Compute simplex corners
    float t = (i_ + j_ + k_ + l_) * G4;
    X[0] = x - i_ + t;
    Y[0] = y - j_ + t;
    Z[0] = z - k_ + t;
    W[0] = w - l_ + t;

    // Simplex char lookup
    int c1 = (X[0] > Y[0]) ? 32 : 0;
    int c2 = (X[0] > Z[0]) ? 16 : 0;
    int c3 = (Y[0] > Z[0]) ? 8 : 0;
    int c4 = (X[0] > W[0]) ? 4 : 0;
    int c5 = (Y[0] > W[0]) ? 2 : 0;
    int c6 = (Z[0] > W[0]) ? 1 : 0;
    int c = c1 + c2 + c3 + c4 + c5 + c6;

    // Compute corners based on offsets from simplex lookup
    i[0] = 0; i[4] = 1;
    j[0] = 0; j[4] = 1;
    k[0] = 0; k[4] = 1;
    l[0] = 0; l[4] = 1;
    for (int a = 1; a < 5; a++) {
        if (a != 4) {
            i[a] = simplex[c][0] >= 4 - a ? 1 : 0;
            j[a] = simplex[c][1] >= 4 - a ? 1 : 0;
            k[a] = simplex[c][2] >= 4 - a ? 1 : 0;
            l[a] = simplex[c][3] >= 4 - a ? 1 : 0;
        }

        X[a] = X[0] + a * G4 - i[a];
        Y[a] = Y[0] + a * G4 - j[a];
        Z[a] = Z[0] + a * G4 - k[a];
        W[a] = W[0] + a * G4 - l[a];
    }

    // Compute weights of each corner and apply gradient
    float n[5];
    for (int a = 0; a < 5; a++) {
        float ta = 0.6f - (X[a]*X[a] + Y[a]*Y[a] + Z[a]*Z[a] + W[a]*W[a]);
        if (ta < 0.f) n[a] = 0.f;
        else n[a] = ta*ta*ta*ta * grad(permutation[i_ % 256 + i[a] +
                                       permutation[j_ % 256 + j[a] +
                                       permutation[k_ % 256 + k[a] +
                                       permutation[l_ % 256 + l[a]]]]], X[a], Y[a], Z[a], W[a]);
    }

    return (n[0] + n[1] + n[2] + n[3] + n[4]) * 27.f;
}

std::vector<std::vector<std::vector<std::vector<float>>>> Terrain4::generateSimplexHeightMap4(float scale) {
    std::vector<std::vector<std::vector<std::vector<float>>>> noise(
        sizeX, std::vector<std::vector<std::vector<float>>>(
            sizeY, std::vector<std::vector<float>>(
                sizeZ, std::vector<float>(sizeW)
                )
            )
        );

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                for (int l = 0; l < sizeW; l++) {
                    float x = (float)i / sizeX * scale;
                    float y = (float)j / sizeY * scale;
                    float z = (float)k / sizeZ * scale;
                    if (sizeZ == 8) z = (float)k / 16 * scale;
                    float w = (float)l / sizeW * scale;
                    noise[i][j][k][l] = simplex4(x, y, z, w);
                }
            }
        }
    }

    return noise;
}

std::vector<std::vector<std::vector<std::vector<float>>>> Terrain4::generateHeightMap4(float scale) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> distribution(-0.5f, 0.5f);

    std::vector<std::vector<std::vector<std::vector<glm::vec4>>>> gradients(
        sizeX + 1, std::vector<std::vector<std::vector<glm::vec4>>>(
            sizeY + 1, std::vector<std::vector<glm::vec4>>(
                sizeZ + 1, std::vector<glm::vec4>(sizeW + 1)
                )
            )
        );

    for (int i = 0; i <= sizeX; i++) {
        for (int j = 0; j <= sizeY; j++) {
            for (int k = 0; k <= sizeZ; k++) {
                for (int l = 0; l <= sizeW; l++) {
                    gradients[i][j][k][l] = glm::normalize(
                        glm::vec4(distribution(generator), distribution(generator), distribution(generator), distribution(generator))
                        );
                }
            }
        }
    }

    std::vector<std::vector<std::vector<std::vector<float>>>> noise(
        sizeX, std::vector<std::vector<std::vector<float>>>(
            sizeY, std::vector<std::vector<float>>(
                sizeZ, std::vector<float>(sizeW)
                )
            )
        );

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                for (int l = 0; l < sizeW; l++) {
                    float x = static_cast<float>(i) / sizeX * scale;
                    float y = static_cast<float>(j) / sizeY * scale;
                    float z = static_cast<float>(k) / sizeZ * scale;
                    float w = static_cast<float>(l) / sizeW * scale;

                    int x0 = static_cast<int>(x);
                    int x1 = x0 + 1;
                    int y0 = static_cast<int>(y);
                    int y1 = y0 + 1;
                    int z0 = static_cast<int>(z);
                    int z1 = z0 + 1;
                    int w0 = static_cast<int>(w);
                    int w1 = w0 + 1;

                    float u = x - x0;
                    float v = y - y0;
                    float s = z - z0;
                    float t = w - w0;

                    // bug here :(

                    float x00 = glm::mix(glm::mix(glm::mix(
                                                      glm::dot(gradients[x0][y0][z0][w0], glm::vec4(u, v, s, t)),
                                                      glm::dot(gradients[x1][y0][z0][w0], glm::vec4(u - 1.0f, v, s, t)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z0][w0], glm::vec4(u, v - 1.0f, s, t)),
                                                           glm::dot(gradients[x1][y1][z0][w0], glm::vec4(u - 1.0f, v - 1.0f, s, t)), u), v),
                                         glm::mix(glm::mix(glm::dot(gradients[x0][y0][z1][w0], glm::vec4(u, v, s - 1.0f, t)),
                                                           glm::dot(gradients[x1][y0][z1][w0], glm::vec4(u - 1.0f, v, s - 1.0f, t)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w0], glm::vec4(u, v - 1.0f, s - 1.0f, t)),
                                                           glm::dot(gradients[x1][y1][z1][w0], glm::vec4(u - 1.0f, v - 1.0f, s - 1.0f, t)), u), v), w);

                    float x10 = glm::mix(glm::mix(glm::mix(
                                                      glm::dot(gradients[x0][y0][z0][w1], glm::vec4(u, v, s, t - 1.0f)),
                                                      glm::dot(gradients[x1][y0][z0][w1], glm::vec4(u - 1.0f, v, s, t - 1.0f)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z0][w1], glm::vec4(u, v - 1.0f, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y1][z0][w1], glm::vec4(u - 1.0f, v - 1.0f, s, t - 1.0f)), u), v),
                                         glm::mix(glm::mix(glm::dot(gradients[x0][y0][z1][w1], glm::vec4(u, v, s - 1.0f, t - 1.0f)),
                                                           glm::dot(gradients[x1][y0][z1][w1], glm::vec4(u - 1.0f, v, s - 1.0f, t - 1.0f)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w1], glm::vec4(u, v - 1.0f, s - 1.0f, t - 1.0f)),
                                                           glm::dot(gradients[x1][y1][z1][w1], glm::vec4(u - 1.0f, v - 1.0f, s - 1.0f, t - 1.0f)), u), v), w);

                    float x01 = glm::mix(glm::mix(glm::mix(
                                                      glm::dot(gradients[x0][y0][z1][w0], glm::vec4(u, v, s, t)),
                                                      glm::dot(gradients[x1][y0][z1][w0], glm::vec4(u - 1.0f, v, s, t)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w0], glm::vec4(u, v - 1.0f, s, t)),
                                                           glm::dot(gradients[x1][y1][z1][w0], glm::vec4(u - 1.0f, v - 1.0f, s, t)), u), v),
                                         glm::mix(glm::mix(glm::dot(gradients[x0][y0][z1][w1], glm::vec4(u, v, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y0][z1][w1], glm::vec4(u - 1.0f, v, s, t - 1.0f)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w1], glm::vec4(u, v - 1.0f, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y1][z1][w1], glm::vec4(u - 1.0f, v - 1.0f, s, t - 1.0f)), u), v), w);

                    float x11 = glm::mix(glm::mix(glm::mix(
                                                      glm::dot(gradients[x0][y0][z1][w1], glm::vec4(u, v, s, t - 1.0f)),
                                                      glm::dot(gradients[x1][y0][z1][w1], glm::vec4(u - 1.0f, v, s, t - 1.0f)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w1], glm::vec4(u, v - 1.0f, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y1][z1][w1], glm::vec4(u - 1.0f, v - 1.0f, s, t - 1.0f)), u), v),
                                         glm::mix(glm::mix(glm::dot(gradients[x0][y0][z1][w1], glm::vec4(u, v, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y0][z1][w1], glm::vec4(u - 1.0f, v, s, t - 1.0f)), u),
                                                  glm::mix(glm::dot(gradients[x0][y1][z1][w1], glm::vec4(u, v - 1.0f, s, t - 1.0f)),
                                                           glm::dot(gradients[x1][y1][z1][w1], glm::vec4(u - 1.0f, v - 1.0f, s, t - 1.0f)), u), v), w);

                    float a0 = glm::mix(x00, x10, v);
                    float a1 = glm::mix(x01, x11, v);

                    float b0 = glm::mix(a0, a1, w);

                    noise[i][j][k][l] = b0;
                }
            }
        }
    }

    return noise;
}


void Terrain4::addHeightMap(
    std::vector<std::vector<std::vector<float>>>& hm1,
    std::vector<std::vector<std::vector<float>>>& hm2) {
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int w = 0; w < sizeZ; w++) {
                hm1[x][y][w] += hm2[x][y][w];
            }
        }
    }
}


void Terrain4::addHeightMap4(
    std::vector<std::vector<std::vector<std::vector<float>>>>& hm1,
    std::vector<std::vector<std::vector<std::vector<float>>>>& hm2) {
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int z = 0; z < sizeZ; z++) {
                for (int w = 0; w < sizeW; w++) {
                    hm1[x][y][z][w] += hm2[x][y][z][w];
                }
            }
        }
    }
}

void Terrain4::generateTerrain4() {
    int numLayers = 2; // 3
    float baseScale = 2.f; // 1.f

    //heightMap = generateHeightMap(baseScale);
    //heightMap4 = generateHeightMap4(baseScale);
    generatePermutation(permutation, 512);
    heightMap4 = generateSimplexHeightMap4(baseScale);
    for (int layer = 1; layer < numLayers; layer++) {
        //float scale = baseScale * std::pow(2, layer);
        //std::vector<std::vector<std::vector<std::vector<float>>>> hm2 = generateHeightMap4(scale);
        //addHeightMap4(heightMap4, hm2);

        //std::vector<std::vector<std::vector<float>>> hm2 = generateHeightMap(scale);
        //addHeightMap(heightMap, hm2);

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {
                for (int w = 0; w < sizeW; w++) {
                    int depth = 0;
                    for (int z = sizeZ - 1; z >= 0; z--) {
                        float height = heightMap4[x][y][z][w]; // do we want to fix this?
                        float val = (height + 0.5) * sizeZ;
                        if (z < val) {
                            if (depth == 0) {
                                terrain4[x][y][z][w] = 1;
                            } else if (depth < 3) {
                                terrain4[x][y][z][w] = 2;
                            } else {
                                int a = 50;
                                if (arc4random() % (sizeZ * a) > sizeZ * a - depth / 5.f) // fix
                                    terrain4[x][y][z][w] = 4;
                                else terrain4[x][y][z][w] = 3;
                            }
                            depth += 1;
                        } else {
                            depth = 0;
                            terrain4[x][y][z][w] = 0;
                        }

                        if (sizeZ == 8) {
                            if (abs(height) > 0.6)
                                terrain4[x][y][z][w] = 1;
                            else terrain4[x][y][z][w] = 0;
                        }
                    }
                }
            }
        }
    }
}



void Terrain4::rotateCrossSection(float theta, float t) {
    float newX = crossSection.direction.x * cos(theta) - crossSection.direction.y * sin(theta);
    float newY = crossSection.direction.x * sin(theta) + crossSection.direction.y * cos(theta);
    crossSection.direction = glm::vec4(newX, newY, 0.f, 0.f);

    generateTerrain();
}

void Terrain4::rotateCrossSection(float theta, float t, Terrain4& terrain2) {
    float newX = crossSection.direction.x * cos(theta) - crossSection.direction.y * sin(theta);
    float newY = crossSection.direction.x * sin(theta) + crossSection.direction.y * cos(theta);
    crossSection.direction = glm::vec4(newX, newY, 0.f, 0.f);

    generateTerrain(terrain2);
}


void Terrain4::breakBlock(IntersectData& intersectData) {
    intersectData.blockType = terrain[intersectData.x][intersectData.y][intersectData.z];
    terrain[intersectData.x][intersectData.y][intersectData.z] = 0;
    *terrain_p[intersectData.x][intersectData.y][intersectData.z] = 0;
    generateTerrainMesh();
}

void Terrain4::placeBlock(IntersectData& intersectData) {

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
                *terrain_p[intersectData.x][intersectData.y][intersectData.z] = intersectData.blockType;
                generateTerrainMesh();
            }
        }
    }
}


std::vector<IntersectData> Terrain4::terrainRayIntersect(Ray ray) {
    std::vector<IntersectData> intersections;

    float x = ray.origin.x;
    float y = ray.origin.y;

    glm::vec2 pos(x, y);

    float dx = ray.direction.x;
    float dy = ray.direction.y;

    float epsilon = 0.01f;
    float step = fmin(1 / (abs(dx) + epsilon), 1 / (abs(dy) + epsilon));

    glm::vec2 dir = glm::vec2(dx, dy) * step;

    while (0 <= pos.x && int(pos.x) < sizeX && 0 <= pos.y && int(pos.y) < sizeW) {
        IntersectData data;
        data.x = int(pos.x);
        data.y = int(pos.y);
        intersections.push_back(data);

        pos += dir;
    }

    return intersections;
}


std::vector<std::vector<IntersectData>> Terrain4::terrainPlaneIntersect(glm::vec3 a, glm::vec3 b, glm::vec3 p) {
    std::vector<std::vector<IntersectData>> intersections(sizeX, std::vector<IntersectData>(sizeY));

    glm::vec3 normal = glm::cross(a, b);
    int count = 0;
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int w = 0; w < sizeW; w++) {
                glm::vec3 center(x + 0.5f, y + 0.5f, w + 0.5f);
                if (abs(glm::dot(glm::normalize(center - p), normal)) < 0.01f)
                    count++;
            }
        }
    }

    std::cout << count <<  " " << sizeX * sizeZ << std::endl;

    return intersections;
}


void Terrain4::generateTerrain(bool test) {


}


void Terrain4::generateTerrain() {

    glm::vec3 p = glm::vec3(sizeX / 2.f, sizeY / 2.f, sizeZ / 2.f);
    glm::vec3 a = glm::normalize(crossSection.direction);
    glm::vec3 b = glm::normalize(glm::vec3(crossSection.origin) - p);
    //terrainPlaneIntersect(a, b, p);


    std::vector<IntersectData> pos = terrainRayIntersect(crossSection);
    crossSection.direction = -crossSection.direction;

    std::vector<IntersectData> neg = terrainRayIntersect(crossSection);
    neg.pop_back();
    crossSection.direction = -crossSection.direction;

    int cellX = int(crossSection.origin.x);
    int cellY = int(crossSection.origin.y);

    for (int i = cellX; i >= 0; i--) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                if (cellX - i < neg.size()) {
                    terrain[i][j][k] = terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y]; // fix?
                    terrain_p[i][j][k] = &terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y];
                } else terrain[i][j][k] = 0; // fix
            }
        }
    }

    for (int i = cellX + 1; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                if (i - cellX - 1 < pos.size()) {
                    terrain[i][j][k] = terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];
                    terrain_p[i][j][k] = &terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];
                } else terrain[i][j][k] = 0;
            }
        }
    }
}

void Terrain4::generateTerrain(Terrain4& terrain2) {

    glm::vec3 p = glm::vec3(sizeX / 2.f, sizeY / 2.f, sizeZ / 2.f);
    glm::vec3 a = glm::normalize(crossSection.direction);
    glm::vec3 b = glm::normalize(glm::vec3(crossSection.origin) - p);
    //terrainPlaneIntersect(a, b, p);


    std::vector<IntersectData> pos = terrainRayIntersect(crossSection);
    crossSection.direction = -crossSection.direction;

    std::vector<IntersectData> neg = terrainRayIntersect(crossSection);
    neg.pop_back();
    crossSection.direction = -crossSection.direction;

    int cellX = int(crossSection.origin.x);
    int cellY = int(crossSection.origin.y);

    for (int i = cellX; i >= 0; i--) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                if (cellX - i < neg.size()) {
                    terrain[i][j][k] = terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y]; // fix?
                    terrain_p[i][j][k] = &terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y];

                    if (k < terrain2.sizeZ) {
                        terrain2.terrain[i][j][k] = terrain2.terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y];
                        terrain2.terrain_p[i][j][k] = &terrain2.terrain4[neg[cellX - i].x][j][k][neg[cellX - i].y];
                    }
                } else {
                    terrain[i][j][k] = 0; // fix
                    if (k < terrain2.sizeZ) terrain2.terrain[i][j][k] = 0;
                }
            }
        }
    }

    for (int i = cellX + 1; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                if (i - cellX - 1 < pos.size()) {
                    terrain[i][j][k] = terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];
                    terrain_p[i][j][k] = &terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];

                    if (k < terrain2.sizeZ) {
                        terrain2.terrain[i][j][k] = terrain2.terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];
                        terrain2.terrain_p[i][j][k] = &terrain2.terrain4[pos[i - cellX - 1].x][j][k][pos[i - cellX - 1].y];
                    }
                } else {
                    terrain[i][j][k] = 0;
                    if (k < terrain2.sizeZ) terrain2.terrain[i][j][k] = 0;
                }
            }
        }
    }
}



