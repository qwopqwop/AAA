#include "CSceneRace.h"
//
#include "CSceneTitle.h"
//
#include "CCamera.h"
//
#include "CText.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
////
#include "CRock.h"
//
#include "CObj.h"
//
#include "CItem.h"
//
#include "CKey.h"
//
#include "CBullet.h"

#define ENEMY_AMOUNT 5

//スマートポインタの生成
std::shared_ptr<CTexture> TextureExp(new CTexture());
std::shared_ptr<CTexture> TextureHit(new CTexture());
std::shared_ptr<CTexture> TextureBoost(new CTexture());

extern CSound BGM;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;

//ベストタイムの初期記録は2分00秒00
int CSceneRace::mBestTime = 20000;
int CSceneRace::mRecord_A = 10000;
int CSceneRace::mRecord_B = 13000;
int CSceneRace::mRecord_C = 20000;
int CSceneRace::mRecord_D = 30000;

CSceneRace::~CSceneRace() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneRace::Init() {
	//シーンの設定
	mScene = ERACE1;
	
	if (CSceneTitle::mDifficulty == 1){//難易度：EASY
		CEnemy::mPointSize = 6;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f);
		//CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1700.0f), 40.0f);
		//CEnemy::mPoint2 = new CPoint(CVector(-958.0f, 30.0f, 2600.0f), 40.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f);
	}
	else if (CSceneTitle::mDifficulty == 2){//難易度：NORMAL
		/*
		//CEnemy::mPointSize = 12;//ポイント数の設定
		//CEnemy::mPoint = new CPoint(CVector(260.0f + 60.0f, 30.0f, 777.0f + 100.0f), 200.0f);
		//CEnemy::mPoint2 = new CPoint(CVector(259.0f + 60.0f, 30.0f, 1300.0f + 200.0f), 200.0f);
		//CEnemy::mPoint3 = new CPoint(CVector(-150.0f + 0.0f, 30.0f, 2058.0f + 200.0f), 200.0f);
		//CEnemy::mPoint4 = new CPoint(CVector(-555.0f - 150.0f, 30.0f, 2111.0f + 300.0f), 200.0f);
		//CEnemy::mPoint5 = new CPoint(CVector(-1039.0f - 150.0f, 30.0f, 2062.0f + 200.0f), 200.0f);
		//CEnemy::mPoint6 = new CPoint(CVector(-1400.0f - 150.0f, 30.0f, 1700.0f + 200.0f), 200.0f);
		//CEnemy::mPoint7 = new CPoint(CVector(-1511.0f, 30.0f, -317.0f - 200.0f), 120.0f);
		//CEnemy::mPoint8 = new CPoint(CVector(-1400.0f, 30.0f, -1079.0f - 300.0f), 200.0f);
		//CEnemy::mPoint9 = new CPoint(CVector(-1000.0f, 30.0f, -1500.0f - 300.0f), 200.0f);
		//CEnemy::mPoint10 = new CPoint(CVector(-550.0f, 30.0f, -1700.0f - 300.0f), 200.0f);
		//CEnemy::mPoint11 = new CPoint(CVector(-100.0f, 30.0f, -1600.0f - 325.0f), 300.0f);
		//CEnemy::mPoint12 = new CPoint(CVector(420.0f, 30.0f, -1182.0f - 300.0f), 350.0f);
		*/
		/*
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(   260.0f + 00.0f, 30.0f, 777.0f), 40.0f * 4);
		CEnemy::mPoint2 = new CPoint(CVector(  259.0f + 00.0f, 30.0f, 1300.0f), 50.0f * 4);
		CEnemy::mPoint3 = new CPoint(CVector( -150.0f + 50.0f, 30.0f + 50.0f, 2058.0f), 65.0f * 4);
		CEnemy::mPoint4 = new CPoint(CVector(-555.0f + 50.0f, 30.0f + 50.0f, 2111.0f), 65.0f * 4);
		CEnemy::mPoint5 = new CPoint(CVector(-1039.0f + 50.0f, 30.0f + 50.0f, 2062.0f), 65.0f * 4);
		CEnemy::mPoint6 = new CPoint(CVector(-1400.0f + 50.0f, 30.0f + 50.0f, 1700.0f), 60.0f * 4);
		CEnemy::mPoint7 = new CPoint(CVector(-1511.0f + 50.0f, 30.0f, -317.0f), 40.0f * 4);
		CEnemy::mPoint8 = new CPoint(CVector(-1400.0f + 50.0f, 30.0f - 50.0f, -1079.0f), 40.0f * 4);
		CEnemy::mPoint9 = new CPoint(CVector(-913.0f + 50.0f, 30.0f - 50.0f, -1637.0f), 40.0f * 4);
		CEnemy::mPoint10 = new CPoint(CVector(-500.0f + 50.0f, 30.0f - 50.0f, -1700.0f), 60.0f * 4);
		CEnemy::mPoint11 = new CPoint(CVector(-160.0f + 50.0f, 30.0f - 50.0f, -1600.0f), 60.0f * 4);
		CEnemy::mPoint12 = new CPoint(CVector(193.0f + 50.0f, 30.0f - 50.0f, -1182.0f), 50.0f * 4);
		*/		
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(   260.0f + 00.0f, 30.0f, 777.0f), 50.0f * 2);
		CEnemy::mPoint2 = new CPoint(CVector(  259.0f + 00.0f, 30.0f, 1300.0f), 60.0f * 2);
		CEnemy::mPoint3 = new CPoint(CVector( -150.0f + 50.0f, 30.0f + 50.0f, 2058.0f), 65.0f * 2);
		CEnemy::mPoint4 = new CPoint(CVector(-555.0f + 50.0f, 30.0f + 50.0f, 2121.0f), 65.0f * 2);
		CEnemy::mPoint5 = new CPoint(CVector(-1039.0f + 50.0f, 30.0f + 50.0f, 2082.0f), 65.0f * 2);
		CEnemy::mPoint6 = new CPoint(CVector(-1400.0f + 50.0f, 30.0f + 50.0f, 1700.0f), 80.0f * 2);
		CEnemy::mPoint7 = new CPoint(CVector(-1511.0f + 50.0f, 30.0f, -317.0f), 40.0f * 2);
		CEnemy::mPoint8 = new CPoint(CVector(-1400.0f + 50.0f, 30.0f - 50.0f, -1079.0f), 60.0f * 2);
		CEnemy::mPoint9 = new CPoint(CVector(-913.0f + 50.0f, 30.0f - 50.0f, -1637.0f), 60.0f * 2);
		CEnemy::mPoint10 = new CPoint(CVector(-500.0f + 50.0f, 30.0f - 50.0f, -1700.0f), 60.0f * 2);
		CEnemy::mPoint11 = new CPoint(CVector(-160.0f + 50.0f, 30.0f - 50.0f, -1600.0f), 60.0f * 2);
		CEnemy::mPoint12 = new CPoint(CVector(193.0f + 50.0f, 30.0f - 50.0f, -1182.0f), 60.0f * 2);
	}
	else if (CSceneTitle::mDifficulty == 3){//難易度：HARD
		CEnemy::mPointSize = 12;//ポイント数の設定
		CEnemy::mPoint = new CPoint(CVector(260.0f, 30.0f, 777.0f), 40.0f * 2);
		CEnemy::mPoint2 = new CPoint(CVector(259.0f, 30.0f, 1300.0f), 50.0f * 2);
		CEnemy::mPoint3 = new CPoint(CVector(-150.0f, 30.0f, 2058.0f), 65.0f * 2);
		CEnemy::mPoint4 = new CPoint(CVector(-555.0f, 30.0f, 2111.0f), 65.0f * 2);
		CEnemy::mPoint5 = new CPoint(CVector(-1039.0f, 30.0f, 2062.0f), 65.0f * 2);
		CEnemy::mPoint6 = new CPoint(CVector(-1400.0f, 30.0f, 1700.0f), 60.0f * 2);
		CEnemy::mPoint7 = new CPoint(CVector(-1511.0f, 30.0f, -317.0f), 40.0f * 2);
		CEnemy::mPoint8 = new CPoint(CVector(-1400.0f, 30.0f, -1079.0f), 40.0f * 2);
		CEnemy::mPoint9 = new CPoint(CVector(-913.0f, 30.0f, -1637.0f), 40.0f * 2);
		CEnemy::mPoint10 = new CPoint(CVector(-500.0f, 30.0f, -1700.0f), 60.0f * 2);
		CEnemy::mPoint11 = new CPoint(CVector(-160.0f, 30.0f, -1600.0f), 60.0f * 2);
		CEnemy::mPoint12 = new CPoint(CVector(193.0f, 30.0f, -1182.0f), 50.0f * 2);
	}
	else{
		//隠しHARDの予定
	}

	////ポイントの設定
	//CEnemy::mPointSize = 5;//ポイント数の設定
	//CEnemy::mPoint = new CPoint[CEnemy::mPointSize];
	//CEnemy::mPoint[0].Set(CVector(135.0f, 30.0f, 200.0f), 40.0f);
	//CEnemy::mPoint[1].Set(CVector(735.0f, 30.0f, 200.0f), 40.0f);
	//CEnemy::mPoint[2].Set(CVector(435.0f, 30.0f, 500.0f), 40.0f);
	//CEnemy::mPoint[3].Set(CVector(435.0f, 30.0f, -100.0f), 40.0f);
	//CEnemy::mPoint[4].Set(CVector(435.0f, 30.0f, 200.0f), 40.0f);//[0]から[3]の中心
	/*for (int i = 0; i < CEnemy::mPointSize; i++){
		CEnemy::mPoint.Set
	}*/

	//的の残数の初期化
	CItem::mTargetAmount = 0;

	//爆発テクスチャの読み込み
	TextureExp->Load("exp.tga");
	//衝突テクスチャの読み込み
	TextureHit->Load("effect\\[Attack]Hit01_panop.tga");
	//加速テクスチャの読み込み
	TextureBoost->Load("effect\\boost01.tga");
	////TextureBoost->Load("effect\\[Magic]SonicWave01_panop.tga");

	//テキストフォントの読み込みと設定
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	//mFont3:最後にmFont3に設定したシーンでのフォント(≒Title画面の時)

	//背景の読み込み
	mSky.Load("sky.obj", "sky.mtl");
	//岩の読み込み
	mRock.Load("Rock1.obj", "Rock1.mtl");
	//車の読み込み
	mRover.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	//mRover.Load("Rover1.obj", "Rover1.mtl");
	mCarRed.Load("Rover1.obj", "material\\racing_mat\\single_color\\red.mtl");
	mCarBlue.Load("Rover1.obj", "material\\racing_mat\\single_color\\blue.mtl");
	mCarGreen.Load("Rover1.obj", "material\\racing_mat\\single_color\\green.mtl");
	mCarYellow.Load("Rover1.obj", "material\\racing_mat\\single_color\\yellow.mtl");
	mCarPink.Load("Rover1.obj", "material\\racing_mat\\single_color\\pink.mtl");
	mCarCyan.Load("Rover1.obj", "material\\racing_mat\\single_color\\cyan.mtl");
	mCarWhite.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	mCarBlack.Load("Rover1.obj", "material\\racing_mat\\single_color\\black.mtl");
