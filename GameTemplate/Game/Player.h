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
	SkinModel m_model;									//�X�L�����f���B
	CVector3 m_pos = CVector3::Zero();
	CQuaternion unityRot;

	Animation g_upAnimation;				//�A�j���[�V�����B
	AnimationClip g_upAnimationClips[2];	//�A�j���[�V�����N���b�v�B
	Animation g_lowAnimation;				//�A�j���[�V�����B
	AnimationClip g_lowAnimationClips[2];	//�A�j���[�V�����N���b�v�B
};

