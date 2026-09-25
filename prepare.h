#pragma once

#include <cstdint>
#include <memory>
#include "gpu/camera.h"

class PreparedScene {
public:
    PreparedScene();
    ~PreparedScene();

    PreparedScene(const PreparedScene&) = delete;
    PreparedScene& operator=(const PreparedScene&) = delete;

    bool initialize(uint32_t width, uint32_t height);
    void render();

    void setCamera(Camera* camera);
    Camera* getCamera();

private:
    struct Data;
    std::unique_ptr<Camera> mOwnedCamera;
    Camera* mCamera = { nullptr };
    std::unique_ptr<Data> data_;
};