//	mRover.Load("sphere.obj", "sphere.mtl");
	//立方体の読み込み
	mCube.Load("cube.obj", "material\\cube.mtl");
	//地面の読み込み
	mPlane.Load("plane.obj", "plane.mtl");
	//階段の読み込み
	mStairs.Load("cube.obj", "material\\stairs.mtl");
	//的の読み込み
	mTarget.Load("Rock1.obj", "material\\target.mtl");
	//ONブロック(？)の読み込み
	mOnBlock.Load("cube.obj", "material\\on.mtl");

	mItem.Load("Rock1.obj", "Rock1.mtl");
	

	mCube2.Load("cube.obj", "cube2.mtl");
	mCube3.Load("cube.obj", "cube3.mtl");
	//中間地点の読み込み(透明、ポリゴン1枚のみ)
	mCheckPoint.Load("plane.obj", "cube2.mtl");

	mWater.Load("cube.obj", "water_sumple.mtl");


	//バネの読み込み
	mSpringS.Load("ばね.obj", "cube.mtl");
	mSpringL.Load("ばね伸び.obj", "cube.mtl");

	//床タイルの読み込み
	mTileBlack.Load("cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("cube.obj", "material\\racing_mat\\tile_white.mtl");
	mRWTile.Load("material\\racing_mat\\NewNewR-W.obj", "material\\racing_mat\\NewNewR-W.mtl");
	//コースの読み込み
	//mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	mCource02.Load("material\\racing_mat\\cource2nd\\track01.obj", "material\\racing_mat\\cource2nd\\track01.mtl");
	//芝生の読み込み
	mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");
	//柵(壁)の読み込み
	mFence01.Load("material\\racing_mat\\Cource01Wall.obj", "material\\racing_mat\\Cource01Wall.mtl");
	//柵の読み込み
	mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");
	mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");

	mPole.Load("cube.obj", "material\\soil.mtl");

	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");

	//ミニマップ上でのプレイヤー・敵のカーソルの読み込み
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//プレイヤー
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//敵
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");

	CBullet::mBullet.Load("銃弾.obj", "銃弾.mtl");

	//ステージ1BGMの読み込み
	if (CSceneTitle::mMode == 1){
		BGM.Load("BGM\\(音量調整版)Popsギター_No.01.wav");
		mMaxLap = 1;
		mBestTime = mRecord_A;
	}
	else if (CSceneTitle::mMode == 2){
		BGM.Load("BGM\\調整後game_maoudamashii_1_battle34.wav");
		mMaxLap = 2;
		mBestTime = mRecord_B;
	}
	else if (CSceneTitle::mMode == 3){
		BGM.Load("BGM\\調整後Crazy_Machine.wav");
		mMaxLap = 3;
		mBestTime = mRecord_C;
	}
	else if (CSceneTitle::mMode == 4){
		BGM.Load("BGM\\(調整後)bgm_maoudamashii_neorock33.wav");
		mMaxLap = 5;
		mBestTime = mRecord_D;
	}

	//効果音の読み込み
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");

	mSumple.Load("UnderGround\\passage2.obj", "UnderGround\\passage2.mtl");
	msumple2.Load("material\\racing_mat\\stage2\\tekitou.obj", "material\\racing_mat\\stage2\\tekitou.mtl");


	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mRover;

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
		/*if (i < 7){
			mEnemys[i]->mPosition = CVector(350.0f - 25.0f*i, -13.538f, -140.0f);
		}		
		else{
			int io = i % 7;
			mEnemys[i]->mPosition = CVector(250.0f + 25.0f*io, -13.538f, -140.0f - (20.0f*i/7));
		}
		if (i == 5){
			mEnemys[i]->mPosition = CVector(250.0f, -13.538f, -100.0f);
		}
		else if (i == 6){
			mEnemys[i]->mPosition = CVector(350.0f, -13.538f, -100.0f);
		}
		else if (i == 7){
			mEnemys[i]->mPosition = CVector(300.0f, -13.538f, -170.0f);
		}
		else if (i % 2 == 1){
			mEnemys[i]->mPosition.mZ -= 7.0f;
		}		*/


		mEnemys[i]->CCharacter::Update();
	}

	//岩の生成　モデルmRock　位置|-20.0 0.0 20.0|
	//回転|0.0 0.0 0.0|　拡大|5.0 5.0 5.0|
	//	new CRock(&mRock, CVector(-20.0f, 0.0f, 20.0f), CVector(), CVector(5.0f, 5.0f, 5.0f));
	//立方体の生成
	//	new CObj(&mCube, CVector(0.0f, 0.0f, 60.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 1.0f), 1);
	//地面の生成
