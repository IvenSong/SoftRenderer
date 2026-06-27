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

#define TEXTURE_WRAP_REPEAT 0
#define TEXTURE_WRAP_MIRROR 1

using byte = unsigned char;

struct RGBA {
    byte mB;
    byte mG;
    byte mR;
    byte mA;

    static byte clampChannel(float value) {
        return static_cast<byte>(std::clamp(value, 0.0f, 255.0f));
    }

    static byte clampChannel(int value) {
        return static_cast<byte>(std::clamp(value, 0, 255));
    }

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
            clampChannel((int)mR + other.mR),
            clampChannel((int)mG + other.mG),
            clampChannel((int)mB + other.mB),
            clampChannel((int)mA + other.mA)
        );
    }

    RGBA& operator+=(const RGBA& other) {
        *this = *this + other;
        return *this;
    }

    RGBA operator-(const RGBA& other) const {
        return RGBA(
            clampChannel((int)mR - other.mR),
            clampChannel((int)mG - other.mG),
            clampChannel((int)mB - other.mB),
            clampChannel((int)mA - other.mA)
        );
    }

    RGBA& operator-=(const RGBA& other) {
        *this = *this - other;
        return *this;
    }

    RGBA operator*(float scalar) const {
        return RGBA(
            clampChannel(mR * scalar),
            clampChannel(mG * scalar),
            clampChannel(mB * scalar),
            clampChannel(mA * scalar)
        );
    }

    RGBA& operator*=(float scalar) {
        *this = *this * scalar;
        return *this;
    }

    RGBA operator/(float scalar) const {
        assert(scalar != 0.0f);
        if (scalar == 0.0f) {
            return RGBA(0, 0, 0, 0);
        }

        return RGBA(
            clampChannel(mR / scalar),
            clampChannel(mG / scalar),
            clampChannel(mB / scalar),
            clampChannel(mA / scalar)
        );
    }

    RGBA& operator/=(float scalar) {
        *this = *this / scalar;
        return *this;
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

    pixel() = default;


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
