
#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

static GLuint
readShaderFile(const char* file, GLenum shaderType);

int
initEverything(SDL_Window **win, SDL_GLContext *ctx)
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_TIMER))
        err(1, "SDL init failed\n");

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );
    SDL_GL_SetAttribute(SDL_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_MINOR_VERSION, 2);

    *win = SDL_CreateWindow("OGL testaus",
                                       0, 0,
                                       800, 600,
                                       SDL_WINDOW_OPENGL);
    if (win == NULL)
        err(1, "win init fail");

    *ctx = SDL_GL_CreateContext(*win);

    if (ctx == NULL)
        err(1, "ctx create fail");

    /*SDL_GL_MakeCurrent(win, ctx);*/

    /*SDL_GL_LoadLibrary(NULL);*/

    /*SDL_GL_SetSwapInterval(1);*/

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        err(1, "glew init fail");

    return 0;
}

static GLuint
readShaderFile(const char* file, GLenum shaderType)
{
    GLuint id;
    GLchar *buffer;
    int length;

    FILE *f = fopen(file, "r");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length + 1);
        if (buffer) {
            fread(buffer, 1, length, f);
            buffer[length] = 0;
        } else {
            fclose(f);
            return 0;
        }
        fclose(f);
    } else {
        fprintf(stderr, "Unable to open file '%s'\n", file);
        return 0;
    }

    id = glCreateShader(shaderType);

    glShaderSource(id, 1, (const GLchar **)&buffer, NULL);

    glCompileShader(id);

    // check for errors
    GLint compiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        fprintf(stderr, "Unable to compile shader '%s'\n", file);
        char error[512];
        glGetShaderInfoLog(id, 512, NULL, error);
        fprintf(stderr, "%s\n", error);
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

static int
handleEvents(SDL_Event *ev)
{
    while (SDL_PollEvent(ev)) {
        switch (ev->type) {
        case SDL_QUIT:
            return 1;
            break;
        case SDL_KEYUP:
            if (ev->key.keysym.sym == SDLK_ESCAPE)
                return 1;
            break;
        default:
            break;
        }
    }
    return 0;
}

static GLuint
createShaderProgram(size_t shaders_size, GLuint* shaders)
{
    GLuint prog = glCreateProgram();
    for (int i=0; i<shaders_size; i++)
        glAttachShader(prog, shaders[i]);

    return prog;
}

static void
deleteShaders(size_t shaders_size, GLuint* shaders)
{
    for (int i=0; i<shaders_size; i++)
        glDeleteShader(shaders[i]);
}
