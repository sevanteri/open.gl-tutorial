
#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <math.h>
#include <time.h>

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "util.h"

int main(void)
{

    SDL_Window *win;
    SDL_GLContext ctx;

    initEverything(&win, &ctx);

    float vertices[] = {
    //  Position        Gray   TexCoords
        -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
        0.5f,   0.5f,   1.0f,  1.0f,  0.0f,
        0.5,    -0.5f,  1.0f,  1.0f,  1.0f,
        -0.5f,  -0.5f,  1.0f,  0.0f,  1.0f,
    };
    int vertexSize = 5*sizeof(float);
    int coordSize = 2;
    void* colorOffset = (void*)(2*sizeof(float));
    int colorSize = 1;
    void* texCoordOffset = (void*)(3*sizeof(float));
    int texCoordSize = 2;

    GLuint elements[] = {
        0,1,2,
        2,3,0
    };

    // create and bind vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // crete element buffer object
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(elements), elements, GL_STATIC_DRAW);

    // create texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int w, h;
    unsigned char* image =
        SOIL_load_image("kitty.png", &w, &h, 0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create shaders
    GLuint shaders[] = {
        readShaderFile("vertex.glsl", GL_VERTEX_SHADER),
        readShaderFile("fragment.glsl", GL_FRAGMENT_SHADER)
    };

    // create program and link shaders
    GLuint shaderProgram = createShaderProgram(2, shaders);

    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttr = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttr);
    glVertexAttribPointer(posAttr, coordSize, GL_FLOAT, GL_FALSE,
                          vertexSize, 0);

    GLint colorAttr = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttr);
    glVertexAttribPointer(colorAttr, colorSize, GL_FLOAT, GL_FALSE,
                          vertexSize, colorOffset);

    GLint texCoordAttr = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texCoordAttr);
    glVertexAttribPointer(texCoordAttr, texCoordSize, GL_FLOAT, GL_FALSE,
                          vertexSize, texCoordOffset);


    int quit = 0;
    SDL_Event ev;
    while (!quit) {
        quit = handleEvents(&ev);

        glClearColor(0x0, 0x0, 0x0, 0xFF);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(win);

    }

    glDeleteProgram(shaderProgram);
    deleteShaders(2, shaders);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glDeleteTextures(1, &tex);

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);

    SDL_Quit();
    return 0;
}

