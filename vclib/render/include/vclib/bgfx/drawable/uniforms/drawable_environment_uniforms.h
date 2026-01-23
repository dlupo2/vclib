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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

namespace vcl {

class DrawableEnvironmentUniforms
{
    mutable std::array<float, 4> mData0 = {
        0.0, // exposure
        0.0, // tone mapping
        0.0, // roughness
        0.0  // cube side
    };

    mutable std::array<float, 4> mData1 = {
        0.0, // specular mip levels
        0.0, // distribution model
        0.0,
        0.0
    };

    Uniform mDataUniform0 =
        Uniform("u_environmentSettingsPack0", bgfx::UniformType::Vec4);

    Uniform mDataUniform1 =
        Uniform("u_environmentSettingsPack1", bgfx::UniformType::Vec4);

public:
    DrawableEnvironmentUniforms() = default;

    void updateExposure(float exposure) const { mData0[0] = exposure; }

    void updateToneMapping(PBRViewerSettings::ToneMapping tm) const
    { mData0[1] = float(tm);}

    void updateRoughness(float roughness) { mData0[2] = roughness; }

    void updateCubeSideResolution(float cubeSide) { mData0[3] = cubeSide; }

    void updateSpecularMipsLevels(uint8_t specMips)
    {mData1[0] = float(specMips);}

    void updateDistributionModel(uint8_t model)
    {mData1[1] = float(model);}

    void bind() const 
    {
        mDataUniform0.bind(mData0.data()); 
        mDataUniform1.bind(mData1.data()); 
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