//	new CObj(&mPlane, CVector(0.0f, 0.0f, 0.0f), CVector(), CVector(100.0f, 1.0f, 100.0f), 1);
	////旧コースの生成
	//new CObj(&mCube, CVector(0.0f, -20.0f, 0.0f + 100.0f * 29), CVector(0.0f, 0.0f, 0.0f), CVector(100.0f, 10.0f, 100.0f * 29 + 100.0f), 1);
	////ゴール地点の床タイル
	//for (int i = 0; i < 10; i++){
	//	if (i % 2 == 0){
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//黒タイル
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//白タイル
	//	}
	//	else{
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//黒タイル
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, -2.0f+0.1f, 5700.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//白タイル
	//	}
	//}
	////ゴール(ゲート側)
	//for (int i = 0; i < 10; i++){
	//	if (i % 2 == 0){
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, 110.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//黒タイル
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, 110.0f + 10.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//白タイル
	//	}
	//	else{
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, 110.0f + 10.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//黒タイル
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, 110.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//白タイル
	//	}
	//}

	// 1 2 3 4 5
	//■□■□■
	//□■□■□
	//チェック柄を作りやすくするメソッド？
	//CChecker(パターン1,パターン2,どこまで模様を繰り返すか,,,正方形のサイズ,厚み)
	//CChecker(&mTileBlack ,&mTileWhite, 5, 10.0f, 0.5f)


	//透明度の高い物から先に描画する
	
	//中間地点(順に通らないと1周したことにならないし、順番を飛ばしてもいけない)
	new CObj(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(777.0f, 1.0f, 255.0f), 101);
	new CObj(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 1.0f, 255.0f), 102);
	new CObj(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 1.0f, 255.0f), 103);
	//new CObj(&mCheckPoint, CVector(600.0f, 15.0f, 6.9f), CVector(-90.0f, -180.0f, 0.0f), CVector(750.0f, 1.0f, 255.0f), 109);//ゴール地点
	//new CObj(&mCheckPoint, CVector(0.0f, 20.0f, 2100.0f), CVector(90.0f, 0.0f, 0.0f), CVector(255.0f, 1.0f, 255.0f), 25);
	/*new CObj(&mPlane, CVector(0.0f, 0.0f, -220.0f), CVector(0.0f, 0.0f, 0.0f), CVector(55.0f, 1.0f, 55.0f), 1);
	new CObj(&mCheckPoint, CVector(0.0f, 15.0f, -400.0f), CVector(0.0f, 0.0f, 0.0f), CVector(55.0f, 1.0f, 55.0f), 1);*/

	////ジャンプ台
	//new CObj(&mOnBlock, CVector(0.0f, 0.0f, 450.0f), CVector(-40.0f, 0.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);
	//new CObj(&mOnBlock, CVector(0.0f, 0.0f, 550.0f), CVector(-40.0f, 90.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);
	//new CObj(&mOnBlock, CVector(0.0f, 0.0f, 650.0f), CVector(-40.0f, 180.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);
	//new CObj(&mOnBlock, CVector(0.0f, 0.0f, 750.0f), CVector(-40.0f, 270.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);

	////ばね
	//new CItem(&mSpringL, CVector(0.0f, -5.0f, 80.0f), CVector(), CVector(11.0f, 11.0f, 11.0f), 2);//バネ

	
	if (CSceneTitle::mMode <= 0){
		//new CObj(&msumple2, CVector(320.0f, -58.0f, 450.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.0f, 3.0f, 3.0f), 1);
		new CObj(&mCource02, CVector(-2800.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(7000.0f, 7000.0f, 7000.0f), 1);
	}
	else{
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
		}
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
	new CObj(&mDashBoard, CVector(260.0f, -13.1f + 3.0f, 800.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);
	new CObj(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);
	new CObj(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -200.0f), CVector(0.0f, 0.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);
	new CObj(&mDashBoard, CVector(-500.0f, -13.1f + 3.0f, -1900.0f), CVector(0.0f, -90.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f), 111);

	new CObj(&mDashBoard, CVector(500.0f, 13.1f + 3.0f, -1200.0f), CVector(0.0f, 180.0f, 90.0f), CVector(3.0f, 3.0f, 3.0f), 111);

	////物理演算(笑)するオブジェクト No.2001
	//new CObj(&mTileWhite, CVector(500.0f, -13.1f + 23.0f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//new CObj(&mTileWhite, CVector(550.0f, -13.1f + 0.0f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//for (int i = 0; i < 5; i++){
	//	new CObj(&mTileWhite, CVector(280.0f + 22.0f*i, -13.1f + 22.0f*i, 200.0f), CVector(0.0f, 0.0f, 0.0f), CVector(11.0f, 11.0f, 11.0f), 2001);
	//}	
	//new CRigidObj(&mTileWhite, CVector(400.0f, -13.1f, 400.0f), CVector(0.0f, 0.0f, 0.0f), CVector(21.0f, 21.0f, 21.0f));


	//new CObj(&mTileWhite, CVector(0.0f, -11.1f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 150.0f, 10.0f), 200);//壁
	//new CObj(&mTileWhite, CVector(230.0f, -11.1f, 1300.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 50.0f, 50.0f), 200);//壁
	//new CObj(&mTileBlack, CVector(0.0f, -1010.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50000.0f, 1.0f, 50000.0f), 99);//黒

	////当たり判定有りで生成してもさほど動作は重くならなくなった
	//new CObj(&mSumple, CVector(430.0f, 10.0f + 1123.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 50.0f, 50.0f), 999);
	//流れるダッシュボード達
	/*for (int i = 0; i < 16; i++){
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i , 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 31);
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i-20.0f, 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(0.9f, 0.9f, 0.9f), 31);
	}*/

	////空を描画しない
	//new CObj(&mSky, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(0.0f, 0.0f, 0.0f), CVector(22.0f, 22.0f, 22.0f), 101);

	//new CObj(&mCource01, CVector(1000.0f, -13.1f + 0.5f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 10000.0f, 10000.0f), 9999);//黒タイル

	////ステージ2のマテリアル
	//if (CSceneTitle::mMode == 2){
	//	new CObj(&msumple2, CVector(320.0f, -58.0f, 450.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.0f, 3.0f, 3.0f), 1);
	//}

	//カメラ視点のY座標
	mCamY = 0.0f;
	//衝突判定の描画(デバッグ用)
	mPutCol = false;

	//一部テキストの点滅時間
	mTextBlinkTime = 0;

	//レースのカウントダウン関連
	mFrame = 0;
	mCountDown = 3+1;
	//レースはカウントダウンが終わってから開始
	isStartRace = isGoal = false;
	//時間のリセット
	mTime = 0;
		
	//ラップ数の初期化
	mLap = 1;
	//記録更新してない状態
	isNewRecord = false;
	
	//※最終順位の初期化
	mRanking = 1;
	//プレイヤーがゴール後の経過時間
	mAfterGoalTime = 0;

	//カメラ視点
	mCamPoV = 1;

	//バックミラーの描画
	isRender_BackMirror = true;

	//初期状態では敵の目標地点は描画しない
	isRendPoint = false;
	//初期状態ではポーズ状態無効
	isPause = false;

	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}

	

	BGM.Repeat();
}


