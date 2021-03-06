/*!
 * @brief	アニメータークラス。
 */


#include "stdafx.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton.h"
#include "graphics/skinModel.h"

#include <corecrt_wstring.h>
#include <string>

Animation::Animation()
{
}
Animation::~Animation()
{
	
}
	
void Animation::Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip, bool Up, const wchar_t* filePath)
{
	if (animClipList == nullptr) {
#ifdef _DEBUG
		char message[256];
		strcpy(message, "animClipListがNULLです。\n");
		OutputDebugStringA(message);
		//止める。
		std::abort();
#endif
		
	}
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);
	//m_skeleton = &skinModel.GetSkeleton();
	int BoneNum = m_skeleton->GetNumBones();
	//Upがtrueなら上のボーンを探す
	if (Up == true) {
		//ボーンの名前取得
		m_NameBone = m_bone->GetName();
		for (int i = 0; i > BoneNum; i++) {
			//検索


			//int UpNum = m_NameBone.rfind("_u");
		}

	}
	//Upがflseなら下のボーン
	else {
		//ボーンの名前取得
		m_NameBone = m_bone->GetName();
		//検索


	}


	for (int i = 0; i < numAnimClip; i++) {
		m_animationClips.push_back(&animClipList[i]);
	}
	for (auto& ctr : m_animationPlayController) {
		ctr.Init(m_skeleton);
	}
		
	Play(0);
}
/*!
* @brief	ローカルポーズの更新。
*/
void Animation::UpdateLocalPose(float deltaTime)
{
	m_interpolateTime += deltaTime;
	if (m_interpolateTime >= 1.0f) {
		//補間完了。
		//現在の最終アニメーションコントローラへのインデックスが開始インデックスになる。
		m_startAnimationPlayController = GetLastAnimationControllerIndex();
		m_numAnimationPlayController = 1;
		m_interpolateTime = 1.0f;
	}
	//AnimationPlayController::Update関数を実行していく。
	for (int i = 0; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i );
		m_animationPlayController[index].Update(deltaTime, this);
	}
}

void Animation::UpdateGlobalPose()
{
	//グローバルポーズ計算用のメモリをスタックから確保。
	int numBone = m_skeleton->GetNumBones();
	CQuaternion* qGlobalPose = (CQuaternion*)alloca(sizeof(CQuaternion) * numBone);
	CVector3* vGlobalPose = (CVector3*)alloca(sizeof(CVector3) * numBone);
	CVector3* vGlobalScale = (CVector3*)alloca(sizeof(CVector3) * numBone);
	for (int i = 0; i < numBone; i++) {
		qGlobalPose[i] = CQuaternion::Identity();
		vGlobalPose[i] = CVector3::Zero();
		vGlobalScale[i] = CVector3::One();
	}
	//グローバルポーズを計算していく。
	int startIndex = m_startAnimationPlayController;
	for (int i = 0; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
		const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//平行移動の補完
			CMatrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(
				intepolateRate, 
				vGlobalPose[boneNo], 
				*(CVector3*)m.m[3]
			);
			//平行移動成分を削除。
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;
			
			//拡大成分の補間。
			CVector3 vBoneScale;
			vBoneScale.x = (*(CVector3*)m.m[0]).Length();
			vBoneScale.y = (*(CVector3*)m.m[1]).Length();
			vBoneScale.z = (*(CVector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(
				intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale
			);
			//拡大成分を除去。
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;
				
			//回転の補完
			CQuaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//グローバルポーズをスケルトンに反映させていく。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		
		//拡大行列を作成。
		CMatrix scaleMatrix;
		scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
		//回転行列を作成。
		CMatrix rotMatrix;
		rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
		//平行移動行列を作成。
		CMatrix transMat;
		transMat.MakeTranslation(vGlobalPose[boneNo]);

		//全部を合成して、ボーン行列を作成。
		CMatrix boneMatrix;
		boneMatrix.Mul(scaleMatrix, rotMatrix);
		boneMatrix.Mul(boneMatrix, transMat);
		
		m_skeleton->SetBoneLocalMatrix(
			boneNo,
			boneMatrix
		);			
	}
		
	//最終アニメーション以外は補間完了していたら除去していく。
	int numAnimationPlayController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
			//補間が終わっているのでアニメーションの開始位置を前にする。
			m_startAnimationPlayController = index;
			numAnimationPlayController = m_numAnimationPlayController - i;
		}
	}
	m_numAnimationPlayController = numAnimationPlayController;
}
	
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if (result == false) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
	
void Animation::Update(float deltaTime)
{
	if (m_numAnimationPlayController == 0) {
		return;
	}
	//ローカルポーズの更新をやっていく。
	UpdateLocalPose(deltaTime);
		
	//グローバルポーズを計算していく。
	UpdateGlobalPose();
}
