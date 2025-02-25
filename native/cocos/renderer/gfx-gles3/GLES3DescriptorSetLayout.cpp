/****************************************************************************
 Copyright (c) 2019-2022 Xiamen Yaji Software Co., Ltd.

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

#include "GLES3Std.h"

#include "GLES3Commands.h"
#include "GLES3DescriptorSetLayout.h"

namespace cc {
namespace gfx {

GLES3DescriptorSetLayout::GLES3DescriptorSetLayout() {
    _typedID = generateObjectID<decltype(this)>();
}

GLES3DescriptorSetLayout::~GLES3DescriptorSetLayout() {
    destroy();
}

void GLES3DescriptorSetLayout::doInit(const DescriptorSetLayoutInfo & /*info*/) {
    _gpuDescriptorSetLayout = ccnew GLES3GPUDescriptorSetLayout;
    _gpuDescriptorSetLayout->descriptorCount = _descriptorCount;
    _gpuDescriptorSetLayout->bindingIndices = _bindingIndices;
    _gpuDescriptorSetLayout->descriptorIndices = _descriptorIndices;
    _gpuDescriptorSetLayout->bindings = _bindings;

    for (auto &binding : _bindings) {
        if (hasAnyFlags(binding.descriptorType, DESCRIPTOR_DYNAMIC_TYPE)) {
            for (uint32_t j = 0U; j < binding.count; j++) {
                _gpuDescriptorSetLayout->dynamicBindings.push_back(binding.binding);
            }
        }
    }
}

void GLES3DescriptorSetLayout::doDestroy() {
    CC_SAFE_DELETE(_gpuDescriptorSetLayout);
}

} // namespace gfx
} // namespace cc
