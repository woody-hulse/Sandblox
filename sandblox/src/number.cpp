#include "number.h"
#include <glm/gtx/transform.hpp>

Number::Number() {}

Number::Number(int n) {
    number = n;

    numberMapW[0] = ZERO_W;
    numberMapW[1] = ONE_W;
    numberMapW[2] = TWO_W;
    numberMapW[3] = THREE_W;
    numberMapW[4] = FOUR_W;
    numberMapW[5] = FIVE_W;
    numberMapW[6] = SIX_W;
    //numberMapW[7] = SEVEN_W;
    //numberMapW[8] = EIGHT_W;
    //numberMapW[9] = NINE_W;

    numberMapB[0] = ZERO_B;
    numberMapB[1] = ONE_B;
    numberMapB[2] = TWO_B;
    numberMapB[3] = THREE_B;
    numberMapB[4] = FOUR_B;
    numberMapB[5] = FIVE_B;
    numberMapB[6] = SIX_B;
    //numberMapB[7] = SEVEN_B;
    //numberMapB[8] = EIGHT_B;
    //numberMapB[9] = NINE_B;

    if (number < 0 || number > 9) return;

    SceneMaterial basicMaterial;
    basicMaterial.clear();
    basicMaterial.cAmbient = glm::vec4(0.87f, 0.56f, 0.2f, 1.f);
    basicMaterial.cDiffuse = glm::vec4(1.f);
    basicMaterial.cSpecular = glm::vec4(0.f);
    basicMaterial.shininess = 0.f;

    terrain = Terrain();
    terrain.generateTerrain();
    terrain.generateTerrainMesh();

    ScenePrimitive primitive;
    primitive.type = PrimitiveType::PRIMITIVE_MESH;
    terrain.shapeData.primitive = primitive;
    terrain.shapeData.ctm = glm::mat4(1.f);
    terrain.shapeData.inverseCtm = glm::inverse(terrain.shapeData.ctm);
    terrain.shapeData.primitive.material = basicMaterial;
    terrain.shapeData.numTriangles = terrain.vertexSize() / 9;
    terrain.shapeData.shape = &terrain;
}


std::vector<std::vector<GLint>> ZERO_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> ZERO_B =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 1},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> ONE_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> ONE_B =     {
    {0, 0, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 1},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 1, 1, 1, 1},
    };
std::vector<std::vector<GLint>> TWO_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> TWO_B =     {
    {0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {1, 0, 1, 1, 0, 1},
    {0, 1, 0, 1, 0, 1},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 1, 0},
    {1, 0, 0, 0, 0, 1},
    {0, 1, 1, 1, 1, 0},
    };
std::vector<std::vector<GLint>> THREE_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> THREE_B =     {
    {0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 1},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1},
    {0, 0, 1, 1, 0, 1},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    };
std::vector<std::vector<GLint>> FOUR_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> FOUR_B =     {
    {0, 0, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 1},
    {0, 1, 1, 0, 1, 1},
    {0, 0, 1, 0, 1, 0},
    {0, 0, 1, 1, 1, 0},
    };
std::vector<std::vector<GLint>> FIVE_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> FIVE_B =     {
    {0, 0, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 1},
    {0, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 0, 1},
    {0, 0, 1, 1, 0, 1},
    {0, 0, 1, 1, 0, 1},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    };
std::vector<std::vector<GLint>> SIX_W =     {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0},
    };
std::vector<std::vector<GLint>> SIX_B =     {
    {0, 0, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0},
    };
 /*
std::vector<std::vector<GLint>> SEVEN_W = ;
std::vector<std::vector<GLint>> SEVEN_B = ;
std::vector<std::vector<GLint>> EIGHT_W = ;
std::vector<std::vector<GLint>> EIGHT_B = ;
std::vector<std::vector<GLint>> NINE_W = ;
std::vector<std::vector<GLint>> NINE_B = ;
*/

/*

GLuint createTexture_(const glm::vec4& color) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned char pixelData[4] = {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        static_cast<unsigned char>(color.a * 255)
    };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}


GLuint TEX_WHITE = createTexture_(glm::vec4(1.f));
GLuint TEX_GREY = createTexture_(glm::vec4(0.5f));
GLuint TEX_BLACK = createTexture_(glm::vec4(0.f));


UIOverlay base(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );


UIOverlay zero_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );

UIOverlay zero_b(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 0, 0, 0, 1},
     {0, 1, 0, 0, 0, 1},
     {0, 1, 0, 0, 0, 1},
     {0, 1, 0, 0, 0, 1},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_BLACK, 0, 0, 1, 1
    );




UIOverlay one_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );
UIOverlay one_b(
    {
     {0, 0, 0, 1, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 1, 1, 0, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 1, 0, 1, 1},
     {0, 1, 0, 0, 0, 1},
     {0, 1, 1, 1, 1, 1},
     },
    TEX_BLACK, 0, 0, 1, 1
    );





UIOverlay two_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 0, 0, 1, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0, 0},
     {0, 1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );


UIOverlay two_b(
    {
     {0, 0, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0},
     {1, 0, 1, 1, 0, 1},
     {0, 1, 0, 1, 0, 1},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 0, 1, 1, 0},
     {1, 0, 0, 0, 0, 1},
     {0, 1, 1, 1, 1, 0},
     },
    TEX_BLACK, 0, 0, 1, 1
    );




UIOverlay three_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 1, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );

UIOverlay three_b(
    {
     {0, 0, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 1},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 0, 1, 0, 1},
     {0, 0, 1, 1, 0, 1},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );



UIOverlay four_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 1, 0, 1, 0, 0},
     {0, 1, 1, 1, 1, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );

UIOverlay four_b(
    {
     {0, 0, 0, 1, 1, 0},
     {0, 0, 1, 0, 1, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 0, 0, 1, 1},
     {0, 0, 0, 0, 0, 1},
     {0, 1, 1, 0, 1, 1},
     {0, 0, 1, 0, 1, 0},
     {0, 0, 1, 1, 1, 0},
     },
    TEX_BLACK, 0, 0, 1, 1
    );



UIOverlay five_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0, 0},
     {0, 0, 1, 1, 1, 0},
     {0, 0, 0, 0, 1, 0},
     {0, 0, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );

UIOverlay five_b(
    {
     {0, 0, 1, 1, 1, 0},
     {0, 1, 0, 0, 0, 1},
     {0, 1, 0, 1, 1, 1},
     {0, 1, 0, 0, 0, 1},
     {0, 0, 1, 1, 0, 1},
     {0, 0, 1, 1, 0, 1},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     },
    TEX_BLACK, 0, 0, 1, 1
    );



UIOverlay six_w(
    {
     {0, 0, 0, 0, 0, 0},
     {0, 0, 1, 1, 1, 0},
     {0, 1, 0, 0, 0, 0},
     {0, 1, 1, 1, 0, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0, 0},
     },
    TEX_WHITE, 0, 0, 1, 1
    );

UIOverlay six_b(
    {
     {0, 0, 1, 1, 1, 0},
     {0, 1, 0, 0, 0, 1},
     {1, 0, 1, 1, 1, 0},
     {1, 0, 0, 0, 1, 0},
     {1, 0, 0, 0, 0, 1},
     {1, 0, 0, 0, 0, 1},
     {0, 1, 0, 0, 1, 0},
     {0, 0, 1, 1, 0, 0},
     },
    TEX_BLACK, 0, 0, 1, 1
    );
*/
