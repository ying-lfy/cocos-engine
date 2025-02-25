/*
 Copyright (c) 2021 Xiamen Yaji Software Co., Ltd.
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

import { ccclass, editable, serializable } from 'cc.decorator';
import { EDITOR, TEST } from "internal:constants";
import { legacyCC } from '../core/global-exports';
import { Node } from './node';
import { applyTargetOverrides, expandNestedPrefabInstanceNode } from "./prefab/utils";
import { assert } from "../core/platform/debug";
import { updateChildrenForDeserialize } from '../core/utils/jsb-utils';

export const Scene = jsb.Scene;
export type Scene = jsb.Scene;
legacyCC.Scene = Scene;

const sceneProto: any = Scene.prototype;

Object.defineProperty(sceneProto, '_globals', {
    enumerable: true,
    configurable: true,
    get () {
        return this.getSceneGlobals();
    },
    set (v) {
        this._globalRef = v;
        this.setSceneGlobals(v);
    },
});

Object.defineProperty(sceneProto, 'globals', {
    enumerable: true,
    configurable: true,
    get () {
        return this.getSceneGlobals();
    },
});

Object.defineProperty(sceneProto, '_renderScene', {
    enumerable: true,
    configurable: true,
    get () {
        if (!this._renderSceneInternal) {
            this._renderSceneInternal = this.getRenderScene();
        }
        return this._renderSceneInternal;
    }
});

Object.defineProperty(sceneProto, 'renderScene', {
    enumerable: true,
    configurable: true,
    get () {
        if (!this._renderSceneInternal) {
            this._renderSceneInternal = this.getRenderScene();
        }
        return this._renderSceneInternal;
    }
});

sceneProto._ctor = function () {
    Node.prototype._ctor.apply(this, arguments);
    this._inited = false;
    this._renderSceneInternal = null;
    this._globalRef = null;
    this._prefabSyncedInLiveReload = false;
};

sceneProto._onBatchCreated = function (dontSyncChildPrefab: boolean) {
    // Don't invoke Node.prototype._onBatchCreated because we refactor Node&BaseNode, BaseNode is empty just for
    // instanceof check in ts engine. After ts engine removes BaseNode, we could remove BaseNode.h/.cpp too.
    if (this._parent) {
        this._siblingIndex = this._parent.children.indexOf(this);
    }
    //
    const len = this._children.length;
    for (let i = 0; i < len; ++i) {
        this.children[i]._siblingIndex = i;
        this._children[i]._onBatchCreated(dontSyncChildPrefab);
    }
};

const oldLoad = sceneProto._load;
sceneProto._load = function () {
    this._scene = this;
    if (!this._inited) {
        if (TEST) {
            assert(!this._activeInHierarchy, 'Should deactivate ActionManager and EventManager by default');
        }

        expandNestedPrefabInstanceNode(this);
        applyTargetOverrides(this);
        this._onBatchCreated(EDITOR && this._prefabSyncedInLiveReload);
        this._inited = true;
    }
    updateChildrenForDeserialize(this);
    oldLoad.call(this);
};

sceneProto._activate = function (active: boolean) {
    active = (active !== false);
    if (EDITOR) {
        // register all nodes to editor
        this._registerIfAttached!(active);
    }
    legacyCC.director._nodeActivator.activateNode(this, active);
    // The test environment does not currently support the renderer
    if (!TEST) {
        this._globals.activate();
        if (this._renderScene) {
            this._renderScene.activate();
        }
    }
};

// handle meta data, it is generated automatically
const SceneProto = Scene.prototype;
const globalsDescriptor = Object.getOwnPropertyDescriptor(SceneProto, 'globals');
editable(SceneProto, 'globals', globalsDescriptor);
editable(SceneProto, 'autoReleaseAssets');
serializable(SceneProto, 'autoReleaseAssets');
serializable(SceneProto, '_globals');
ccclass('cc.Scene')(Scene);
