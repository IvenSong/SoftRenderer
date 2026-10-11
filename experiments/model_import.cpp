// Run from the repository root; exercise the existing loader without a window.
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "model.h"
#include "gpu.h"
#include "../prepare.h"

int main() {
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    const std::string path = "assets/model/blender_house/house.obj";

    // Establish that the fixture and texture are readable before testing Model.
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene || !scene->mRootNode || scene->mNumMeshes != 2) {
        std::cerr << "ASSET_FAILED: " << importer.GetErrorString() << std::endl;
        return 1;
    }
    unsigned int faces = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        auto* mesh = scene->mMeshes[i];
        aiString diffuse;
        if (!mesh->HasNormals() || !mesh->HasTextureCoords(0) ||
            scene->mMaterials[mesh->mMaterialIndex]->GetTexture(
                aiTextureType_DIFFUSE, 0, &diffuse) != AI_SUCCESS) {
            std::cerr << "ASSET_FAILED: mesh lacks normals, UVs or texture" << std::endl;
            return 1;
        }
        faces += mesh->mNumFaces;
        std::cout << "MESH_OK: " << mesh->mName.C_Str() << " faces="
            << mesh->mNumFaces << " texture=" << diffuse.C_Str() << std::endl;
    }
    aiString texturePath;
    if (scene->mMaterials[scene->mMeshes[0]->mMaterialIndex]->GetTexture(
            aiTextureType_DIFFUSE, 0, &texturePath) != AI_SUCCESS) {
        std::cerr << "ASSET_FAILED: missing diffuse texture" << std::endl;
        return 1;
    }
    auto* texture = image::createImage(
        std::string("assets/model/blender_house/") + texturePath.C_Str());
    if (!texture || !texture->mData) {
        image::destroyImage(texture);
        std::cerr << "ASSET_FAILED: texture decode failed" << std::endl;
        return 1;
    }
    std::cout << "ASSET_OK: meshes=" << scene->mNumMeshes
        << " faces=" << faces
        << " root_children=" << scene->mRootNode->mNumChildren
        << " texture=" << texture->mWidth << 'x' << texture->mHeight << std::endl;
    image::destroyImage(texture);

    constexpr uint32_t width = 640, height = 480;
    std::vector<RGBA> pixels(width * height);
    sgl->initSurface(width, height, pixels.data());
    PreparedScene prepared;
    std::cout << "PREPARE_BEGIN" << std::endl;
    if (!prepared.initialize(width, height)) {
        std::cerr << "PREPARE_FAILED" << std::endl;
        return 1;
    }
    std::cout << "PREPARE_OK; DRAW_BEGIN" << std::endl;
    prepared.render();
    const auto colored = std::count_if(pixels.begin(), pixels.end(), [](const RGBA& p) {
        return p.mR || p.mG || p.mB;
    });
    std::cout << "DRAW_END: colored_pixels=" << colored << std::endl;

    // Dump the actual framebuffer; positive BMP height matches screen-space Y.
    std::filesystem::create_directories("out/blender-house-experiment");
    BITMAPFILEHEADER header{};
    BITMAPINFOHEADER info{};
    header.bfType = 0x4d42;
    header.bfOffBits = sizeof(header) + sizeof(info);
    header.bfSize = header.bfOffBits + width * height * 4;
    info.biSize = sizeof(info);
    info.biWidth = width;
    info.biHeight = height;
    info.biPlanes = 1;
    info.biBitCount = 32;
    info.biCompression = BI_RGB;
    std::ofstream output("out/blender-house-experiment/soft_renderer.bmp", std::ios::binary);
    output.write(reinterpret_cast<const char*>(&header), sizeof(header));
    output.write(reinterpret_cast<const char*>(&info), sizeof(info));
    for (const auto& pixel : pixels) {
        const unsigned char bgra[] = {pixel.mB, pixel.mG, pixel.mR, 255};
        output.write(reinterpret_cast<const char*>(bgra), 4);
    }
    if (!output) return 3;
    return colored > 0 && faces == 28 ? 0 : 2;
}
