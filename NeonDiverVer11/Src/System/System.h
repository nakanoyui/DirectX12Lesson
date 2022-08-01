#pragma once

//====================================================
//
// �V�X�e���n�̃w�b�_�[�t�@�C��
//
//====================================================

// �֗��@�\
#include "System/Utility/Utility.h"
#include "System/Utility/RandomMaker.h"

// GraphicsDevice
#include "Graphics/GraphicsDevice.h"

// �����_�[�^�[�Q�b�g
#include "Graphics/RenderTarget/RenderTarget.h"

// �e�N�X�`��
#include "Graphics/Texture/Texture.h"

// �����_�[�e�N�X�`��
#include "Graphics/RenderTexture/RenderTexture.h"

// �f�v�X�X�e���V��
#include "Graphics/DepthStencil/DepthStencil.h"

// �f�v�X�X�e���V���e�N�X�`��
#include "Graphics/DepthStencilTexture/DepthStencilTexture.h"

// �p�C�v���C��
#include "Graphics/Shader/Pipeline/Pipeline.h"

// ���[�g�V�O�l�`��
#include "Graphics/Shader/RootSignature/RootSignature.h"

// ���b�V��
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshData/MeshData.h"
#include "Graphics/Mesh/MeshCreater/MeshCreater.h"

// ���f��
#include "Graphics/Model/Model.h"

// �A�j���[�V����
#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/AnimationBlend.h"

// �V�F�[�_�[
#include "Graphics/Shader/Shader.h"

// CBV�ESRV�EUAV�̃q�[�v
#include "Graphics/CBVSRVUAVHeap/CBVSRVUAVHeap.h"

// �萔�o�b�t�@�[�A���P�[�^�[
#include "Graphics/CBufferAllocater/CBufferAllocater.h"

// �萔�o�b�t�@�[�̃p�����[�^�[
#include "Graphics/CBufferAllocater/CBufferStruct/CBufferStruct.h"

// �J����
#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/FPSCamera.h"
#include "Graphics/Camera/TPSCamera.h"

// �Q�[���I�u�W�F�N�g
#include "GameObject/GameObject.h"

// �}�l�[�W���[
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

// �E�B���h�E
#include "Window/Window.h"