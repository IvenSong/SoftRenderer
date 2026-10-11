#include <Windows.h>

#include "application.h"
#include "gpu.h"
#include "prepare.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    auto* application = Application::getInstance();
    if (!application->initApplication(hInstance, 800, 600)) {
        return -1;
    }

    sgl->initSurface(application->getWidth(), application->getHeight(), application->getCanvas());

    PreparedScene scene;
    if (!scene.initialize(application->getWidth(), application->getHeight())) {
        return -1;
    }
    application->setInputHandler(scene.getCamera());

    while (application->peekMessage()) {
        scene.getCamera()->update();
        scene.render();
        application->show();
    }
    application->setInputHandler(nullptr);

    return 0;
}
