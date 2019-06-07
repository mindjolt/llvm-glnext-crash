//
// Created by Jesus Lopez on 4/23/19.
//

#ifndef TEST_BED_RENDERERWRAPPER_H
#define TEST_BED_RENDERERWRAPPER_H

#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include <string>

#include "NativeClass.h"
#include "Obliviate.h"
#include "AssetManager.h"

class RendererWrapper : public NativeClass<RendererWrapper>, private Logger {
public:
    explicit RendererWrapper(AAssetManager *pManager, bool useProtego);

    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    void onDrawFrame();

private:
    GLuint createProgram(const std::string &vertexShader, const std::string &fragmentShader);
    GLuint loadShader(GLenum type, const std::string &source);
    void deleteProgram(GLuint program);

    AssetManager assetManager;
    bool useProtego;
    Obliviate amulet;
    GLuint programObject{};
    int surfaceWidth{};
    int surfaceHeight{};
    int frame{};
    std::string badVertexShaderSource;
    std::string badFragmentShaderSource;
};


#endif //TEST_BED_RENDERERWRAPPER_H
