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

#ifndef VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH

uniform vec4 u_baseColorFactor;
uniform vec4 u_FactorsPack;
uniform vec4 u_emissivePack;
uniform vec4 u_sheenFactorsPack;
uniform vec4 u_alphaPack;
uniform vec4 u_settings;

uniform vec4 u_baseColorTexUvScaleTrans;
uniform vec4 u_metallicRoughnessTexUvScaleTrans;
uniform vec4 u_normalTexUvScaleTrans;
uniform vec4 u_occlusionTexUvScaleTrans;
uniform vec4 u_emissiveTexUvScaleTrans;
uniform vec4 u_sheenColorTexUvScaleTrans;
uniform vec4 u_sheenRoughnessTexUvScaleTrans;

uniform vec4 u_uvRotationPack0;
uniform vec4 u_uvRotationPack1;

#define u_occlusionStrength u_FactorsPack.r
#define u_roughnessFactor u_FactorsPack.g
#define u_metallicFactor u_FactorsPack.b
#define u_normalScale u_FactorsPack.a

#define u_emissiveFactor u_emissivePack.rgb
#define u_emissiveStrength u_emissivePack.a

#define u_sheenColorFactor u_sheenFactorsPack.rgb
#define u_sheenRoughnessFactor u_sheenFactorsPack.a

#define u_alphaCutoff u_alphaPack.x

#define u_pbr_settings floatBitsToUint(u_settings.x)
#define u_pbr_texture_settings floatBitsToUint(u_settings.y)

#define u_baseColorUvTransform         getUvTransform(u_baseColorTexUvScaleTrans.xy, u_baseColorTexUvScaleTrans.zw, u_uvRotationPack0.x)
#define u_metallicRoughnessUvTransform getUvTransform(u_metallicRoughnessTexUvScaleTrans.xy, u_metallicRoughnessTexUvScaleTrans.zw, u_uvRotationPack0.y)
#define u_normalUvTransform            getUvTransform(u_normalTexUvScaleTrans.xy, u_normalTexUvScaleTrans.zw, u_uvRotationPack0.z)
#define u_occlusionUvTransform         getUvTransform(u_occlusionTexUvScaleTrans.xy, u_occlusionTexUvScaleTrans.zw, u_uvRotationPack0.w)
#define u_emissiveUvTransform          getUvTransform(u_emissiveTexUvScaleTrans.xy, u_emissiveTexUvScaleTrans.zw, u_uvRotationPack1.x)
#define u_sheenColorUvTransform        getUvTransform(u_sheenColorTexUvScaleTrans.xy, u_sheenColorTexUvScaleTrans.zw, u_uvRotationPack1.y)
#define u_sheenRoughnessUvTransform    getUvTransform(u_sheenRoughnessTexUvScaleTrans.xy, u_sheenRoughnessTexUvScaleTrans.zw, u_uvRotationPack1.z)
#endif // VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH
