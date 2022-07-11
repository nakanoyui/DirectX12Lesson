#pragma once

//===========================================
//
// 便利機能
//
//===========================================

// 算術系短縮名
namespace Math = DirectX::SimpleMath;

template<class T>
void SafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

template<class T>
void SafeDelete(T*& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

//===========================================
//
// 色定数
//
//===========================================
static const Math::Color	WhiteColor = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
static const Math::Color	BlackColor = Math::Color(0.0f, 0.0f, 0.0f, 1.0f);
static const Math::Color	RedColor = Math::Color(1.0f, 0.0f, 0.0f, 1.0f);
static const Math::Color	GreenColor = Math::Color(0.0f, 1.0f, 0.0f, 1.0f);
static const Math::Color	BlueColor = Math::Color(0.0f, 0.0f, 1.0f, 1.0f);
static const Math::Color	NormalColor = Math::Color(0.5f, 0.5f, 1.0f, 1.0f);	// 垂直に伸びる法線情報

//===========================================
//
// ファイル
//
//===========================================

// ファイルパスから、親ディレクトリまでのパスを取得
inline std::string GetDirFromPath(const std::string& path)
{
	const std::string::size_type pos = std::max<signed>(path.find_last_of('/'), path.find_last_of('\\'));
	return (pos == std::string::npos) ? std::string() : path.substr(0, pos + 1);
}