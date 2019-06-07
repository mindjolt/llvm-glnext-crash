//
// Created by Jesus Lopez on 4/23/19.
//

#include <android/log.h>
#include <vector>

#include "RendererWrapper.h"
#include "Protego.h"

RendererWrapper::RendererWrapper(AAssetManager *pManager, bool useProtego) :
  Logger("RendererWrapper"),
  assetManager(pManager),
  useProtego(useProtego) {
    badVertexShaderSource = assetManager.readAsset("vertex.glsl");
    badFragmentShaderSource = assetManager.readAsset("fragment.glsl");
}

void RendererWrapper::onSurfaceCreated() {
    if (useProtego)
        Protego::cast();

    char vShaderStr[] =
            "attribute vec4 vPosition;\n"
            "void main() {\n"
            "  gl_Position = vPosition;\n"
            "}";

    char fShaderStr[] =
            "precision mediump float;\n"
            "void main() {\n"
            "  gl_FragColor = vec4(1, 0, 0, 1);\n"
            "}";

    amulet.curse();
    programObject = createProgram(vShaderStr, fShaderStr);
}

void RendererWrapper::onSurfaceChanged(int width, int height) {
    surfaceWidth = width;
    surfaceHeight = height;
}

void RendererWrapper::onDrawFrame() {
    if (frame && frame % 60 == 0) {
        info("Compiling bad program at frame %d. This will %s", frame,
                useProtego ? "not crash because Protego is enabled"
                           : "crash on beyondq devices because Protego is disabled");
        GLuint badKitty = createProgram(badVertexShaderSource, badFragmentShaderSource);
        deleteProgram(badKitty);
    }
    frame++;

    GLfloat vVertices[] = {
            0, 0.5, 0,
            -0.5f, -0.5f, 0,
            0.5, -0.5f, 0};

    // Set the viewport
    glViewport(0, 0, surfaceWidth, surfaceHeight);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(programObject);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint RendererWrapper::createProgram(const std::string &vertexSource, const std::string &fragSource) {
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragSource);

    // Create the program object
    GLuint programObject = glCreateProgram();
    if (programObject == 0)
        return 0;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = static_cast<char *>(malloc(static_cast<size_t>(infoLen)));

            glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog);
            __android_log_print(ANDROID_LOG_ERROR, "Game", "Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programObject);
        return 0;
    }
    return programObject;
}

GLuint RendererWrapper::loadShader(GLenum type, const std::string &shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
        return 0;

    // Load the shader source
    const GLchar *sources = shaderSrc.c_str();
    glShaderSource(shader, 1, &sources, nullptr);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = static_cast<char *>(malloc(static_cast<size_t>(infoLen)));

            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            __android_log_print(ANDROID_LOG_ERROR, "Game", "Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void RendererWrapper::deleteProgram(GLuint program) {
    GLint shaderCount;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaderCount);
    std::vector<GLuint> shaders(static_cast<unsigned int>(shaderCount));
    glGetAttachedShaders(program, shaderCount, &shaderCount, &shaders[0]);
    shaders.resize(static_cast<unsigned int>(shaderCount));
    for (GLuint shader : shaders) {
        glDetachShader(program, shader);
        glDeleteShader(shader);
    }
    glDeleteProgram(program);
}
