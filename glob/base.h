#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdint> // 必须包含这个！为了让编译器认识 uint32_t
#include <cassert> // 现代 C++ 推荐写法

#define PI 3.14159265358979323
#define DEG2RAD(theta) (0.01745329251994329 * (theta))
#define FRACTION(v) ((v) - (int)(v))

using byte = unsigned char;

struct RGBA {
    byte mB;
    byte mG;
    byte mR;
    byte mA;

    RGBA(
        byte r = 255,
        byte g = 255,
        byte b = 255,
        byte a = 255)
    {
        mR = r;
        mG = g;
        mB = b;
        mA = a;
    }
};

struct pixel {
    int x;
    int y;
    RGBA color;

    pixel(int x, int y, RGBA color) {
        this->x = x;
        this->y = y;
        this->color = color;
    }
};