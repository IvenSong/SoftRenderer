#include <Windows.h>
#include<iostream>
#include "application.h"
#include "gpu.h"
#include "frameBuffer.h"
#include "MathLib.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup") // 这里的subsystem:console 是显示控制台, console改成windows就是不显示控制台

image* img1 = image::createImage("assets/texture/img1.jpg");

void render();

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,         // Instance Handle: 本应用程序实例句柄，唯一指代当前程序
    _In_opt_ HINSTANCE hPrevInstance, // Previous Instance: 前一个实例，现代 Windows 中一般为 null
    _In_ LPWSTR lpCmdLine,            // Command Line: 应用程序运行参数（宽字符字符串）
    _In_ int nCmdShow)                // Show Command: 窗口显示方式（最大化、最小化、隐藏）
{
    if (!app->initApplication(hInstance, 1920, 1080)) {
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

    int r = 150;
    pixel a{ 500, 100, RGBA(255, 0, 0, 255) };
    pixel b{ 100, 300, RGBA(0, 255, 0, 255) };
    pixel c{ 700, 300, RGBA(0, 0, 255, 255) };

    sgl->drawTriangle(a, b, c);

    // sgl->drawImageAlpha(*img1, 0, 0, 100);

    //for (float i = 0; i < 360; i += 10)
    //{
    //    float radian = DEG2RAD(i);
    //    int x = r * sin(radian) + c.x;
    //    int y = r * cos(radian) + c.y;

    //    pixel pt{ x, y, RGBA(rand() % 255, rand() % 255, rand() % 255, 255) };

    //    sgl->drawLine(c, pt);
    //}

    //for (uint32_t i = 0; i < app->getWidth(); ++i) {
    //    for (uint32_t j = 0; j < app->getHeight(); ++j) {
    //        uint32_t v = std::rand() % 255;
    //        RGBA color(v, v, v, v);
    //        sgl->drawPoint(i, j, color);
    //    }
    //}
}