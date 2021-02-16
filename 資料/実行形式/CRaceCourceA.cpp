#include "CRaceCourceA.h"
#include "CObj.h"
#include "CSceneTitle.h"

void CRaceCourceA::Init(){
	//シーンの設定
	mScene = ERACE1;

	CSceneRace::Init();
	
	//敵のポイント(目的地)の設定
	if (CSceneTitle::mDifficulty == 1){//難易度：EASY
		//現在、仮の設定
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
	}
	else if (CSceneTitle::mDifficulty == 2){//難易度：NORMAL
		//現在、仮の設定
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
	}
	else if (CSceneTitle::mDifficulty == 3){//難易度：HARD
		//現在、仮の設定
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f);
	}
	else{
		//隠しHARDの予定
	}
	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
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
			mEnemys[i]->mpModel = &mCarWhite;
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
	}
	/*透明度の高い物から先に描画する*/
	//中間地点(順に通らないと1周したことにならないし、順番を飛ばしてもいけない)
	new CObj(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(2000.0f, 31.0f, 255.0f), 101);
	new CObj(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 31.0f, 255.0f), 102);
	new CObj(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 31.0f, 255.0f), 103);
	//新・コースの生成
	for (int i = 0; i < 1; i++){
		//コースの生成//ここを床と壁で分割して処理を分ける予定
		new CObj(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 1);
		//芝生の生成(通行中は速度低下)
		new CObj(&mGrass01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 112);
		//コースに柵の配置(壁扱い)
		new CObj(&mFenceTop, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f), 1);
		new CObj(&mFenceSide, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f), 200);
		//道路と芝生の境目のタイルを生成(当たり判定無し)
		new CObj(&mRWTile, CVector(-360.0f, 5.0f - 33.0f + 0.05f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 99);
		//柵の内側を詰めてみたがやっぱり車が入り込んでしまう
		//new CObj(&mStuff, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f), 1);
	}
	//白・黒タイルでゴール示唆
	for (int i = 0; i < 40; i++){
		//	mStartPoint[0] = 300.0f;  mStartPoint[1] = 63.0f;  mStartPoint[2] = -50.0f;
		if (i % 2 == 0){
			new CObj(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//黒タイル
			new CObj(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//白タイル
		}
		else{
			new CObj(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//黒タイル
			new CObj(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//白タイル
			//new CObj(&mTileWhite, CVector(300.0f + 20.0f*i, -2.0f + 0.1f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//白タイル
		}
	}
	//ゴール(ゲート側)
	for (int i = 0; i < 20; i++){
		if (i % 2 == 0){
			new CObj(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//黒タイル
			new CObj(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//白タイル
		}
		else{
			new CObj(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//黒タイル
			new CObj(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//白タイル
		}
	}
	//ポール
	new CObj(&mTileWhite, CVector(170.0f + 20.0f * -1 + 5.0f + 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0f, 10.0f), 1);//柱
	new CObj(&mTileWhite, CVector(170.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0, 10.0f), 1);//柱
	//加速床
	//new CObj(&mDashBoard, CVector(260.0f, -13.1f + 3.0f, 800.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);//保留
	new CObj(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);
	new CObj(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);
	//ジャンプ台
	new CObj(&mOnBlock, CVector(240.0f, -13.1f + 3.0f, 1110.0f), CVector(-30.0f, 0.0f, 0.0f), CVector(77.0f, 5.0f, 50.0f), 202);


	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
}
void CRaceCourceA::Update(){
	CSceneRace::Update();
}

//void CRaceCource2::Init(){
//
//}
//void CRaceCource2::Update(){
//
//}