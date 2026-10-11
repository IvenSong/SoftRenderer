// Run from the repository root. Render the cube and check lighting regressions.
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "gpu.h"
#include "Shader/lightShader.h"
#include "Shader/textureShader.h"
#include "../cubeMesh.h"
#include "../prepare.h"

// This standard-library name must not be replaced by an application macro.
static_assert(std::ios_base::app != std::ios_base::openmode{});

namespace {
constexpr uint32_t width = 800;
constexpr uint32_t height = 600;
const RGBA background(32, 36, 44, 255);

using Vertex = CubeVertex;
using Cube = CubeMesh;

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

bool samePixels(const std::vector<RGBA>& a, const std::vector<RGBA>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(),
        [](const RGBA& x, const RGBA& y) {
            return x.mR == y.mR && x.mG == y.mG && x.mB == y.mB && x.mA == y.mA;
        });
}

struct MeshResources {
    uint32_t vao = sgl->genVertexArray();
    uint32_t vbo = sgl->genBuffer();
    uint32_t ebo = sgl->genBuffer();
    uint32_t texture = sgl->genTexture();

    MeshResources(Cube& cube, const image& source) {
        sgl->bindVertexArray(vao);
        sgl->bindBuffer(ARRAY_BUFFER, vbo);
        sgl->bufferData(ARRAY_BUFFER, sizeof(cube.vertices), cube.vertices.data());
        sgl->vertexAttributePointer(0, 3, sizeof(Vertex), offsetof(Vertex, position));
        sgl->vertexAttributePointer(2, 2, sizeof(Vertex), offsetof(Vertex, uv));
        sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, ebo);
        sgl->bufferData(ELEMENT_ARRAY_BUFFER, sizeof(cube.indices), cube.indices.data());
        sgl->bindTexture(texture);
        sgl->texImage2D(source.mWidth, source.mHeight, source.mData);
        sgl->texParameter(TEXTURE_FILTER, TEXTURE_FILTER_NEAREST);
    }

    ~MeshResources() {
        sgl->useProgram(nullptr);
        sgl->bindVertexArray(0);
        sgl->bindBuffer(ARRAY_BUFFER, 0);
        sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, 0);
        sgl->bindTexture(0);
        sgl->deleteVertexArray(vao);
        sgl->deleteBuffer(vbo);
        sgl->deleteBuffer(ebo);
        sgl->deleteTexture(texture);
    }

    void draw(Shader& shader, bool lighting) {
        sgl->useProgram(&shader);
        sgl->bindVertexArray(vao);
        sgl->bindBuffer(ARRAY_BUFFER, vbo);
        sgl->vertexAttributePointer(1, lighting ? 3 : 4, sizeof(Vertex),
            lighting ? offsetof(Vertex, normal) : offsetof(Vertex, color));
        sgl->bindBuffer(ELEMENT_ARRAY_BUFFER, ebo);
        sgl->drawElement(DRAW_TRIANGLES, 0, 36);
    }
};

// Only observes values. All shading is delegated unchanged to LightShader.
class ObservedLightShader : public LightShader {
public:
    size_t fragments = 0;
    size_t zeroNormals = 0;
    size_t nonfiniteNormals = 0;

    void fragmentShader(const VsOutput& input, FsOutput& output,
        const std::map<uint32_t, Texture*>& textures) override {
        ++fragments;
        if (Math::lengthSQ(input.mNormal) == 0.0f) {
            ++zeroNormals;
        }
        if (!std::isfinite(input.mNormal.x) || !std::isfinite(input.mNormal.y)
            || !std::isfinite(input.mNormal.z)) {
            ++nonfiniteNormals;
        }
        LightShader::fragmentShader(input, output, textures);
    }
};

void beginFrame(std::vector<RGBA>& pixels) {
    sgl->clear();
    std::fill(pixels.begin(), pixels.end(), background);
}

size_t changedPixels(const std::vector<RGBA>& pixels) {
    return std::count_if(pixels.begin(), pixels.end(), [](const RGBA& p) {
        return p.mR != background.mR || p.mG != background.mG || p.mB != background.mB;
    });
}

