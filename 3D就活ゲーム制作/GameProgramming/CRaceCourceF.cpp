#include "CRaceCourceF.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CSceneTitle.h"

void CRaceCourceF::Init(){
	//シーンの設定
	mScene = ERACE6;

	CSceneRace::Init();

	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//カメラの生成
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();
	//敵車の生成
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		mEnemys[i] = new CEnemy();
		if (i % 8 == 0){
			mEnemys[i]->mpModel = &mCarBlue;
		}
		else if (i % 8 == 1){
			mEnemys[i]->mpModel = &mCarPink;
		}
		else if (i % 8 == 2){
			mEnemys[i]->mpModel = &mCarRed;
		}
		else if (i % 8 == 3){
			mEnemys[i]->mpModel = &mCarGreen;
		}
		else if (i % 8 == 4){
			mEnemys[i]->mpModel = &mCarYellow;
		}
		else if (i % 8 == 5){
			mEnemys[i]->mpModel = &mCarBlack;
		}
		else if (i % 8 == 6){
			mEnemys[i]->mpModel = &mCarGray;
		}
		else if (i % 8 == 7){
			mEnemys[i]->mpModel = &mCarCyan;
		}
		//初期配置座標を設定する
		mEnemys[i]->mPosition = CVector(0.0f - 30.0f - 80.0f*i, -13.538f, 0.0f);
		mEnemys[i]->mRotation.mY = 90.0f;
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mZ += 80.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	printf("※開発中\n");
	//中間地点(順に通らないと1周したことにならないし、順番を飛ばしてもいけない)
	new CObjCheckPoint(&mCheckPoint, CVector(1127.4f, -100.0f, -5054.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(777.0f, -100.0f, 1925.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(-5861.0f, -300.0f, 1165.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 3);
	//ゴール地点
	new CObjNonCol(&mMiniGoal, CVector(140.0f, -159.0f, 90.0f - 30.0f), CVector(0.0f, 90.0f, 0.0f), CVector(5.1f, 10.0f, 5.1f));

	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
}
void CRaceCourceF::Update(){
	CSceneRace::Update();

}