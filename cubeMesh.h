#pragma once

#include <array>
#include <stdexcept>
#include "Math/MathLib.h"

struct CubeVertex {
    vec3f position;
    vec3f normal;
    vec2f uv;
    vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
};

struct CubeMesh {
    std::array<CubeVertex, 24> vertices;
    std::array<uint32_t, 36> indices;

    CubeMesh() {
        const vec3f normals[] = {
            {0, 0, 1}, {0, 0, -1}, {1, 0, 0},
            {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}
        };
        // cross(u, v) points outward on each face.
        const vec3f u[] = {
            {1, 0, 0}, {-1, 0, 0}, {0, 0, -1},
            {0, 0, 1}, {1, 0, 0}, {1, 0, 0}
        };
        const vec3f v[] = {
            {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
            {0, 1, 0}, {0, 0, -1}, {0, 0, 1}
        };
        const vec2f uv[] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
        const uint32_t faceIndices[] = {0, 1, 2, 0, 2, 3};
        for (uint32_t face = 0; face < 6; ++face) {
            for (uint32_t corner = 0; corner < 4; ++corner) {
                auto& vertex = vertices[face * 4 + corner];
                vertex.position = (normals[face]
                    + u[face] * (2.0f * uv[corner].x - 1.0f)
                    + v[face] * (2.0f * uv[corner].y - 1.0f)) * 0.8f;
                vertex.normal = normals[face];
                vertex.uv = uv[corner];
            }
            for (uint32_t i = 0; i < 6; ++i) {
                indices[face * 6 + i] = face * 4 + faceIndices[i];
            }
            const auto& a = vertices[face * 4].position;
            const auto& b = vertices[face * 4 + 1].position;
            const auto& c = vertices[face * 4 + 2].position;
            if (Math::dot(Math::cross(b - a, c - a), normals[face]) <= 0) {
                throw std::runtime_error("CubeMesh face winding is incorrect");
            }
        }
    }
};

