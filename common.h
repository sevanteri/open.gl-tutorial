
#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SOIL/SOIL.h>

static void
printShaderInfo(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        GLchar buffer[length];
        glGetShaderInfoLog(shader, length, NULL, buffer);
        fprintf(stderr, "%s\n", buffer);
    }
}

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

static GLchar*
readShaderFile(const char* file)
{
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

    return buffer;

}

static GLint
compileShader(GLuint id, char *buffer)
{
    //fprintf(stderr, "compiling: %d\n", id);
    glShaderSource(id, 1, (const GLchar **)&buffer, NULL);

    glCompileShader(id);

    // check for errors
    GLint compiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        //fprintf(stderr, "not compiled\n");

        printShaderInfo(id);
        //glDeleteShader(*id);
        //*id = 0;
    }

    return compiled;
}

static GLuint
compileShaderFile(const char* file, GLenum shaderType)
{
    GLuint id;
    GLchar *buffer;

    id = glCreateShader(shaderType);

    buffer = readShaderFile(file);

    compileShader(id, buffer);

    free(buffer);

    return id;
}

static GLint
reloadShaderFile(GLuint shader, char *file)
{
    GLchar *buffer = readShaderFile(file);

    GLint compiled = compileShader(shader, buffer);

    free(buffer);
    return compiled;
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

void
reloadShader(GLuint shader, char* filename, GLuint prog)
{
    if(reloadShaderFile(shader, filename) == GL_TRUE)
    {
        glLinkProgram(prog);
        glUseProgram(prog);
    }
}

void
loadTextures(char** images, GLuint* textures, size_t s)
{
    int w, h;
    unsigned char* image;
    int i;

    for (i=0; i<s; i++) {
        image = SOIL_load_image(images[i], &w, &h, 0, SOIL_LOAD_RGB);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void
pointTextures(GLint prog, char** locations, GLuint* textures, size_t s)
{
    int i;
    for (i=0; i<s; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(prog, locations[i]), i);
    }
}

void
resizeWindow(SDL_WindowEvent *ev)
{
    Sint32 w = ev->data1;
    Sint32 h = ev->data2;
    glViewport(0, 0, w, h);
}
