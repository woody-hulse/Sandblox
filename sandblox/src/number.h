#ifndef NUMBER_H
#define NUMBER_H

#include "cube.h"
#include "ui.h"
#include "terrain.h"
#include <glm/glm.hpp>
#include <map>

GLuint createTexture(const glm::vec4& color);

extern GLuint TEX_WHITE;
extern GLuint TEX_GREY;
extern GLuint TEX_BLACK;

extern std::vector<std::vector<GLint>> ZERO_W;
extern std::vector<std::vector<GLint>> ZERO_B;
extern std::vector<std::vector<GLint>> ONE_W;
extern std::vector<std::vector<GLint>> ONE_B;
extern std::vector<std::vector<GLint>> TWO_W;
extern std::vector<std::vector<GLint>> TWO_B;
extern std::vector<std::vector<GLint>> THREE_W;
extern std::vector<std::vector<GLint>> THREE_B;
extern std::vector<std::vector<GLint>> FOUR_W;
extern std::vector<std::vector<GLint>> FOUR_B;
extern std::vector<std::vector<GLint>> FIVE_W;
extern std::vector<std::vector<GLint>> FIVE_B;
extern std::vector<std::vector<GLint>> SIX_W;
extern std::vector<std::vector<GLint>> SIX_B;
/*
extern std::vector<std::vector<GLint>> SEVEN_W;
extern std::vector<std::vector<GLint>> SEVEN_B;
extern std::vector<std::vector<GLint>> EIGHT_W;
extern std::vector<std::vector<GLint>> EIGHT_B;
extern std::vector<std::vector<GLint>> NINE_W;
extern std::vector<std::vector<GLint>> NINE_B;
*/

extern UIOverlay base;
extern UIOverlay zero_w;
extern UIOverlay zero_b;
extern UIOverlay one_w;
extern UIOverlay one_b;
extern UIOverlay two_w;
extern UIOverlay two_b;
extern UIOverlay three_w;
extern UIOverlay three_b;
extern UIOverlay four_w;
extern UIOverlay four_b;
extern UIOverlay five_w;
extern UIOverlay five_b;
extern UIOverlay six_w;
extern UIOverlay six_b;
extern UIOverlay seven_w;
extern UIOverlay seven_b;
extern UIOverlay eight_w;
extern UIOverlay eight_b;
extern UIOverlay nine_w;
extern UIOverlay nine_b;

class Number {
public:
    Number();
    Number(int n);

    int number;
    std::vector<std::vector<GLint>> whiteVector;
    std::vector<std::vector<GLint>> blackVector;
    UIElement white;
    UIElement black;
    Terrain terrain;

    std::map<int, std::vector<std::vector<GLint>>> numberMapW;
    std::map<int, std::vector<std::vector<GLint>>> numberMapB;
};

#endif // NUMBER_H
