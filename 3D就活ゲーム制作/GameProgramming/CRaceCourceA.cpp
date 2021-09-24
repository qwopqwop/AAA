#include "CRaceCourceA.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CRoadManager.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CObjBoost.h"
#include "CObjGrass.h"
#include "CObjJumper.h"
#include "CSceneTitle.h"

void CRaceCourceA::Init(){
	//シーンの設定
	mScene = ERACE1;

	CSceneRace::Init();
		
	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//プレイヤーのリスポーンするCheckPointの設定
	mPlayer->SetRespawnPoint(0, CVector(350.0f, -13.538f, -130.0f), CVector(0.0f, 0.0f, 0.0f));
	mPlayer->SetRespawnPoint(1, CVector(-80.0f, -13.538f, 2175.0f), CVector(0.0f, -55.0f, 0.0f));
	mPlayer->SetRespawnPoint(2, CVector(-1620.0f, -13.538f, 450.0f), CVector(0.0f, -175.0f, 0.0f));
	mPlayer->SetRespawnPoint(3, CVector(-1212.0f, -13.538f, -1616.0f), CVector(0.0f, 120.0f, 0.0f));
	mPlayer->GetReady();
	//カメラの生成
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();
	//コースの生成//床と壁で分割して処理を区別する
	//コース全体のサイズ感を設定
	float mtsize = 50.0f;
	float height = 2.0f;
	new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, 1.0f), 500.0f, 0.0f);
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
		//初期の配置座標を設定する
		mEnemys[i]->mPosition = CVector(350.0f, -13.538f, -130.0f - 80.0f*i);
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mX -= 80.0f;
		}
		mEnemys[i]->CCharacter::Update();

		if (i == 0){
			mEnemys[i]->mEnemyAI = CEnemy::ENEWBIE;
		}		
	}
	/*透明度の高い物から先に描画する*/
	//中間地点(1周と判定されるには順番通りに通過する必要がある)
	new CObjCheckPoint(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(2000.0f, 31.0f, 255.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 31.0f, 255.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 31.0f, 255.0f), 3);
	//ゴール地点
	new CObjCheckPoint(&mCheckPoint, CVector(600.0f, -30.0f, 1.9f), CVector(), CVector(800.0f, 100.0f, 10.0f), 9);

	//コースの生成
	for (int i = 0; i < 1; i++){		
		//芝生の生成(通行中は速度低下)
		new CObjGrass(&mGrass01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
		//コースに柵の配置(壁扱い)
		new CObjFloor(&mFenceTop, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
		new CObjWall(&mFenceSide, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
		//道路と芝生の境目のタイルを生成(当たり判定無し)
		new CObjNonCol(&mCurb01, CVector(-360.0f, 5.0f - 33.0f + 0.05f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
	}
	//白・黒タイルでゴール示唆
	new CObjNonCol(&mGoalTile01, CVector(170.0f + 590.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	new CObjNonCol(&mGoalTile01, CVector(170.0f + 190.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	//ゴール(ゲート側)
	for (int i = 0; i < 20; i++){
		if (i % 2 == 0){
			new CObjNonCol(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//黒タイル
			new CObjNonCol(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//白タイル
		}
		else{
			new CObjNonCol(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//黒タイル
			new CObjNonCol(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//白タイル
		}
	}	
	//ゲートの柱部分
	new CObjWall(&mPole, CVector(170.0f + 20.0f * -1 + 5.0f + 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0f, 10.0f));
	new CObjWall(&mPole, CVector(170.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0, 10.0f));
	//加速床
	new CObjBoost(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	new CObjBoost(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	//ジャンプ台
	new CObjJumper(&mJumper01, CVector(240.0f, -13.1f + 3.0f, 1110.0f), CVector(-30.0f, 0.0f, 0.0f), CVector(77.0f, 5.0f, 50.0f));

	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
	//敵車のカラー情報の出力
	PutCPUColor();
}
void CRaceCourceA::Update(){
	CSceneRace::Update();
}