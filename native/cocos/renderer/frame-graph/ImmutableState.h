
/****************************************************************************
 Copyright (c) 2021-2022 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#pragma once

#include "frame-graph/Resource.h"
#include "gfx-base/GFXDef-common.h"

namespace cc {
namespace framegraph {

class DevicePassResourceTable;

struct Range {
    size_t base{0};
    size_t len{0};
};

struct AccessStatus {
    gfx::PassType passType{gfx::PassType::RASTER};
    gfx::ShaderStageFlagBit visibility{gfx::ShaderStageFlagBit::NONE};
    gfx::MemoryAccessBit access{gfx::MemoryAccessBit::NONE};
};

enum class ResourceType : uint32_t {
    UNKNOWN,
    BUFFER,
    TEXTURE,
};

struct ResourceBarrier {
    ResourceType resourceType{ResourceType::UNKNOWN};
    gfx::BarrierType barrierType{gfx::BarrierType::FULL};

    Handle handle;

    AccessStatus beginStatus;
    AccessStatus endStatus;
    Range layerRange;

    union {
        Range mipRange;
        Range bufferRange;
    };
};

struct PassBarrierPair {
    ccstd::vector<ResourceBarrier> frontBarriers;
    ccstd::vector<ResourceBarrier> rearBarriers;
};

using BarriersList = std::vector<PassBarrierPair>;

struct PassBarriers {
    PassBarrierPair blockBarrier;
    BarriersList subpassBarriers;
};

std::pair<gfx::GFXObject* /*barrier*/, gfx::GFXObject* /*resource*/> getBarrier(const ResourceBarrier& barrierInfo, const DevicePassResourceTable* dict) noexcept;

} // namespace framegraph
} // namespace cc
