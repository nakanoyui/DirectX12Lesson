#pragma once

//====================================================
//
// システム系のヘッダーファイル
//
//====================================================

// 便利機能
#include "System/Utility/Utility.h"

// GraphicsDevice
#include "Graphics/GraphicsDevice/GraphicsDevice.h"

// レンダーターゲット
#include "Graphics/RenderTarget/RenderTarget.h"

// テクスチャ
#include "Graphics/Texture/Texture.h"

// レンダーテクスチャ
#include "Graphics/RenderTexture/RenderTexture.h"

// デプスステンシル
#include "Graphics/DepthStencil/DepthStencil.h"

// デプスステンシルテクスチャ
#include "Graphics/DepthStencilTexture/DepthStencilTexture.h"

// パイプライン
#include "Graphics/Shader/Pipeline/Pipeline.h"

// ルートシグネチャ
#include "Graphics/Shader/RootSignature/RootSignature.h"

// メッシュ
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshData/MeshData.h"
#include "Graphics/Mesh/MeshCreater/MeshCreater.h"

// モデル
#include "Graphics/Model/Model.h"

// アニメーション
#include "Graphics/Model/Animation/Animation.h"
#include "Graphics/Model/Animation/AnimationBlend.h"

// シェーダー
#include "Graphics/Shader/Shader.h"

// CBV・SRV・UAVのヒープ
#include "Graphics/CBVSRVUAVHeap/CBVSRVUAVHeap.h"

// 定数バッファーアロケーター
#include "Graphics/CBufferAllocater/CBufferAllocater.h"

// 定数バッファーのパラメーター
#include "Graphics/CBufferAllocater/CBufferStruct/CBufferStruct.h"

// モデル
#include "Graphics/Model/ModelStruct/ModelStruct.h"

// カメラ
#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/FPSCamera.h"
#include "Graphics/Camera/TPSCamera.h"

// ゲームオブジェクト
#include "GameObject/GameObject.h"

// ゲームオブジェクトファクトリー
#include "Application/GameSystem/GameObject/GameObjectFactory.h"

// マネージャー
#include "Manager/CameraManager/CameraManager.h"
#include "Manager/ShaderManager/ShaderManager.h"
#include "Manager/ModelManager/ModelManager.h"
#include "Manager/TextureManager/TextureManager.h"
#include "Manager/DrawManager/DrawManager.h"
#include "Manager/KeyManager/KeyManager.h"
#include "Manager/JsonManager/JsonManager.h"
#include "Manager/CSVManager/CSVManager.h"

//
#include "Math/MyMath.h"
#include "Math/Collider.h"
#include "Math/Collision.h"

// シーンチェンジャー
#include "Scene/SceneChanger.h"

//
#include "SpriteFont/SpriteFont.h"

//
#include "DebugWireFrame/DebugWireFrame.h"

// ウィンドウ
#include "Window/Window.h"