void writeBmp(const std::filesystem::path& path, const std::vector<RGBA>& pixels) {
    // Screen coordinates and a positive-height BMP both start at the bottom left.
    const uint32_t stride = (width * 3 + 3) & ~3u;
    BITMAPFILEHEADER file{};
    BITMAPINFOHEADER info{};
    file.bfType = 0x4d42;
    file.bfOffBits = sizeof(file) + sizeof(info);
    file.bfSize = file.bfOffBits + stride * height;
    info.biSize = sizeof(info);
    info.biWidth = width;
    info.biHeight = height;
    info.biPlanes = 1;
    info.biBitCount = 24;
    info.biCompression = BI_RGB;
    info.biSizeImage = stride * height;
    std::ofstream output(path, std::ios::binary);
    output.write(reinterpret_cast<const char*>(&file), sizeof(file));
    output.write(reinterpret_cast<const char*>(&info), sizeof(info));
    std::vector<unsigned char> row(stride, 0);
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            const auto& pixel = pixels[y * width + x];
            row[x * 3] = pixel.mB;
            row[x * 3 + 1] = pixel.mG;
            row[x * 3 + 2] = pixel.mR;
        }
        output.write(reinterpret_cast<const char*>(row.data()), row.size());
    }
    if (!output) {
        throw std::runtime_error("Could not save BMP");
    }
}

void probeShaderStages(Cube& cube, std::ostream& log) {
    LightShader shader;
    shader.mModelMatrix = mat4f(1.0f);
    shader.mViewMatrix = mat4f(1.0f);
    shader.mProjectionMatrix = mat4f(1.0f);
    shader.mDirectionalLight.direction = vec3f(0, 0, -1);
    shader.mDirectionalLight.color = vec3f(0.25f, 0.25f, 0.25f);
    shader.mDiffuseTexture = 1;
    RGBA gray(128, 128, 128, 255);
    Texture texture;
    texture.setBufferData(1, 1, &gray);
    std::map<uint32_t, Texture*> textures{{1, &texture}};
    VsOutput input;
    input.mNormal = vec3f(0, 0, 1);
    input.mUV = vec2f(0.5f, 0.5f);
    FsOutput noAmbient{}, withAmbient{}, backFacing{};
    shader.mEnvLight = vec3f(0, 0, 0);
    shader.fragmentShader(input, noAmbient, textures);
    shader.mEnvLight = vec3f(0.5f, 0.5f, 0.5f);
    shader.fragmentShader(input, withAmbient, textures);
    input.mNormal = vec3f(0, 0, -1);
    shader.fragmentShader(input, backFacing, textures);
    log << "ambient_probe_red: env0=" << int(noAmbient.mColor.mR)
        << " env0.5=" << int(withAmbient.mColor.mR)
        << " back_facing_env0.5=" << int(backFacing.mColor.mR)
        << " (expected 32,96,64 for texture*(diffuse+ambient))\n";
    require(noAmbient.mColor.mR == 32 && withAmbient.mColor.mR == 96
        && backFacing.mColor.mR == 64, "Ambient/diffuse regression");

    input.mNormal = vec3f(0, 0, 0);
    FsOutput zeroNormal{};
    shader.fragmentShader(input, zeroNormal, textures);
    require(zeroNormal.mColor.mR == 64, "Zero normal should retain ambient light");
    gray.mA = 96;
    texture.setBufferData(1, 1, &gray);
    input.mNormal = vec3f(0, 0, 1);
    FsOutput translucent{};
    shader.fragmentShader(input, translucent, textures);
    require(translucent.mColor.mA == 96, "Lighting must preserve texture alpha");
    const auto clamped = shader.vectorToRGBA(vec4f(1.4f, -0.25f, 0.5f, 1.2f));
    require(clamped.mR == 255 && clamped.mG == 0 && clamped.mB == 127
        && clamped.mA == 255, "RGBA clamping regression");
    log << "color_checks: clamped=(255,0,127,255), texture_alpha=96, zero_normal_ambient=64\n";

    std::map<uint32_t, std::unique_ptr<BufferObject>> buffers;
    buffers[1] = std::make_unique<BufferObject>();
    buffers[1]->setBufferData(sizeof(cube.vertices), cube.vertices.data());
    std::map<uint32_t, BindingDescription> bindings{
        {0, {1, 3, sizeof(Vertex), offsetof(Vertex, position)}},
        {1, {1, 3, sizeof(Vertex), offsetof(Vertex, normal)}},
        {2, {1, 2, sizeof(Vertex), offsetof(Vertex, uv)}}};
    vec3f axis(0, 1, 0);
    shader.mModelMatrix = Math::rotate(mat4f(1.0f), static_cast<float>(PI / 2), axis);
    const auto vertex = shader.vertexShader(bindings, buffers, 0);
    const auto expected = shader.mModelMatrix * vec4f(0, 0, 1, 0);
    log << "rotated_normal: actual=" << vertex.mNormal.x << ',' << vertex.mNormal.y
        << ',' << vertex.mNormal.z << " expected=" << expected.x << ',' << expected.y
        << ',' << expected.z << '\n';
    require(Math::length(vertex.mNormal - static_cast<vec3f>(expected)) < 0.0001f,
        "Rotated normal regression");

    Cube scaledCube = cube;
    scaledCube.vertices[0].normal = Math::normalize(vec3f(1, 1, 0));
    buffers[1]->setBufferData(sizeof(scaledCube.vertices), scaledCube.vertices.data());
    shader.mModelMatrix = Math::scale(mat4f(1.0f), 2.0f, 1.0f, 0.5f);
    const vec3f scaledNormal = shader.vertexShader(bindings, buffers, 0).mNormal;
    const vec3f expectedScaledNormal = Math::normalize(vec3f(0.5f, 1.0f, 0.0f));
    require(Math::length(scaledNormal - expectedScaledNormal) < 0.0001f,
        "Nonuniform scaling must use the inverse-transpose normal matrix");
    log << "scaled_normal: actual=" << scaledNormal.x << ',' << scaledNormal.y
        << ',' << scaledNormal.z << " expected=(0.447214,0.894427,0)\n";
}
} // namespace

