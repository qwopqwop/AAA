#include "CRaceCourceE.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CRoadManager.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CObjGrass.h"
#include "CObjWater.h"
#include "CObjJumper.h"
#include "CSceneTitle.h"

void CRaceCourceE::Init(){
	//シーンの設定
	mScene = ERACE5;

	//コース05の読み込み
	mCource05Wall.Load("material\\racing_mat\\stage5\\cource05wall.obj", "material\\racing_mat\\stage5\\cource05wall.mtl");
	mCource05Mountain.Load("material\\racing_mat\\stage5\\cource05mountain.obj", "material\\racing_mat\\stage5\\cource05mountain.mtl");//全ての山共通
	mCource05Road.Load("material\\racing_mat\\stage5\\cource05road2.obj", "material\\racing_mat\\stage5\\cource05road2.mtl");
	mCource05Lake.Load("material\\racing_mat\\stage5\\cource05_lake.obj", "material\\racing_mat\\stage5\\cource05_lake.mtl");
	mCource05Grass_Floor.Load("material\\racing_mat\\stage5\\cource05grassF03.obj", "material\\racing_mat\\stage5\\cource05grassF03.mtl");
	mCource05Grass_Wall.Load("material\\racing_mat\\stage5\\cource05grass_wall.obj", "material\\racing_mat\\stage5\\cource05grass_wall.mtl");
	mCource05GoalTile.Load("material\\racing_mat\\stage5\\Checker_Tile.obj", "material\\racing_mat\\stage5\\Checker_Tile.mtl");

	CSceneRace::Init();
	mBestTime = mRecords[5];
	mMinimap_size = 5500.0f;
	isScrollMinimap = true;
	
	//BGMの読み込み
	BGM.Load("BGM\\revolumed_Go_on_the_mountain_road.wav");

	StartPosVec = CVector(-3831.5f, 13.5f, 16011.5f);//スタート位置の基点
	//CVector StartPosVecs[ENEMYS_AMOUNT + 1];//スタート位置(配列)
	for (int i = 0; i < LIST_SIZE; i++) {
		StartPosVecs[i] = StartPosVec + CVector(45.0f*list[i], 0.0f, 60.0f*list[i]);
		if (list[i] % 2 == 1){
			StartPosVecs[i].mX += 80.0f;
		}
	}


	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//プレイヤーのリスポーンするCheckPointの設定
	mPlayer->SetRespawnPoint(0, StartPosVecs[0], CVector(0.0f, -145.0f, 0.0f));
	mPlayer->SetRespawnPoint(1, CVector(-16054.4f, 4915.0f, -2180.0f), CVector(0.0f, -174.6f, 0.0f));
	mPlayer->SetRespawnPoint(2, CVector(4680.0f, 13.5f, -2027.0f), CVector(0.0f, 147.2f, 0.0f));
	mPlayer->SetRespawnPoint(3, CVector(14809.0f, 13.5f, 4270.0f), CVector(0.0f, -9.5f, 0.0f));
	mPlayer->GetReady();
	//カメラの生成
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();
	//コース全体のサイズ感を設定
	float mtsize = 35.0f;
	float height = 11.0f;
	new CRoadManager(&mCource05Road, CVector(0.0f, 21.0f, 0.0f), CVector(), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, -1.0f), 320.0f, 0.0f);//
	
	/*CSceneRace::InstantiatePlayer(CVector(0, -145, 0));
	CSceneRace::SetRespawnPoint(1);*/

	//for (int i = 0; i < LIST_SIZE; i++) {
	//	StartPosVecs[i] = StartPosVec + CVector(-0.1f*list[i], 0.0f, -80.0f*list[i]);
	//	if (list[i] % 2 == 1){
	//		StartPosVecs[i].mX -= 80.0f;
	//	}
	//	printf("%d, ", list[i]);
	//}
	//printf("\n");
	////プレイヤーの生成
	//mPlayer = new CPlayer();
	//mPlayer->mpModel = &mCarWhite;
	////プレイヤーのリスポーンするCheckPointの設定
	//mPlayer->SetRespawnPoint(0, StartPosVecs[0], CVector(0.0f, 0.0f, 0.0f));
	////mPlayer->SetRespawnPoint(0, CVector(350.0f, -13.538f, -130.0f), CVector(0.0f, 0.0f, 0.0f));
	//mPlayer->SetRespawnPoint(1, CVector(-80.0f, -13.538f, 2175.0f), CVector(0.0f, -55.0f, 0.0f));
	//mPlayer->SetRespawnPoint(2, CVector(-1620.0f, -13.538f, 450.0f), CVector(0.0f, -175.0f, 0.0f));
	//mPlayer->SetRespawnPoint(3, CVector(-1212.0f, -13.538f, -1616.0f), CVector(0.0f, 120.0f, 0.0f));
	//mPlayer->GetReady();
	////カメラの生成
	//mCamRange = new CCameraRange();
	//mCam = new CCameraPos();
	////コースの生成//床と壁で分割して処理を区別する
	////コース全体のサイズ感を設定
	//float mtsize = 50.0f;
	//float height = 2.0f;
	//new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, 1.0f), 500.0f, 0.0f);



	////GPモードのみ敵が生成される
	//if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
	//	//敵車の生成
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		mEnemys[i] = new CEnemy();
	//		if (i % 8 == 0){
	//			mEnemys[i]->mpModel = &mCarBlue;
	//		}
	//		else if (i % 8 == 1){
	//			mEnemys[i]->mpModel = &mCarPink;
	//		}
	//		else if (i % 8 == 2){
	//			mEnemys[i]->mpModel = &mCarRed;
	//		}
	//		else if (i % 8 == 3){
	//			mEnemys[i]->mpModel = &mCarGreen;
	//		}
	//		else if (i % 8 == 4){
	//			mEnemys[i]->mpModel = &mCarYellow;
	//		}
	//		else if (i % 8 == 5){
	//			mEnemys[i]->mpModel = &mCarBlack;
	//		}
	//		else if (i % 8 == 6){
	//			mEnemys[i]->mpModel = &mCarGray;
	//		}
	//		else if (i % 8 == 7){
	//			mEnemys[i]->mpModel = &mCarCyan;
	//		}
	//		//初期の配置座標を設定する
	//		
	//		mEnemys[i]->mPosition = CVector(StartPosVecs[i + 1]);
	//		mEnemys[i]->mRotation = CVector(0, -145, 0);
	//		mEnemys[i]->CCharacter::Update();
	//	}
	//}
	////優先度変更
	//CTaskManager::Get()->ChangePriority(mPlayer, 15);
	//if (CSceneTitle::mMode == CSceneTitle::EMODE_GRANDPRIX){
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	//	}
	//}
	////敵車のカラー情報の出力
	//PutCPUColor();

	CSceneRace::InstantiateEnemy(CVector(0, -145, 0));

	//中間地点(順に通らないと1周したことにならないし、順番を飛ばしてもいけない)
	new CObjCheckPoint(&mCheckPoint, CVector(-16054.4f, 4915.0f, -2180.0f), CVector(0.0f, 0.0f, 0.0f), CVector(180.0f, 100.0f, 180.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(4680.0f, 13.5f, -2027.0f), CVector(0.0f, 0.0f, 0.0f), CVector(200.0f, 100.0f, 200.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(14809.0f, 13.5f, 4270.0f), CVector(0.0f, 0.0f, 0.0f), CVector(200.0f, 100.0f, 200.0f), 3);
	//ゴール地点
	new CObjCheckPoint(&mCheckPoint, CVector(-3862.5f, 21.3f, 15925.5f), CVector(0.0f, 0.0f - 145.3f, 0.0f), CVector(240.0f, 100.0f, 30.0f), 9);
	new CObjNonCol(&mCource05GoalTile, CVector(-3862.5f, 21.3f, 15925.5f), CVector(0.0f, -145.3f, 0.0f), CVector(3.9f, 3.9f, 3.9f));
	//山道以外のオブジェクトの生成
	new CObjWall(&mCource05Wall, CVector(0.0f, 21.0f, 0.0f), CVector(), CVector(mtsize, height, mtsize));//
	new CObjGrass(&mCource05Mountain, CVector(0.0f, 21.0f, 0.0f), CVector(), CVector(mtsize, height, mtsize));//
	new CObjGrass(&mCource05Grass_Floor, CVector(0.0f, 20.0f, 0.0f), CVector(), CVector(mtsize, 30.0f, mtsize));
	new CObjWall(&mCource05Grass_Wall, CVector(0.0f, 20.0f, 0.0f), CVector(), CVector(mtsize, 30.0f, mtsize));
	new CObjWater(&mCource05Lake, CVector(0.0f, 221.0f, 0.0f), CVector(), CVector(mtsize, 30.0f, mtsize));

	new CObjNonCol(&mSign_Left, CVector(2440.0f, 321.0f, 1432.0f), CVector(0.0f, 33.0f, 0.0f), CVector(4.0f, 4.0f, 4.0f));//標識：左折
	new CObjNonCol(&mSign_Right, CVector(13277.0f, 12.0f, -6939.0f), CVector(0.0f, 82.3f, 0.0f), CVector(4.0f, 4.0f, 4.0f));//標識：右	
}
void CRaceCourceE::Update(){
	CSceneRace::Update();
}