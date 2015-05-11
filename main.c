
#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <math.h>

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <graphene-1.0/graphene.h>

#include "common.h"

#include "inotifyWatch.h"

Uint32 GL_RELOAD_FRAG;

void*
fragReloadFunc(void* arg) {
    SDL_Event ev;
    SDL_zero(ev);
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return NULL;
}

int main(void)
{

    SDL_Window *win;
    SDL_GLContext ctx;

    initEverything(&win, &ctx);

    Uint32 GL_RELOAD_FRAG = SDL_RegisterEvents(1);
    if (GL_RELOAD_FRAG == (Uint32) -1)
        err(1, "Couldn't register SDL_Event");

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

    // create shaders
    GLuint shaders[] = {
        compileShaderFile("vertex.glsl", GL_VERTEX_SHADER),
        compileShaderFile("fragment.glsl", GL_FRAGMENT_SHADER)
    };

    GLuint fragmentShader = shaders[1];

    // create program and link shaders
    GLuint shaderProgram = createShaderProgram(2, shaders);


    // specify the layout of the vertex data

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

    // time uniform
    GLuint uniTime = glGetUniformLocation(shaderProgram, "time");


    // translation stuff
    GLuint uniTrans = glGetUniformLocation(shaderProgram, "model");

    graphene_matrix_t model;
    graphene_matrix_init_identity(&model);

    graphene_vec3_t rotAxis;
    graphene_vec3_init(&rotAxis, 0, 0, 1);

    // 3D stuff!!
    GLuint uniView = glGetUniformLocation(shaderProgram, "view");
    GLuint uniProj = glGetUniformLocation(shaderProgram, "proj");

    graphene_matrix_t view;
    graphene_vec3_t eye;
    graphene_vec3_t center;
    graphene_vec3_t up;
    graphene_matrix_init_look_at(
        &view,
        graphene_vec3_init(&eye, 1.2, 1.2, 1.2),
        graphene_vec3_init(&center, 0, 0, 0),
        graphene_vec3_init(&up, 0, 0, 1.0)
    );
    glUniformMatrix4fv(uniView, 1, GL_FALSE, (float*)&view);

    graphene_matrix_t proj;
    graphene_matrix_init_perspective(&proj, 45.0, 800.0/600.0,  1.0, 10.0);
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, (float*)&proj);


    // create textures
#define TEXCOUNT 1
    GLuint tex[TEXCOUNT];
    glGenTextures(TEXCOUNT, tex);

    char *textures[] = {
        "kitty.png",
    };
    char *textureLoc[] = {
        "texKitty",
    };

    loadTextures(textures, tex, TEXCOUNT);
    pointTextures(shaderProgram, textureLoc, tex, TEXCOUNT);

    // fragment shader autoloading
    watcher_t wat = {0};
    watchFile(&wat, "fragment.glsl", fragReloadFunc, NULL);

    int quit = 0;
    SDL_Event ev;
    float dt = 0;
    float lasttime = 0;
    float uptime = 0;
    while (!quit) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_WINDOWEVENT:
                if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
                    resizeWindow(&ev.window);
                break;
            case SDL_USEREVENT:
                reloadShader(fragmentShader, "fragment.glsl", shaderProgram);
                pointTextures(shaderProgram, textureLoc, tex, TEXCOUNT);
                break;
            case SDL_KEYUP:
                if (ev.key.keysym.sym == SDLK_ESCAPE)
                    return 1;
                else if (ev.key.keysym.sym == SDLK_RETURN) {
                    reloadShader(fragmentShader, "fragment.glsl", shaderProgram);
                    pointTextures(shaderProgram, textureLoc, tex, TEXCOUNT);
                }
                break;
            default:
                break;
            }
        }

        glClearColor(0x0, 0x0, 0x0, 0xFF);
        glClear(GL_COLOR_BUFFER_BIT);

        uptime = SDL_GetTicks()/1000.0;
        /*fprintf(stderr, "%d\n", uptime);*/
        glUniform1f(uniTime, uptime);

        dt = uptime - lasttime;
        lasttime = uptime;

        // rotate
        graphene_matrix_rotate(
            &model,
            (dt) * 45,
            &rotAxis
        );

        GLfloat s = 1 + sin(uptime) * 0.01;
        graphene_matrix_scale(&model, s,s,s);

        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, (float *)&model);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(win);

        SDL_Delay(15); //shit way to limit frames. don't do this.

    }

    glDeleteProgram(shaderProgram);
    deleteShaders(2, shaders);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glDeleteTextures(2, tex);

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);

    SDL_Quit();
    return 0;
}

