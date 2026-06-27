#include <Windows.h>
#include<iostream>
#include "application.h"
#include "gpu.h"
#include "frameBuffer.h"
#include "MathLib.h"
#include "matrix.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup") // 这里的subsystem:console 是显示控制台, console改成windows就是不显示控制台

image* img1 = image::createImage("assets/texture/img1.jpg");
image* img2 = image::createImage("assets/texture/HD5I_yPbgAAoUAA.jpg");

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
void renderRotatingTriangleExperiment();

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,         // Instance Handle: 本应用程序实例句柄，唯一指代当前程序
    _In_opt_ HINSTANCE hPrevInstance, // Previous Instance: 前一个实例，现代 Windows 中一般为 null
    _In_ LPWSTR lpCmdLine,            // Command Line: 应用程序运行参数（宽字符字符串）
    _In_ int nCmdShow)                // Show Command: 窗口显示方式（最大化、最小化、隐藏）
{
    if (!app->initApplication(hInstance, 800, 600)) {
        return -1;
    }
    auto v = Math::normalize(vec3f(3, 0, 4));
    std::cout << v.x << v.y << v.z;


    bool alive = true;

    sgl->initSurface(app->getWidth(), app->getHeight(), app->getCanvas());

    prepare_wrap();

    while (alive) {
        alive = app->peekMessage();
        renderRotatingTriangleExperiment();
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

void renderRotatingTriangleExperiment() {
    static float angle = 0.0f;

    const vec4f vertices[3] = {
        vec4f(-0.8f, -0.6f, 0.0f, 1.0f),
        vec4f(0.8f, -0.6f, 0.0f, 1.0f),
        vec4f(0.0f, 0.8f, 0.0f, 1.0f)
    };

    vec3f rotationAxis(0.4f, 1.0f, 0.2f);

    mat4f model(1.0f);
    model = Math::translate(model, 0.0f, 0.0f, -3.0f);
    model = Math::rotate(model, angle, rotationAxis);

    const float aspect = static_cast<float>(app->getWidth()) /
        static_cast<float>(app->getHeight());
    static mat4f projection = Math::perspective(60.0f, aspect, 0.1f, 100.0f);
    static int back = 1;
    if (projection.get(0, 3) > 3 || projection.get(0, 3) < -3) {
        back *= -1;
    }
    projection.set(0, 3, projection.get(0, 3) + 0.01 * back);
    mat4f screen = Math::screenMatrix<float>(app->getWidth(), app->getHeight());
    mat4f transform = screen * projection * model;

    vec4f transformed[3] = {
        transform * vertices[0],
        transform * vertices[1],
        transform * vertices[2]
    };

    for (const vec4f& vertex : transformed) {
        if (std::abs(vertex.w) < 0.00001f) {
            return;
        }
    }

    pixel triangle[3] = {
        pixel(static_cast<int>(transformed[0].x / transformed[0].w),
            static_cast<int>(transformed[0].y / transformed[0].w),
            RGBA(255, 80, 80, 255)),
        pixel(static_cast<int>(transformed[1].x / transformed[1].w),
            static_cast<int>(transformed[1].y / transformed[1].w),
            RGBA(80, 255, 120, 255)),
        pixel(static_cast<int>(transformed[2].x / transformed[2].w),
            static_cast<int>(transformed[2].y / transformed[2].w),
            RGBA(80, 140, 255, 255))
    };

    // set img uv
    triangle[0].uv = vec2f(0.0f, 0.0f);
    triangle[1].uv = vec2f(1.0f, 0.0f);
    triangle[2].uv = vec2f(0.5f, 1.0f);

    sgl->clear();
    sgl->setTexture(nullptr);
    sgl->drawTriangle(triangle[0], triangle[1], triangle[2]);

    angle += 0.02f;
    if (angle >= 2.0f * PI) {
        angle -= static_cast<float>(2.0 * PI);
    }
}
