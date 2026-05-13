#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdint>
#include <cassert> 
#include <algorithm>
#include "vector.h"

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

    RGBA operator+(const RGBA& other) const {
        return RGBA(
            static_cast<byte>(std::clamp((int)mR + other.mR, 0, 255)),
            static_cast<byte>(std::clamp((int)mG + other.mG, 0, 255)),
            static_cast<byte>(std::clamp((int)mB + other.mB, 0, 255)),
            static_cast<byte>(std::clamp((int)mA + other.mA, 0, 255))
        );
    }

    RGBA operator*(float scalar) const {
        return RGBA(
            static_cast<byte>(std::clamp(mR * scalar, 0.0f, 255.0f)),
            static_cast<byte>(std::clamp(mG * scalar, 0.0f, 255.0f)),
            static_cast<byte>(std::clamp(mB * scalar, 0.0f, 255.0f)),
            static_cast<byte>(std::clamp(mA * scalar, 0.0f, 255.0f))
        );
    }

    friend RGBA operator*(float scalar, const RGBA& c) {
        return c * scalar;
    }
};

struct pixel {
    int x;
    int y;
    RGBA color;
    vec2f uv;


    pixel(int x, int y, RGBA color) {
        this->x = x;
        this->y = y;
        this->color = color;
    }
    pixel(int x, int y) {
        this->x = x;
        this->y = y;
        this->color = RGBA();
    }
};