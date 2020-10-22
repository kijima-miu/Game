#pragma once
#include "character/CharacterController.h"
#include "GameObjectManager.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
private:
	SkinModel m_model;									//スキンモデル。
	CVector3 m_pos = CVector3::Zero();
	CQuaternion unityRot;

	Animation g_upAnimation;				//アニメーション。
	AnimationClip g_upAnimationClips[2];	//アニメーションクリップ。
	Animation g_lowAnimation;				//アニメーション。
	AnimationClip g_lowAnimationClips[2];	//アニメーションクリップ。
};

