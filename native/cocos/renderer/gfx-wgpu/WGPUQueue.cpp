/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

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

#include "WGPUQueue.h"
#include <webgpu/webgpu.h>
#include "WGPUCommandBuffer.h"
#include "WGPUDevice.h"
#include "WGPUObject.h"
#include "WGPUUtils.h"

namespace cc {
namespace gfx {

using namespace emscripten;

CCWGPUQueue::CCWGPUQueue() : Queue() {
}

CCWGPUQueue::~CCWGPUQueue() {
    doDestroy();
}

void CCWGPUQueue::doInit(const QueueInfo &info) {
    _gpuQueueObject = ccnew CCWGPUQueueObject;
    _gpuQueueObject->type = info.type;
    _gpuQueueObject->wgpuQueue = wgpuDeviceGetQueue(CCWGPUDevice::getInstance()->gpuDeviceObject()->wgpuDevice);
}

void CCWGPUQueue::doDestroy() {
    if (_gpuQueueObject) {
        if (_gpuQueueObject->wgpuQueue) {
            wgpuQueueRelease(_gpuQueueObject->wgpuQueue);
        }
        delete _gpuQueueObject;
        _gpuQueueObject = nullptr;
    }
}

namespace {

void wgpuQueueSubmitCallback(WGPUQueueWorkDoneStatus status, void *userdata) {
    auto *recycleBin = static_cast<CCWGPURecycleBin *>(userdata);
    recycleBin->bufferBin.purge();
    recycleBin->textureBin.purge();
    recycleBin->queryBin.purge();
}
} // namespace

void CCWGPUQueue::submit(CommandBuffer *const *cmdBuffs, uint32_t count) {
    // ccstd::vector<WGPUCommandBuffer> commandBuffs(count);
    // for (size_t i = 0; i < count; i++) {
    //     auto* commandBuff = static_cast<CCWGPUCommandBuffer*>(cmdBuffs[i]);
    //     commandBuffs[i]   = commandBuff->gpuCommandBufferObject()->wgpuCommandBuffer;
    // }
    // wgpuQueueSubmit(_gpuQueueObject->wgpuQueue, count, commandBuffs.data());
    // for (size_t i = 0; i < count; i++) {
    //     auto* commandBuff = static_cast<CCWGPUCommandBuffer*>(cmdBuffs[i]);
    //     wgpuCommandBufferRelease(commandBuff->gpuCommandBufferObject()->wgpuCommandBuffer);
    // }

    // CCWGPUDevice::getInstance()->stagingBuffer()->unmap();

    ccstd::vector<WGPUCommandBuffer> wgpuCmdBuffs(count);
    for (size_t i = 0; i < count; ++i) {
        const auto *cmdBuff = static_cast<CCWGPUCommandBuffer *>(cmdBuffs[i]);
        wgpuCmdBuffs[i] = cmdBuff->gpuCommandBufferObject()->wgpuCommandBuffer;

        _numDrawCalls += cmdBuff->getNumDrawCalls();
        _numInstances += cmdBuff->getNumInstances();
        _numTriangles += cmdBuff->getNumTris();
    }

    wgpuQueueSubmit(_gpuQueueObject->wgpuQueue, count, wgpuCmdBuffs.data());
    std::for_each(wgpuCmdBuffs.begin(), wgpuCmdBuffs.end(), [](auto wgpuCmdBuffer) { wgpuCommandBufferRelease(wgpuCmdBuffer); });

    auto *recycleBin = CCWGPUDevice::getInstance()->recycleBin();
    wgpuQueueOnSubmittedWorkDone(_gpuQueueObject->wgpuQueue, 0, wgpuQueueSubmitCallback, recycleBin);
}

} // namespace gfx
} // namespace cc
