/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H

#include <vclib/bgfx/drawable/mesh/pbr_macros.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/draw_object_settings.h>

#include <vclib/mesh.h>

namespace vcl {

class MaterialUniforms
{
    static const uint N_TEXTURES = toUnderlying(Material::TextureType::COUNT);

    std::array<float, 4> mBaseColor = {1.0, 1.0, 1.0, 1.0};

    // metallic, roughness and occlusion are stored in the B, G and R channels
    // respectively for consistency with textures
    std::array<float, 4> mFactorsPack = {
        1.0, // occlusion strength
        1.0, // roughness
        1.0, // metallic
        1.0  // normal scale
    };

    // emissive color factor stored in RGB channels, alpha channel is unused so
    // it can be used to store the emissive strength
    std::array<float, 4> mEmissivePack = {0.0, 0.0, 0.0, 1.0};

    // sheen color RGB and roughness A
    std::array<float, 4> mSheenFactorsPack = {0.0, 0.0, 0.0, 0.0};

    // settings packed in a vec4
    // .x : pbr settings
    // .y : texture settings
    // .z : tone mapping operator
    // .w : exposure
    std::array<float, 4> mSettings = {0.0, 0.0, 2.0, 1.0};

    // alpha cutoff and maybe other alpha related settings can be stored here
    std::array<float, 4> mAlphaPack = {0.5, 0.0, 0.0, 0.0};

    std::array<std::array<float, 4>, N_TEXTURES>
        mTextureUvScaleTrans = // uv scale and translation for each texture
        { 
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // base color
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // metallic-roughness
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // normal
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // occlusion
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // emissive
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}, // sheen color
            std::array<float, 4>{1.0, 1.0, 0.0, 0.0}  // sheen roughness
        };

    constexpr static uint N_UV_ROTATION_PACKS =
        N_TEXTURES / 4 + static_cast<uint>(N_TEXTURES % 4 != 0);

    std::array<std::array<float, 4>, N_UV_ROTATION_PACKS> 
        mTextureUvRotations = {
            std::array<float, 4>{0.0, 0.0, 0.0, 0.0}, // rotations for textures 0-3
            std::array<float, 4>{0.0, 0.0, 0.0, 0.0}  // rotations for texture 4-6
        };

    Uniform mBaseColorUniform =
        Uniform("u_baseColorFactor", bgfx::UniformType::Vec4);

    Uniform mFactorsPackUniform =
        Uniform("u_FactorsPack", bgfx::UniformType::Vec4);

    Uniform mEmissivePackUniform =
        Uniform("u_emissivePack", bgfx::UniformType::Vec4);

    Uniform mSheenFactorsPackUniform =
        Uniform("u_sheenFactorsPack", bgfx::UniformType::Vec4);

    Uniform mSettingsUniform = Uniform("u_settings", bgfx::UniformType::Vec4);

    Uniform mAlphaPackUniform =
        Uniform("u_alphaPack", bgfx::UniformType::Vec4);

    std::array<Uniform, N_TEXTURES> mTextureUvScaleTransUniforms = {
        Uniform("u_baseColorTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_metallicRoughnessTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_normalTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_occlusionTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_emissiveTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_sheenColorTexUvScaleTrans", bgfx::UniformType::Vec4),
        Uniform("u_sheenRoughnessTexUvScaleTrans", bgfx::UniformType::Vec4)
    };

    Uniform mUvRotationPack0Uniform =
        Uniform("u_uvRotationPack0", bgfx::UniformType::Vec4);

    Uniform mUvRotationPack1Uniform =
        Uniform("u_uvRotationPack1", bgfx::UniformType::Vec4);

public:
    MaterialUniforms() = default;

    const std::array<float, 4>& currentBaseColor() const { return mBaseColor; }

    const std::array<float, 4>& currentFactorsPack() const
    {
        return mFactorsPack;
    }

    const std::array<float, 4>& currentEmissivePack() const
    {
        return mEmissivePack;
    }

    const std::array<float, 4>& currentSettings() const { return mSettings; }

