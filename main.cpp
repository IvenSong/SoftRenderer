#include <Windows.h>
#include<iostream>
#include "application.h"
#include "gpu.h"
#include "frameBuffer.h"
#include "MathLib.h"
#include "matrix.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup") // 这里的subsystem:console 是显示控制台, console改成windows就是不显示控制台

image* img1 = image::createImage("assets/texture/img1.jpg");

float speed = 0.6;

void change_uv(vec2f& uv);

void prepare_wrap();

vec2f auv = vec2f(0, 0);
vec2f buv = vec2f(0, 1);
vec2f cuv = vec2f(1, 1);
vec2f duv = vec2f(1, 0);

pixel a{ 0, 0, RGBA(255, 0, 0, 255) };
pixel b{ 0, 800, RGBA(0, 255, 0, 255) };
pixel c{ 800, 800, RGBA(0, 0, 255, 255) };
pixel d{ 800, 0, RGBA(0, 0, 255, 255) };

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

    prepare_wrap();

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



    a.uv = auv;
    b.uv = buv;
    c.uv = cuv;
    d.uv = duv;

    change_uv(auv);
    change_uv(buv);
    change_uv(cuv);
    change_uv(duv);

    sgl->setTexture(img1);
    // sgl->setTexture(nullptr);

    sgl->drawTriangle(a, b, c);
    sgl->drawTriangle(a, c, d);


    // sgl->drawImage(*img1, 50, 50);
    // sgl->drawImageAlpha(*img1, 40, 50, 255);

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

void change_uv(vec2f& uv) {
    uv.x += speed;
    // uv.y += speed;
}

void prepare_wrap() {

}