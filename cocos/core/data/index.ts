/*
 Copyright (c) 2018-2020 Xiamen Yaji Software Co., Ltd.

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
*/

import * as _decorator from './class-decorator';
import { legacyCC } from '../global-exports';

legacyCC._decorator = _decorator;
export { _decorator };
export { CCClass, ENUM_TAG, BITMASK_TAG, isCCClassOrFastDefined } from './class';
export { CCObject, isCCObject, isValid } from './object';
export { CCInteger, CCFloat, CCBoolean, CCString } from './utils/attribute';
export { CompactValueTypeArray } from './utils/compact-value-type-array';
export { editorExtrasTag } from './editor-extras-tag';
export * from './utils/asserts';
export * from './utils/compiler';
export { deserializeTag, serializeTag } from './custom-serializable';
export type {
    SerializationInput,
    SerializationOutput,
    SerializationContext,
    CustomSerializable,
    DeserializationContext, // engine internal exports
} from './custom-serializable';
export { getSerializationMetadata } from './serialization-metadata';
export type { SerializationMetadata } from './serialization-metadata';

// engine internal exports
export { EditorExtendable } from './editor-extendable';
export { garbageCollectionManager } from './garbage-collection';
export { GCObject } from './gc-object';
