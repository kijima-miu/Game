#include "stdafx.h"
#include "Player.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"


Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/Kari/Karimodel.cmo");
	//�A�j���[�V����
	//tka�t�@�C���̓ǂݍ��݁B
	g_upAnimationClips[0].Load(L"Assets/animData/Karianim/yura.tka");
	g_upAnimationClips[0].SetLoopFlag(true);
	g_upAnimationClips[1].Load(L"Assets/animData/Karianim/updown.tka");
	g_upAnimationClips[1].SetLoopFlag(true);
	//�A�j���[�V�����̏������B
	g_upAnimation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
									//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		g_upAnimationClips,	//�A�j���[�V�����N���b�v�̔z��B
		2,							//�A�j���[�V�����N���b�v�̐��B
		true,
		L"Assets/modelData/Kari/Karimodel.cmo"
	);

	g_lowAnimationClips[0].Load(L"Assets/animData/Karianim/yura.tka");
	g_lowAnimationClips[0].SetLoopFlag(true);
	g_lowAnimationClips[1].Load(L"Assets/animData/Karianim/updown.tka");
	g_lowAnimationClips[1].SetLoopFlag(true);
	//�A�j���[�V�����̏������B
	g_lowAnimation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
									//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		g_lowAnimationClips,	//�A�j���[�V�����N���b�v�̔z��B
		2,							//�A�j���[�V�����N���b�v�̐��B
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
	//���E�ړ�
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

	//�A�j���[�V����
	if (g_pad[0].IsPress(enButtonA))
	{
		g_upAnimation.Play(1, 0.5f);
	}
	else if (g_pad[0].IsPress(enButtonB))
	{
		g_lowAnimation.Play(0, 0.5f);
	}
	//���[���h�s��̍X�V�B
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