void CSceneRace::Update() {
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
//	e = CVector(-2.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrix   * mPlayer->mMatrixScale * mPlayer->mMatrixRotate * mPlayer->mMatrixTranslate;
	if (mCamPoV == 1){
		e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////注視点を求める
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else if (mCamPoV == 2){
		e = CVector(0.0f, 0.0f + 0.5f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////注視点を求める
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else if (mCamPoV == 3){//後方を映す視点
		e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////注視点を求める
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}

	//CMatrix().RotateZ(mRotation.mZ) *CMatrix().RotateX(mRotation.mX) *CMatrix().RotateY(mRotation.mY);

	//////注視点を求める
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);// *mPlayer->mMatrixRotate;
	
	if (CKey::Once('J')){
		printf("カメラ視点(Main) %f %f %f   %f %f %f     %f %f %f\n", e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	}

	//e = CVector(0.0f+mXXX, 5000.0f, mZZZ+0.0f);
	//	/*CVector(0.0f, 3000.0f, 0.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);*/
	////注視点を求める
	//c = CVector(0.0f+mXXX, 0.0f, mZZZ+0.0001f);
	//	//mPlayer->mPosition + CVector(0.0f, 0.0f, 0.01f)*mPlayer->mMatrixRotate;
	////上方向を求める
	//u = CVector(0.0f, 1.0f, 0.0f) * mPlayer->mMatrixRotate;

	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	////タスクマネージャの更新
	//TaskManager.Update();
	////タスクマネージャの描画
	//TaskManager.Render();

	/*mPlayer->mpModel = &mRover;
	mPlayer->mScale = CVector(2.5f, 2.5f, 2.5f);*/

	//タスクマネージャの更新・描画
	if (isPause == false){
		CTaskManager::Get()->Update();
	}

	CTaskManager::Get()->Render();
	
	////岩の描画
	//mRock.Render(CMatrix().Scale(5.0f, 5.0f, 5.0f));
	////車の描画
	//mRover.Render(CMatrix().Translate(-20.0f, 0.0f, 10.0f));

	////あ゛
	////コリジョンマネージャの衝突処理
	//CollisionManager.Collision();
	//TaskManager.TaskCollision();
	CTaskManager::Get()->TaskCollision();

	//TaskManager.Delete();
	CTaskManager::Get()->Delete();

	//デバッグ用
#ifdef _DEBUG
	if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
		printf("ベストタイム:%d\n", mBestTime);
	}
	if (CKey::Once('2')){//Playerの座標を出力する
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Once('3')){//強制的に的の残数を0にする(本来の的は消えない)
		CItem::mTargetAmount = 0;
	}
	if (CKey::Once('4')){//余剰の計算時、割る数(右の数字)は正の値で計算される？
		//printf("%d\n", -13 % 5);//負%正=負
		//printf("%d\n", -12 % -5);//負%負=負
		//printf("%d\n", 14 % -5);//正%負=正
		isRender_BackMirror = !isRender_BackMirror;
	}
	if (CKey::Push('5')){
		printf("%f:%f:%f\n", CPlayer::mpPlayer->mRotation.mX, CPlayer::mpPlayer->mRotation.mY, CPlayer::mpPlayer->mRotation.mZ);
	}
	if (CKey::Once('6')){
		printf("%d\n", CPlayer::mpPlayer->mChecks);
	}
	if (CKey::Once('7')){
		if (CPlayer::mpPlayer->mFlyingMode){
			CPlayer::mpPlayer->mFlyingMode = false;
		}
		else{
			CPlayer::mpPlayer->mFlyingMode = true;
		}
	}
	if (CKey::Once('8')){
		//敵車すべてのmVPointの値を出力
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			//printf("mEnemys[%d]->mVPoint…X:%.1f Y:%.1f Z:%.1f\n", i, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mZ);
			printf("mEnemys[%d]の中間地点…%d  %d週目\n", i, mEnemys[i]->mChecks, mEnemys[i]->mEnemyLap);
		}
	}
	if (CKey::Once('9')){
		if (mPutCol){
			mPutCol = false;
		}
		else{
			mPutCol = true;
		}
	}
	if (mPutCol){
		//衝突判定の描画
		CollisionManager.Render();
	}

	//敵の中継地点の表示ON・OFF切り替え
	if (CKey::Once('O')){		
		/*if (isRendPoint){
			isRendPoint = false;
		}
		else{
			isRendPoint = true;
		}*/
		//もっと簡潔に切り替える方法
		isRendPoint = !isRendPoint;
	}
	//BGMを停止する
	if (CKey::Once('M')){
		BGM.Stop();
	}
	////カメラ視点の切り替え
	//if (CKey::Once('0')){
	//	//mCamPoV = 1;
	//	if (mCamPoV == 1){
	//		mCamPoV = 2;
	//	}
	//	else if (mCamPoV == 2){
	//		mCamPoV = 3;
	//	}
	//	else if (mCamPoV == 3){
	//		mCamPoV = 1;
	//	}
	//}
	
#endif


	//Bキーを押している間は後方視点になる
	if (CKey::Push('B')){
		mCamPoV = 3;
	}
	else{
		mCamPoV = 1;
	}

	//ポーズ画面に入っていない時
	if (isPause == false){
		if (isStartRace){
			//59:59:59が最大時間
			if (mTime < 595959){
				if (mTime % 10000 == 5959){
					mTime += 1 + 40 + 4000;
				}
				else if (mTime % 100 == 59){
					mTime += 1 + 40;
				}
				else{
					mTime += 1;
				}
			}
		}
	}
	
	//ミニマップの描画
	RenderMiniMap();
	//バックミラーの描画
	if (isRender_BackMirror){
		RenderBackMirror();

		//e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
		//	* CMatrix().RotateY(mPlayer->mRotation.mY)
		//	* mPlayer->mMatrixTranslate
		//	+ CVector(0.0f, 0.0f, 0.0f);
		//
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
		//	* CMatrix().RotateY(mPlayer->mRotation.mY);
		//u = CVector(0.0f, 1.0f, 0.0f);

		///*////後方視点を表示？
		////e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
		////	* CMatrix().RotateY(mPlayer->mRotation.mY)
		////	* mPlayer->mMatrixTranslate
		////	+ CVector(0.0f, 0.0f, 0.0f);
		////c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
		////	* CMatrix().RotateY(mPlayer->mRotation.mY);
		//////上方向を求める
		////u = CVector(0.0f, 1.0f, 0.0f);
		//*/
		//
		////バックミラーのカメラの設定
		//Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
		//Camera.mEye = e;
		//CTaskManager::Get()->Render();
	}

	

	//2D描画開始
	Start2D(0, 800, 0, 600);

	//順位の描画
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	CText::DrawString("COURCE 1", 20, 20, 10, 12);

	//時間の表示
	char mti[70];// :も含めた最大文字数の設定
//	sprintf(mti, "%06d", mTime);
	sprintf(mti, "TIME:%02d:%02d:%02d", mTime / 10000 % 100, mTime / 100 % 100, mTime % 100);
	CText::DrawString(mti, 20, 530, 10, 12);
	//ベストタイムの表示
	char mbestti[70];// :も含めた最大文字数の設定
	//	sprintf(mti, "%06d", mTime);
	sprintf(mbestti, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	CText::DrawString(mbestti, 20, 580, 10, 12);
	
	//カウントダウン開始、GO!で操作の受付開始
	if (mFrame < 60){
		mFrame++;
	}
	else{
		if (mCountDown >= 0){
			mCountDown--;
			if (mCountDown == 0){
				isStartRace = true;
				SoundStart.Play();
			}
			else if (mCountDown > 0){
				SoundCountDown.Play();
			}
		}
		mFrame = 0;
	}
	//カウントダウン表示
	char mcountd[7];
	//残り3秒までの間、表示
	sprintf(mcountd, "%d", mCountDown);
	if (mCountDown > 0 && mCountDown <= 3){
		CText::DrawString(mcountd, 400, 300, 25, 30);
	}
	else if (mCountDown == 0){
		CText::DrawString("GO!", 400-40, 300, 25, 30);
		CPlayer::mpPlayer->CanMove = true;
		for (int i = 0; i < ENEMYS_AMOUNT; i++){
			mEnemys[i]->CanMove = true;
		}
	}

	//if (isStartRace == false){
	//	if (mCountDown > 0){
	//		//カウントダウン中(ゴール以前に、スタートすらしてない)
	//	}
	//	else{
	//		CText::DrawString("FINISH!", 400 - 25 * 6, 300, 25, 30);
	//	}
	//}

	char lap[19];
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	CText::DrawString(lap, 20, 500, 10, 12, 2);

	//ゴール後、継続して実行する処理
	if (mLap == mMaxLap && isStartRace == false && isGoal){
		//60fで一巡
		if (mTextBlinkTime < 60){
			mTextBlinkTime++;
		}
		else{
			mTextBlinkTime = 0;
		}
		//Enterキー入力でタイトル画面に戻れることを伝えるテキスト
		if (mTextBlinkTime < 30){
			CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
		}
		//新記録をたたき出した時
		if (isNewRecord){
			//CText::DrawString("FINISH!", 400 - 20 * 6, 300, 20, 24);
			if (mTextBlinkTime < 15 || mTextBlinkTime >= 30 && mTextBlinkTime < 45){
				CText::DrawString("NEW RECORD!", 55, 551, 8, 9, 2);
			}
		}
	}
	char carspeed[33];
	sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	CText::DrawString(carspeed, 20+560, 20, 10, 12);

	
	//ゴール後に表示される文字
	if (isGoal){
		if (mAfterGoalTime < 999){
			mAfterGoalTime++;
		}

		if (mAfterGoalTime > 85){
			color[3] = 1.0f - 0.1f * (mAfterGoalTime - 85);
			glColor4fv(color);
		}
		CText::DrawString("FINISH!", 400 - 25 * 6, 300, 25, 30);

		color[3] = 0.05f * (mAfterGoalTime - 100);
		glColor4fv(color);
		char rank[8];
		if (mRanking == 1){      //1st
			sprintf(rank, "%dst", mRanking);
		}
		else if (mRanking == 2){ //2nd
			sprintf(rank, "%dnd", mRanking);
		}
		else if (mRanking == 3){ //3rd
			sprintf(rank, "%drd", mRanking);
		}
		else{ //4th,5th,...
			sprintf(rank, "%dth", mRanking);
		}
		CText::DrawString(rank, 357, 310, 10 * 2, 12 * 2, 2);		
	}
	color[3] = 1.0f;
	glColor4fv(color);
	

	//ポーズ時に表示される文字
	if (isPause){
		CText::DrawString("PAUSE", 280, 300, 10*3, 12*3, 3);		
		CText::DrawString("P - Resume", 290, 200, 10, 12, 2);
		CText::DrawString("Esc - Back to Title", 250, 170, 10, 12, 2);
	}
	
	//2D描画終了
	End2D();

	//if (CKey::Push('Y')){//でば
	//	mCamY += 1.0f;
	//}
	//if (CKey::Push('U')){//つぐ
	//	mCamY += -1.0f;
	//}


	//ゴール地点通過時の処理
	if ((CPlayer::mpPlayer->mPosition.mX > -55.0f && CPlayer::mpPlayer->mPosition.mX < 1400.0f)
		&& (CPlayer::mpPlayer->mPosition.mZ > -3.1f - 5.0f && CPlayer::mpPlayer->mPosition.mZ < -3.1f + 5.0f + 20.0f)
		&& (CPlayer::mpPlayer->mChecks == 3)
		&& (isStartRace)){
		//new CObj(&mCube, CVector(0.0f, 0.0f, 5700.0f), CVector(-90.0f, 0.0f, 0.0f), CVector(100.0f, 13.0f, 211.0f), 1);//ゴール
		//new CObj(&mTileBlack, CVector(170.0f + 20.0f * -1 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//柱
		//new CObj(&mTileBlack, CVector(170.0f + 20.0f * 40 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//柱

		if (mLap == mMaxLap){
			//ベストタイム更新時
			if (mTime < mBestTime){
				mBestTime = mTime;
				isNewRecord = true;
				//コースによって新しく記録する
				if (CSceneTitle::mMode == 1){
					mRecord_A = mBestTime;
				}
				else if (CSceneTitle::mMode == 2){				
					mRecord_B = mBestTime;
				}
				else if (CSceneTitle::mMode == 3){					
					mRecord_C = mBestTime;
				}
				else if (CSceneTitle::mMode == 4){					
					mRecord_D = mBestTime;
				}
			}
			isStartRace = false;
			isGoal = true;
			BGM.Stop();
			SoundGoal.Play();
			//CPlayer::mpPlayer->CanMove = false;//動きストップ
			CPlayer::mpPlayer->mChecks = 0;
		}
		else{
			mLap++;
			CPlayer::mpPlayer->mChecks = 0;
		}
	}
	//CPUの車がゴール地点を通過した時の処理
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		if (mEnemys[i]->mPosition.mX>-55.0f && mEnemys[i]->mPosition.mX < 1400.0f
			&& (mEnemys[i]->mPosition.mZ > -3.1f - 5.0f && mEnemys[i]->mPosition.mZ < -3.1f + 5.0f + 20.0f)
			&& (mEnemys[i]->mChecks == 3)
			&& (mEnemys[i]->isEnemyGoaled == false)){
			//その敵が最終ラップだった場合
			if (mEnemys[i]->mEnemyLap == mMaxLap){
				//プレイヤーの未ゴール時はプレイヤーの最終順位が落ちる
				if (isGoal == false){
					mRanking++;
				}
				mEnemys[i]->isEnemyGoaled = true;
			}
			//まだ最終ラップでない場合
			else{
				//チェックポイントをリセットし、次の周スタート
				mEnemys[i]->mChecks = 0;
				mEnemys[i]->mEnemyLap++;
			}
		}
	}
	//if ((CPlayer::mpPlayer->mPosition.mX > -55.0f && CPlayer::mpPlayer->mPosition.mX < 1400.0f)
	//	&& (CPlayer::mpPlayer->mPosition.mZ > -3.1f - 5.0f && CPlayer::mpPlayer->mPosition.mZ < -3.1f + 5.0f + 20.0f)
	//	&& (CPlayer::mpPlayer->mChecks == 3)
	//	&& (isStartRace)){	
	//	if (mLap == mMaxLap){
	//		//ベストタイム更新時
	//		if (mTime < mBestTime){
	//			mBestTime = mTime;
	//			isNewRecord = true;
	//			//コースによって新しく記録する
	//			if (CSceneTitle::mMode == 1){
	//				mRecord_A = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 2){
	//				mRecord_B = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 3){
	//				mRecord_C = mBestTime;
	//			}
	//			else if (CSceneTitle::mMode == 4){
	//				mRecord_D = mBestTime;
	//			}
	//		}
	//		isStartRace = false;
	//		isGoal = true;
	//		BGM.Stop();
	//		SoundGoal.Play();
	//		//CPlayer::mpPlayer->CanMove = false;//動きストップ
	//		CPlayer::mpPlayer->mChecks = 0;
	//	}
	//	else{
	//		mLap++;
	//		CPlayer::mpPlayer->mChecks = 0;
	//	}
	//}
	

	if (CKey::Once('P')){
		//カウントダウン終了後、ポーズの切り替えが可能になる。
		if (mCountDown < 0){
			//ゴール後は切り替え不可
			if (isGoal)return;

			//ポーズのON・OFF切り替え
			isPause = !isPause;
		}		
	}

	/*シーン切り替え系の処理*/
	//ゴール後Enterキー押下→タイトル画面移行
	if (isGoal){
		if (CKey::Once(VK_RETURN)){
			//次のシーンはゲーム
			mScene = ETITLE;
		}
	}
	//ポーズ中Escキー押下→タイトル画面移行
	if (isPause){
		if (CKey::Once(VK_BACK) || CKey::Once(VK_ESCAPE)){
			//次のシーンはゲーム
			mScene = ETITLE;
		}
	}


	return;
}

/* マップ上からの視点 */
void CSceneRace::RenderMiniMap() {
	glPushMatrix();
	glViewport(600 + 20-30, 450 - 440, 200, 150); //画面の描画エリアの指定
	glLoadIdentity();
	gluLookAt(0, 4800, 0, 0, 0, 0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
//	BackGround.Render(CMatrix());
	//タスクマネージャの描画
//	TaskManager.Render();

//	mPlayer->mpModel = &mCarsol;
//	mPlayer->mScale = CVector(10.0f, 1.0f, 10.0f);
	CTaskManager::Get()->Render();


	if (isRendPoint == true){
		/*デバッグ用*/
		//設定した敵の目標地点すべてをミニマップ上に描画する
		CMatrix point;
		for (int i = 1; i <= 12; i++){//ポイントの数だけ処理実行
			point = CMatrix().Scale(111.0f, 1.0f, 111.0f)
				* CMatrix().RotateY(45);
			//* CEnemy::mPoint->mMatrixTranslate;
			//1より小さい時は即やめ
			if (i < 1){
				break;
			}
			if (i == 1){
				point = point * CEnemy::mPoint->mMatrixTranslate;
			}
			else if (i == 2){
				point = point * CEnemy::mPoint2->mMatrixTranslate;
			}
			else if (i == 3){
				point = point * CEnemy::mPoint3->mMatrixTranslate;
			}
			else if (i == 4){
				point = point * CEnemy::mPoint4->mMatrixTranslate;
			}
			else if (i == 5){
				point = point * CEnemy::mPoint5->mMatrixTranslate;
			}
			else if (i == 6){
				point = point * CEnemy::mPoint6->mMatrixTranslate;
			}
			//ハードモードではさらに目標地点が細かく設定される
			else if (CSceneTitle::mDifficulty == 2 || CSceneTitle::mDifficulty == 3){
				if (i == 7){
					point = point * CEnemy::mPoint7->mMatrixTranslate;
				}
				else if (i == 8){
					point = point * CEnemy::mPoint8->mMatrixTranslate;
				}
				else if (i == 9){
					point = point * CEnemy::mPoint9->mMatrixTranslate;
				}
				else if (i == 10){
					point = point * CEnemy::mPoint10->mMatrixTranslate;
				}
				else if (i == 11){
					point = point * CEnemy::mPoint11->mMatrixTranslate;
				}
				else if (i == 12){
					point = point * CEnemy::mPoint12->mMatrixTranslate;
				}
			}
			else{
				break;
			}
			mTileWhite.Render(point);
		}
	}
	//ミニマップにゴールアイコンを描画
	CMatrix matminig;
	matminig = CMatrix().Scale(20.0f, 1.0f, 20.0f)
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(0)
		* CMatrix().RotateZ(0)
		* CMatrix().Translate(550.0f, 0.0f, -10.0f);
	mMiniGoal.Render(matminig);
	
	CMatrix matenemys[ENEMYS_AMOUNT];
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mEnemys[i]->mMatrixTranslate;
		mCarsol_Enemy.Render(matenemys[i]);
	}

	//ミニマップ状にプレイヤーを示すカーソルを描画
	CMatrix matplayer;
	matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mPlayer->mMatrixTranslate;
	mCarsol.Render(matplayer);


	//mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;//参考
	


	glPopMatrix();
	glViewport(0, 0, 800, 600); //画面の描画エリアの指定
	glEnable(GL_DEPTH_TEST);
	
	//Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);

	
}
//バックミラーを表示
void CSceneRace::RenderBackMirror(){
	glDisable(GL_CULL_FACE);//一時的に両面を描画可能にする
	glDisable(GL_DEPTH_TEST);
	glViewport(800 -400 -150-3, 400 - 7-3, 306, 206); //バックミラーの描画エリアの指定
	////カメラのパラメータを作成する
	//CVector be, bc, bu;//視点、注視点、上方向
	////視点を求める
	//be = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);
	//////注視点を求める
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//bc = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//bu = CVector(0.0f, 1.0f, 0.0f);
	//be = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);
	//////注視点を求める
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//bc = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	////* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	////カメラの設定
	//Camera3D(be.mX, be.mY, be.mZ, bc.mX, bc.mY, bc.mZ, bu.mX, bu.mY, bu.mZ);
	//Camera.mEye = be;
	
	//2D描画開始
	Start2D(0, 800, 0, 600);
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	int expand = 100;
	//白背景のよりも先に黒枠となるものを描画する
	glBegin(GL_TRIANGLES);//久しぶり
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glVertex2d(0 - expand, 600 + expand);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glEnd();
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//2D描画終了
	End2D();

	glViewport(800 - 400 - 150, 400 - 7, 300, 200);
	//2D描画開始
	Start2D(0, 800, 0, 600);
	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//上記の2D描画範囲の指定値より大きめに白背景を描画する
	expand = 100;
	//白背景を"先に"描画する
	glBegin(GL_TRIANGLES);//久しぶり
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glVertex2d(0 - expand, 600 + expand);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0 - expand, 0 - expand);
	glVertex2d(800 + expand, 0 - expand);
	glVertex2d(800 + expand, 600 + expand);
	glEnd();

	color[0] = color[1] = color[2] = color[3] = 1.0f;
	glColor4fv(color);
	//2D描画終了
	End2D();



	//行列を退避させる
	glPushMatrix();
	//行列を単位行列にする
	glLoadIdentity();

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//メインカメラと同じ前方視点なはず…
	e = CVector(0.0f, 17.0f + 13.0f, 40.0f - 41.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* mPlayer->mMatrixTranslate;
		//+ CVector(0.0f, 0.0f, 0.0f);
	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 12.8f, 40.0f - 42.0f)* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);	

	//バックミラーのカメラの設定
	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);

	if (CKey::Once('L')){
		printf("カメラ視点(Back) %f %f %f   %f %f %f     %f %f %f\n", e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	}

	
	GLfloat translate[] = {
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixf(translate);

	/*e.mX *= -1;
	c.mX *= -1;
	mPlayer->mMatrix.Identity.mM*/

	//mPlayer->mPosition.mX *= -1;
	//e.mX *= -1;
	//c.mX *= -1;
	//glMultMatrixf(CMatrix().RotateY(180));
	//glMultMatrixf(&e.mX);//ｷﾞｬｱｱｱｱｱｱｱｱ

	CTaskManager::Get()->Render();


	////2D描画開始
	//Start2D(0, 800, 0, 600);
	////バックミラー画面のフレーム(枠)
	///*????*/
	////2D描画終了
	//End2D();

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600); //画面の描画エリアをメインの画面に戻す
	glEnable(GL_CULL_FACE);//表面のみの描画に戻す

	//行列を戻す
	glPopMatrix();
}

//次のシーンの取得
CScene::EScene CSceneRace::GetNextScene(){
	return mScene;
}

