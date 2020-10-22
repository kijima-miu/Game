#include "stdafx.h"
#include "Player.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"


Player::Player()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/Kari/Karimodel.cmo");
	//アニメーション
	//tkaファイルの読み込み。
	g_upAnimationClips[0].Load(L"Assets/animData/Karianim/yura.tka");
	g_upAnimationClips[0].SetLoopFlag(true);
	g_upAnimationClips[1].Load(L"Assets/animData/Karianim/updown.tka");
	g_upAnimationClips[1].SetLoopFlag(true);
	//アニメーションの初期化。
	g_upAnimation.Init(
		m_model,			//アニメーションを流すスキンモデル。
									//これでアニメーションとスキンモデルが関連付けされる。
		g_upAnimationClips,	//アニメーションクリップの配列。
		2,							//アニメーションクリップの数。
		true,
		L"Assets/modelData/Kari/Karimodel.cmo"
	);

	g_lowAnimationClips[0].Load(L"Assets/animData/Karianim/yura.tka");
	g_lowAnimationClips[0].SetLoopFlag(true);
	g_lowAnimationClips[1].Load(L"Assets/animData/Karianim/updown.tka");
	g_lowAnimationClips[1].SetLoopFlag(true);
	//アニメーションの初期化。
	g_lowAnimation.Init(
		m_model,			//アニメーションを流すスキンモデル。
									//これでアニメーションとスキンモデルが関連付けされる。
		g_lowAnimationClips,	//アニメーションクリップの配列。
		2,							//アニメーションクリップの数。
		false,
		L"Assets/modelData/Kari/Karimodel.cmo"
	);

	unityRot.SetRotationDeg({1,0,0},90.0f);
}


Player::~Player()
{
	
}

void Player::Update()
{
	//左右移動
	if (g_pad[0].IsPress(enButtonLeft))
	{
		m_pos.x += 1.0f;
	}
	if (g_pad[0].IsPress(enButtonRight))
	{
		m_pos.x -= 1.0f;
	}
	if (g_pad[0].IsPress(enButtonUp))
	{
		m_pos.z -= 1.0f;
	}
	if (g_pad[0].IsPress(enButtonDown))
	{
		m_pos.z += 1.0f;
	}

	//アニメーション
	if (g_pad[0].IsPress(enButtonA))
	{
		g_upAnimation.Play(1, 0.5f);
	}
	else if (g_pad[0].IsPress(enButtonB))
	{
		g_lowAnimation.Play(0, 0.5f);
	}
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_pos, /*CQuaternion::Identity()*/unityRot, CVector3::One());
	g_upAnimation.Update(1.0f / 30.0f);
	g_lowAnimation.Update(1.0f / 30.0f);
}
void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}