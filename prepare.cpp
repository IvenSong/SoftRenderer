#include "prepare.h"

#include <iostream>
#include <filesystem>
#include <memory>

#include "gpu.h"
#include "Shader/lightShader.h"
#include "model.h"

struct PreparedScene::Data {
    LightShader shader;
    Model model;
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
    }
};

PreparedScene::PreparedScene() : data_(std::make_unique<Data>()) {}

PreparedScene::~PreparedScene() = default;

bool PreparedScene::initialize(uint32_t width, uint32_t height) {
    if (data_->initialized || width == 0 || height == 0) {
        return false;
    }

    const char* modelPath = "assets/model/blender_house/house.obj";
    if (!std::filesystem::is_regular_file(modelPath) ||
        !std::filesystem::is_regular_file("assets/model/blender_house/house.mtl")) {
        std::cerr << "Missing house.obj or house.mtl" << std::endl;
        return false;
    }
    std::unique_ptr<image> source(image::createImage("assets/model/blender_house/house_atlas.png"));
    if (!source || !source->mData) {
        std::cerr << "Failed to load assets/model/blender_house/house_atlas.png" << std::endl;
        return false;
    }

    auto& scene = *data_;

    scene.model.read(modelPath);
    scene.model.setModelMatrix(mat4f(1.0f));
    scene.shader.mDirectionalLight.direction = vec3f(0.0f, 0.0f, -1.0f);
    scene.shader.mDirectionalLight.color = vec3f(0.0f, 0.0f, 0.0f);
    scene.shader.mEnvLight = vec3f(1.0f, 1.0f, 1.0f);

    mOwnedCamera = std::make_unique<Camera>(50.0f,
        static_cast<float>(width) / static_cast<float>(height),
        0.1f, 100.0f, vec3f{ 0.0f, 1.0f, 0.0f });
    mCamera = mOwnedCamera.get();
    mCamera->setPose(vec3f(3.0f, 2.7f, 4.5f), vec3f(0.0f, 0.25f, 0.0f));

    sgl->enable(CULL_FACE);
    sgl->frontFace(FRONT_FACE_CCW);
    sgl->cullFace(BACK_FACE);
    sgl->enable(DEPTH_TEST);
    sgl->depthFunc(DEPTH_LESS);
    sgl->disable(BLENDING);

    scene.initialized = true;
    return true;
}

void PreparedScene::render() {
    if (!data_->initialized) {
        return;
    }

    auto& scene = *data_;
    scene.shader.mViewMatrix = mCamera->getViewMatrix();
    scene.shader.mProjectionMatrix = mCamera->getProjectionMatrix();

    sgl->clear();
    sgl->useProgram(&scene.shader);
    scene.model.draw(&scene.shader);
}

void PreparedScene::setCamera(Camera* camera) {
    mCamera = camera ? camera : mOwnedCamera.get();
}

Camera* PreparedScene::getCamera()
{
    return mCamera;
}
