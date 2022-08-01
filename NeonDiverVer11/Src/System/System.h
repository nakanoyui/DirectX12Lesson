#pragma once

//====================================================
//
// システム系のヘッダーファイル
//
//====================================================

// 便利機能
#include "System/Utility/Utility.h"
#include "System/Utility/RandomMaker.h"

// GraphicsDevice
#include "Graphics/GraphicsDevice.h"

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
#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/AnimationBlend.h"

// シェーダー
#include "Graphics/Shader/Shader.h"

// CBV・SRV・UAVのヒープ
#include "Graphics/CBVSRVUAVHeap/CBVSRVUAVHeap.h"

// 定数バッファーアロケーター
#include "Graphics/CBufferAllocater/CBufferAllocater.h"

// 定数バッファーのパラメーター
#include "Graphics/CBufferAllocater/CBufferStruct/CBufferStruct.h"

// カメラ
#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/FPSCamera.h"
#include "Graphics/Camera/TPSCamera.h"

// ゲームオブジェクト
#include "GameObject/GameObject.h"

// マネージャー
#include "Manager/CameraManager/CameraManager.h"
#include "Manager/AssetManager/AssetManager.h"
#include "Manager/DrawManager/DrawManager.h"
#include "Manager/KeyManager/KeyManager.h"
#include "Manager/JsonManager/JsonManager.h"
#include "System/Manager/ImGuiManager/ImGuiManager.h"
#include "System/Manager/ImGuiManager/ImGuiTexture.h"

//
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

//
#include "Math/MyMath.h"
#include "Math/Collider.h"
#include "Math/Collision.h"

// Component
#include "Component/Component.h"
#include "Component/ScripteComponent/ScriptComponent.h"
#include "Component/TransformComponent/TransformComponent.h"
#include "Component/ModelComponent/ModelComponent.h"
#include "Component/CameraComponent/CameraComponent.h"
#include "Component/RendererComponent/SpriteRendererComponent.h"
#include "Component/RendererComponent/StandardRendererComponent.h"
#include "Component/RendererComponent/SkinMeshRendererComponent.h"
#include "Component/ColliderComponent/SphereColliderComponent.h"
#include "Component/ColliderComponent/BoxColliderComponent.h"
#include "Component/ColliderComponent/ModelColliderComponent.h"
#include "Component/ColliderComponent/RigidBodyComponent.h"

// GPUParticle
#include "GPUParticle/ComputePipeline/ComputePipeline.h"

//
#include "SpriteFont/SpriteFont.h"

//
#include "DebugWireFrame/DebugWireFrame.h"

// ウィンドウ
#include "Window/Window.h"