#include <Windows.h>
#include<iostream>
#include "application.h"
#include "gpu.h"
#include "frameBuffer.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")

void render();

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,         // Instance Handle: 本应用程序实例句柄，唯一指代当前程序
    _In_opt_ HINSTANCE hPrevInstance, // Previous Instance: 前一个实例，现代 Windows 中一般为 null
    _In_ LPWSTR lpCmdLine,            // Command Line: 应用程序运行参数（宽字符字符串）
    _In_ int nCmdShow)                // Show Command: 窗口显示方式（最大化、最小化、隐藏）
{
    if (!app->initApplication(hInstance, 800, 600)) {
        return -1;
    }

    bool alive = true;

    sgl->initSurface(app->getWidth(), app->getHeight(), app->getCanvas());
    while (alive) {
        alive = app->peekMessage();
        render();
        app->show();
    }
    return 0;
}

void render() {
    sgl->clear();

    RGBA color(255, 255, 255, 255);
    for (int i = 400; i < 500; ++i) {
        sgl->drawPoint(i, i, color);
    }
    sgl->drawLine(0, 0, 800, 800, color);

    //for (uint32_t i = 0; i < app->getWidth(); ++i) {
    //    for (uint32_t j = 0; j < app->getHeight(); ++j) {
    //        uint32_t v = std::rand() % 255;
    //        RGBA color(v, v, v, v);
    //        sgl->drawPoint(i, j, color);
    //    }
    //}
}