    void update(
        const Material&                     m,
        bool                                vertexColorAvailable,
        const std::array<bool, N_TEXTURES>& textureAvailable,
        bool                                vertexTangentAvailable,
        const PBRViewerSettings&            pbrSettings,
        const Environment*                  environment)
    {
        uint pbrSettingBits = 0;

        if (vertexColorAvailable) // per-vertex color available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_COLOR;

        if (vertexTangentAvailable) // per-vertex tangent available
            pbrSettingBits |= 1 << VCL_PBR_VERTEX_TANGENT;

        if (m.alphaMode() ==
            Material::AlphaMode::ALPHA_MASK) { // alpha mode is MASK
            pbrSettingBits |= 1 << VCL_PBR_IS_ALPHA_MODE_MASK;
            mAlphaPack[0] = m.alphaCutoff();
        }

        if (environment != nullptr && environment->canDraw()) {
            pbrSettingBits |= 1 << VCL_PBR_IMAGE_BASED_LIGHTING;
        }

        mSettings[0] = Uniform::uintBitsToFloat(pbrSettingBits);

        uint textureSettings = 0;

        for (int i = 0; i < N_TEXTURES; ++i) {
            if (textureAvailable[i]) {
                // texture available, uses settings from 0 to N_TEXTURES
                textureSettings |= 1 << (VCL_PBR_TEXTURE_BASE_COLOR + i);

                const TextureDescriptor& td = m.textureDescriptor(
                    static_cast<Material::TextureType>(i));

                // set texture UV scale and translation uniforms
                mTextureUvScaleTrans[i][0] = td.scale().x();
                mTextureUvScaleTrans[i][1] = td.scale().y();
                mTextureUvScaleTrans[i][2] = td.offset().x();
                mTextureUvScaleTrans[i][3] = td.offset().y();

                mTextureUvRotations[i / 4][i % 4] = td.rotation();
            }
        }

        mSettings[1] = Uniform::uintBitsToFloat(textureSettings);
        mSettings[2] =
            Uniform::uintBitsToFloat(toUnderlying(pbrSettings.toneMapping));
        mSettings[3] = pbrSettings.exposure;

        mBaseColor[0] = m.baseColor().redF();
        mBaseColor[1] = m.baseColor().greenF();
        mBaseColor[2] = m.baseColor().blueF();
        mBaseColor[3] = m.baseColor().alphaF();

        // metallic, roughness and occlusion are stored in the B, G and R
        // channels respectively for consistency with textures
        mFactorsPack[0] = m.occlusionStrength();
        mFactorsPack[1] = m.roughness();
        mFactorsPack[2] = m.metallic();
        mFactorsPack[3] = m.normalScale();

        mEmissivePack[0] = m.emissiveColor().redF();
        mEmissivePack[1] = m.emissiveColor().greenF();
        mEmissivePack[2] = m.emissiveColor().blueF();
        mEmissivePack[3] = m.emissiveStrength();

        mSheenFactorsPack[0] = m.sheenColor().redF();
        mSheenFactorsPack[1] = m.sheenColor().greenF();
        mSheenFactorsPack[2] = m.sheenColor().blueF();
        mSheenFactorsPack[3] = m.sheenRoughness();
    }

    void bind() const
    {
        mBaseColorUniform.bind(&mBaseColor);
        mFactorsPackUniform.bind(&mFactorsPack);
        mEmissivePackUniform.bind(&mEmissivePack);
        mSheenFactorsPackUniform.bind(&mSheenFactorsPack);
        mSettingsUniform.bind(&mSettings);
        mAlphaPackUniform.bind(&mAlphaPack);

        for(int i=0; i<N_TEXTURES; ++i)
            mTextureUvScaleTransUniforms[i].bind(&(mTextureUvScaleTrans[i]));
            
        mUvRotationPack0Uniform.bind(&(mTextureUvRotations[0]));
        mUvRotationPack1Uniform.bind(&(mTextureUvRotations[1]));
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_H
