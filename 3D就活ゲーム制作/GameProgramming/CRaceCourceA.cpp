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

	//このコース用のマテリアル読込み
	mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");//路面
	mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");//芝生
	mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//柵(上面)
	mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//柵(壁)
	mCurb01.Load("material\\racing_mat\\Curb01.obj", "material\\racing_mat\\Curb01.mtl");//紅白タイル
	mGoalTile01.Load("material\\racing_mat\\cource01_goaltile.obj", "material\\racing_mat\\cource01_goaltile.mtl");//＝白黒タイル

	CSceneRace::Init();
		
	for (int i = 0; i < LIST_SIZE; i++) {
		StartPosVecs[i] = StartPosVec + CVector(-0.1f*list[i], 0.0f, -80.0f*list[i]);
		if (list[i] % 2 == 1){
			StartPosVecs[i].mX -= 80.0f;
		}
		printf("%d, ",list[i]);
	}
	printf("\n");

	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;

	//プレイヤーのリスポーンするCheckPointの設定
	mPlayer->SetRespawnPoint(0, StartPosVecs[0], CVector(0.0f, 0.0f, 0.0f));
	//mPlayer->SetRespawnPoint(0, CVector(350.0f, -13.538f, -130.0f), CVector(0.0f, 0.0f, 0.0f));
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
	
	CSceneRace::InstantiateEnemy();	

	/*透明度の高い物から先に描画する*/
	//中間地点(1周と判定されるには順番通りに通過する必要がある)
	new CObjCheckPoint(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(2000.0f, 31.0f, 255.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 31.0f, 255.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 31.0f, 255.0f), 3);
	//ゴール地点
	new CObjCheckPoint(&mCheckPoint, CVector(600.0f, -30.0f, 1.9f), CVector(), CVector(800.0f, 100.0f, 10.0f), 9);

	//コースの生成
	//芝生の生成(通行時速度低下)
	new CObjGrass(&mGrass01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
	//コースに柵の配置(壁扱い)
	new CObjFloor(&mFenceTop, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
	new CObjWall(&mFenceSide, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
	//道路と芝生の境目のタイルを生成(当たり判定無し)
	new CObjNonCol(&mCurb01, CVector(-360.0f, 5.0f - 33.0f + 0.05f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
	
	//白・黒タイルでゴール示唆
	new CObjNonCol(&mGoalTile01, CVector(170.0f + 590.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	new CObjNonCol(&mGoalTile01, CVector(170.0f + 190.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	//ゴール(ゲート側)
	new CObjNonCol(&mGoalGate, CVector(170.0f - 15.0f + 410.0f, 110.0f + 200.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), false);//白タイル
	
	//ゲートの柱部分
	new CObjWall(&mPole, CVector(170.0f + 20.0f * -1 + 5.0f + 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0f, 10.0f));
	new CObjWall(&mPole, CVector(170.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0, 10.0f));
	//加速床
	new CObjBoost(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	new CObjBoost(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	//ジャンプ台
	new CObjJumper(&mJumper01, CVector(240.0f, -13.1f + 3.0f, 1110.0f), CVector(-30.0f, 0.0f, 0.0f), CVector(77.0f, 5.0f, 50.0f));

	//ゲートの柱部分
	new CObjWall(&mPole, CVector(270.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f + 110.0f, -660.0f), CVector(0.0f, 0.0f, 0.0f), CVector(77.0f, 77.0f, 77.0f));	
}
void CRaceCourceA::Update(){
	CSceneRace::Update();
}