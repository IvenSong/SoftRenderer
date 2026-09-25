#include <Windows.h>

#include "application.h"
#include "gpu.h"
#include "prepare.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    if (!app->initApplication(hInstance, 800, 600)) {
        return -1;
    }

    sgl->initSurface(app->getWidth(), app->getHeight(), app->getCanvas());

    PreparedScene scene;
    if (!scene.initialize(app->getWidth(), app->getHeight())) {
        return -1;
    }
    app->setInputHandler(scene.getCamera());

    while (app->peekMessage()) {
        scene.getCamera()->update();
        scene.render();
        app->show();
    }
    app->setInputHandler(nullptr);

    return 0;
}