int main(int argc, char** argv) {
    try {
        const std::string texturePath = argc > 1 ? argv[1] : "assets/texture/img1.jpg";
        const std::filesystem::path outputDir = argc > 2 ? argv[2] : "out/cube-lighting-experiment";
        if (!std::filesystem::is_regular_file(texturePath)) {
            throw std::runtime_error("Texture missing: run from the repository root or pass its path");
        }
        std::unique_ptr<image> source(image::createImage(texturePath));
        if (!source || !source->mData) {
            throw std::runtime_error("Texture could not be decoded");
        }
        std::filesystem::create_directories(outputDir);
        Cube cube;
        MeshResources resources(cube, *source);
        std::vector<RGBA> pixels(width * height);
        const mat4f view = Math::lookAt<float>(vec3f(3, 2.5f, 4), vec3f(0, 0, 0), vec3f(0, 1, 0));
        const mat4f projection = Math::perspective(50.0f, float(width) / height, 0.1f, 100.0f);
        sgl->enable(CULL_FACE);
        sgl->frontFace(FRONT_FACE_CCW);
        sgl->cullFace(BACK_FACE);
        sgl->enable(DEPTH_TEST);
        sgl->depthFunc(DEPTH_LESS);
        sgl->disable(BLENDING);

        ObservedLightShader light;
        light.mModelMatrix = mat4f(1.0f);
        light.mViewMatrix = view;
        light.mProjectionMatrix = projection;
        light.mDiffuseTexture = resources.texture;
        light.mDirectionalLight.direction = Math::normalize(vec3f(-0.4f, -1.0f, -0.6f));
        light.mDirectionalLight.color = vec3f(1, 1, 1);
        light.mEnvLight = vec3f(0.15f, 0.15f, 0.15f);

        std::ostringstream log;
        log << "cube: 24 vertices, 12 CCW triangles, half extent 0.8\n"
            << "camera: (3,2.5,4) looking at origin; FOV 50 degrees; 800x600\n"
            << "texture: " << texturePath << "; nearest sampling\n"
            << "light travel direction: normalize(-0.4,-1,-0.6); white light; ambient 0.15\n"
            << "back-face culling: on; depth test: LESS\n";
        sgl->initSurface(width, height, pixels.data());
        beginFrame(pixels);
        resources.draw(light, true);
        writeBmp(outputDir / "cube_light_actual.bmp", pixels);
        log << "lit_frame: fragments=" << light.fragments << " zero_normals=" << light.zeroNormals
            << " nonfinite_input_normals=" << light.nonfiniteNormals
            << " changed_pixels=" << changedPixels(pixels) << '\n';
        require(light.fragments > 0 && light.zeroNormals == 0 && light.nonfiniteNormals == 0,
            "Rasterized normals are missing or nonfinite");
        const auto firstLitFrame = pixels;
        beginFrame(pixels);
        resources.draw(light, true);
        require(samePixels(firstLitFrame, pixels), "Identical lit frames differ after clear");
        log << "lit_frame2_after_clear: identical=true\n";

        TextureShader texture;
        texture.mModelMatrix = mat4f(1.0f);
        texture.mViewMatrix = view;
        texture.mProjectionMatrix = projection;
        texture.mDiffuseTexture = resources.texture;
        // Start the independent texture control with a fresh framebuffer.
        sgl->initSurface(width, height, pixels.data());
        beginFrame(pixels);
        resources.draw(texture, false);
        writeBmp(outputDir / "cube_texture_control.bmp", pixels);
        log << "texture_frame1: changed_pixels=" << changedPixels(pixels) << '\n';
        const auto firstTextureFrame = pixels;
        require(changedPixels(pixels) > 0, "Texture control did not render");
        // Repeat the identical frame using only the public clear() operation.
        beginFrame(pixels);
        resources.draw(texture, false);
        writeBmp(outputDir / "cube_texture_frame2.bmp", pixels);
        log << "texture_frame2_after_clear: changed_pixels=" << changedPixels(pixels) << '\n';
        require(samePixels(firstTextureFrame, pixels), "Depth clear regression");

        probeShaderStages(cube, log);

        // Exercise the exact scene implementation used by main.cpp, without a window.
        sgl->initSurface(width, height, pixels.data());
        PreparedScene mainScene;
        require(mainScene.initialize(width, height), "Main scene initialization failed");
        const mat4f initialView = mainScene.getCamera()->getViewMatrix();
        mainScene.getCamera()->onRMouseDown(0, 0);
        mainScene.getCamera()->onMouseMove(0, 0);
        mainScene.getCamera()->onRMouseUp(0, 0);
        mainScene.getCamera()->update();
        const mat4f updatedView = mainScene.getCamera()->getViewMatrix();
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                require(std::abs(initialView.get(row, col) - updatedView.get(row, col)) < 0.0001f,
                    "Camera orientation jumps after initialization");
            }
        }
        const auto nonblackPixels = [&pixels]() {
            return std::count_if(pixels.begin(), pixels.end(), [](const RGBA& p) {
                return p.mR != 0 || p.mG != 0 || p.mB != 0;
            });
        };
        mainScene.render();
        const auto mainFrame1 = nonblackPixels();
        writeBmp(outputDir / "cube_main_scene.bmp", pixels);
        mainScene.render();
        const auto mainFrame2 = nonblackPixels();
        require(mainFrame1 > 1000 && mainFrame2 > 1000, "Main scene failed to render consecutive frames");
        log << "main_scene: nonblack_pixels_frame1=" << mainFrame1
            << " frame2=" << mainFrame2 << " camera_pose_stable=true\n";
        log << "ALL_CHECKS_PASSED\n";
        std::ofstream(outputDir / "measurements.txt") << log.str();
        std::cout << log.str();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
