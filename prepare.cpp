#include "prepare.h"

#include <iostream>
#include <memory>

#include "gpu.h"
#include "Shader/textureShader.h"

struct PreparedScene::Data {
    TextureShader shader;
    uint32_t textureId = 0;
    uint32_t ebo = 0;
    uint32_t vao = 0;
    uint32_t positionVbo = 0;
    uint32_t colorVbo = 0;
    uint32_t uvVbo = 0;
    float angle = 0.0f;
    bool initialized = false;

    ~Data() {
        if (!initialized) {
            return;
        }

        sgl->useProgram(nullptr);
        sgl->bindVertexArray(0);
        sgl->bindBuffer(ARRAY_BUFFER, 0);
        sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, 0);
        sgl->bindTexture(0);
        sgl->deleteBuffer(positionVbo);
        sgl->deleteBuffer(colorVbo);
        sgl->deleteBuffer(uvVbo);
        sgl->deleteBuffer(ebo);
        sgl->deleteVertexArray(vao);
        sgl->deleteTexture(textureId);
    }
};

PreparedScene::PreparedScene() : data_(std::make_unique<Data>()) {}

PreparedScene::~PreparedScene() = default;

bool PreparedScene::initialize(uint32_t width, uint32_t height) {
    if (data_->initialized || width == 0 || height == 0) {
        return false;
    }

    std::unique_ptr<image> source(image::createImage("assets/texture/img1.jpg"));
    if (!source || !source->mData) {
        std::cerr << "Failed to load assets/texture/img1.jpg" << std::endl;
        return false;
    }

    auto& scene = *data_;

    scene.textureId = sgl->genTexture();
    sgl->bindTexture(scene.textureId);
    sgl->texImage2D(source->mWidth, source->mHeight, source->mData);
    sgl->texParameter(TEXTURE_FILTER, TEXTURE_FILTER_NEAREST);
    scene.shader.mDiffuseTexture = scene.textureId;

    mOwnedCamera = std::make_unique<Camera>(60.0f,
        static_cast<float>(width) / static_cast<float>(height),
        0.1f, 100.0f, vec3f{ 0.0f, 1.0f, 0.0f });
    mCamera = mOwnedCamera.get();

    // Two CCW triangles form the textured quad.
    float positions[] = {
        -0.55f, -1.0f, 0.0f,
         0.55f, -1.0f, 0.0f,
         0.55f,  1.0f, 0.0f,
        -0.55f,  1.0f, 0.0f
    };
    float colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

    scene.ebo = sgl->genBuffer();
    sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, scene.ebo);
    sgl->bufferData(ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
    sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, 0);

    scene.vao = sgl->genVertexArray();
    sgl->bindVertexArray(scene.vao);

    scene.positionVbo = sgl->genBuffer();
    sgl->bindBuffer(ARRAY_BUFFER, scene.positionVbo);
    sgl->bufferData(ARRAY_BUFFER, sizeof(positions), positions);
    sgl->vertexAttributePointer(0, 3, 3 * sizeof(float), 0);

    scene.colorVbo = sgl->genBuffer();
    sgl->bindBuffer(ARRAY_BUFFER, scene.colorVbo);
    sgl->bufferData(ARRAY_BUFFER, sizeof(colors), colors);
    sgl->vertexAttributePointer(1, 4, 4 * sizeof(float), 0);

    scene.uvVbo = sgl->genBuffer();
    sgl->bindBuffer(ARRAY_BUFFER, scene.uvVbo);
    sgl->bufferData(ARRAY_BUFFER, sizeof(uvs), uvs);
    sgl->vertexAttributePointer(2, 2, 2 * sizeof(float), 0);

    sgl->bindBuffer(ARRAY_BUFFER, 0);
    sgl->bindVertexArray(0);

    sgl->enable(CULL_FACE);
    sgl->frontFace(FRONT_FACE_CCW);
    sgl->cullFace(BACK_FACE);
    sgl->disable(DEPTH_TEST);

    scene.initialized = true;
    return true;
}

void PreparedScene::render() {
    if (!data_->initialized) {
        return;
    }

    auto& scene = *data_;
    scene.angle += 0.001f;
    vec3f rotationAxis{ 0.0f, 1.0f, 0.0f };
    scene.shader.mModelMatrix = Math::rotate(mat4f(1.0f), scene.angle, rotationAxis);
    scene.shader.mViewMatrix = mCamera->getViewMatrix();
    scene.shader.mProjectionMatrix = mCamera->getProjectionMatrix();

    sgl->clear();
    sgl->useProgram(&scene.shader);
    sgl->bindVertexArray(scene.vao);
    sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, scene.ebo);
    sgl->drawElement(DRAW_TRIANGLES, 0, 6);
}

void PreparedScene::setCamera(Camera* camera) {
    mCamera = camera ? camera : mOwnedCamera.get();
}

Camera* PreparedScene::getCamera()
{
    return mCamera;
}
