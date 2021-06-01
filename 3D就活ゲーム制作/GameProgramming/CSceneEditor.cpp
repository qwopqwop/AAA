#include "CSceneEditor.h"
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
#include "CObjWall.h"
//
#include "CItem.h"
//
#include "CKey.h"
//
#include "CBullet.h"


#include "CRaceCourceD.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CObjGrass.h"
#include "CObjBoost.h"
#include "CObjNonCol.h"

////スマートポインタの生成
//std::shared_ptr<CTexture> TextureExp(new CTexture());
//std::shared_ptr<CTexture> TextureHit(new CTexture());
//std::shared_ptr<CTexture> TextureBoost(new CTexture());

extern CSound BGM;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;

////ここのmBestTimeの値は関係ない(mRecord_ の値を入れるため)
//int CSceneEditor::mBestTime = 0;
//int CSceneEditor::mRecord_A = 10000;
//int CSceneEditor::mRecord_B = 13000;
//int CSceneEditor::mRecord_C = 22000;
//int CSceneEditor::mRecord_D = 460000;
//int CSceneEditor::mRecord_E = 40000;
//int CSceneEditor::mRecord_F = 43300;

//画面サイズは800x600を想定
#define SCREENSIZE_X 800
#define SCREENSIZE_Y 600
//描画エリアの指定(左端のX座標,下端のY座標,幅,高さ)
#define MINIMAP_AREA 590,10,200,150
#define MINIMAP_AREA_EDITMODE 0,0,800,600
#define BACKMIRROR_FRAME_AREA 286,491,229,154
#define BACKMIRROR_BG_WHITE_AREA 288,493,225,150
#define BACKMIRROR_VIEW_AREA 288,493,225,150
#define BACKMIRROR_EXTRAFRAME_AREA 286,598,228,5

CSceneEditor::~CSceneEditor() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneEditor::Init() {
	//的の残数の初期化
	CItem::mTargetAmount = 0;

	////爆発テクスチャの読み込み
	//TextureExp->Load("exp.tga");
	////衝突テクスチャの読み込み
	//TextureHit->Load("effect\\[Attack]Hit01_panop.tga");
	////加速テクスチャの読み込み
	//TextureBoost->Load("effect\\boost01.tga");

	//テキストフォントの読み込みと設定
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	//mFont3:最後にmFont3に設定したシーンでのフォント(=Title画面のフォント)

	/*全コース共通のマテリアル*/
	mSky.Load("sky.obj", "sky.mtl");//背景
	mRock.Load("Rock1.obj", "Rock1.mtl");//岩
	//車の読み込み
	mRover.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");//プレイヤー
	mCarRed.Load("Rover1.obj", "material\\racing_mat\\single_color\\red.mtl");//以下敵の車
	mCarBlue.Load("Rover1.obj", "material\\racing_mat\\single_color\\blue.mtl");
	mCarGreen.Load("Rover1.obj", "material\\racing_mat\\single_color\\green.mtl");
	mCarYellow.Load("Rover1.obj", "material\\racing_mat\\single_color\\yellow.mtl");
	mCarPink.Load("Rover1.obj", "material\\racing_mat\\single_color\\pink.mtl");
	mCarCyan.Load("Rover1.obj", "material\\racing_mat\\single_color\\cyan.mtl");
	mCarWhite.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	mCarBlack.Load("Rover1.obj", "material\\racing_mat\\single_color\\black.mtl");
	mCarGray.Load("Rover1.obj", "material\\racing_mat\\single_color\\gray.mtl");
	mCheckPoint.Load("cube.obj", "cube2.mtl");//中間地点(透明、ポリゴン1枚)
	//床タイルの読み込み
	mTileBlack.Load("cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("cube.obj", "material\\racing_mat\\tile_white.mtl");
	mRWTile.Load("material\\racing_mat\\NewNewR-W.obj", "material\\racing_mat\\NewNewR-W.mtl");
	//立方体の読み込み
	mCube.Load("cube.obj", "material\\cube.mtl");//白
	mCube2.Load("cube.obj", "cube2.mtl");//透明
	mCube3.Load("cube.obj", "cube3.mtl");//?
	//地面の読み込み
	mPlane.Load("plane.obj", "plane.mtl");
	//階段の読み込み
	mStairs.Load("cube.obj", "material\\stairs.mtl");
	//的の読み込み
	mTarget.Load("Rock1.obj", "material\\target.mtl");
	//ONブロック(？)の読み込み
	mOnBlock.Load("cube.obj", "material\\on.mtl");
	mSign_Left.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnLeft.mtl");//標識:左折
	mSign_Right.Load("material\\racing_mat\\stage5\\Sign_TurnLeft.obj", "material\\racing_mat\\stage5\\Sign_TurnRight.mtl");//標識:右折
	//ミニマップ上でのプレイヤー・敵のカーソル、ゴール地点の読み込み
	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");//プレイヤー
	mCarsol_Enemy.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol_enemy.mtl");//敵
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");
	mPole.Load("cube.obj", "material\\soil.mtl");//ポール(木製)
	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");//加速床
	mJumper01.Load("cube.obj", "cube.mtl");//ジャンプ台
	/*コース個別に読み込むマテリアル*/
	if (CSceneTitle::mMode == 1){
		mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");//路面
		mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");//芝生
		mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");//柵(上面)
		mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");//柵(壁)
	}
	else if (CSceneTitle::mMode == 2){
		mCource02Road.Load("material\\racing_mat\\cource2nd\\cource02road.obj", "material\\racing_mat\\cource2nd\\cource02road.mtl");
		mCource02Wall.Load("material\\racing_mat\\cource2nd\\cource02wall.obj", "material\\racing_mat\\cource2nd\\cource02wall.mtl");
		mCource02Jump.Load("material\\racing_mat\\cource2nd\\cource02jumper.obj", "material\\racing_mat\\cource2nd\\cource02jumper.mtl");
	}
	else if (CSceneTitle::mMode == 3){
		mCource03Road.Load("material\\racing_mat\\stage3\\cource03road.obj", "material\\racing_mat\\stage3\\cource03road.mtl");
		mCource03Wall.Load("material\\racing_mat\\stage3\\cource03wall.obj", "material\\racing_mat\\stage3\\cource03wall.mtl");
		mCource03Fence.Load("material\\racing_mat\\stage3\\cource03fence.obj", "material\\racing_mat\\stage3\\cource03fence.mtl");
	}
	else if (CSceneTitle::mMode == 4){
		//コースエディターのタイルの読み込み
		mTile_Curve01_Floor.Load("material\\racing_mat\\stage_edit\\Curve01_floor.obj", "material\\racing_mat\\stage_edit\\Curve01_floor.mtl");
		mTile_Curve01_Wall.Load("material\\racing_mat\\stage_edit\\Curve01_wall.obj", "material\\racing_mat\\stage_edit\\Curve01_wall.mtl");
		mTile_Curve02_Floor.Load("material\\racing_mat\\stage_edit\\Curve02_floor.obj", "material\\racing_mat\\stage_edit\\Curve02_floor.mtl");
		mTile_Curve02_Wall.Load("material\\racing_mat\\stage_edit\\Curve02_wall.obj", "material\\racing_mat\\stage_edit\\Curve02_wall.mtl");
		mTile_Curve03_Floor.Load("material\\racing_mat\\stage_edit\\Curve03_floor.obj", "material\\racing_mat\\stage_edit\\Curve03_floor.mtl");
		mTile_Curve03_Wall.Load("material\\racing_mat\\stage_edit\\Curve03_wall.obj", "material\\racing_mat\\stage_edit\\Curve03_wall.mtl");
		mTile_Curve04_Floor.Load("material\\racing_mat\\stage_edit\\Curve04_floor.obj", "material\\racing_mat\\stage_edit\\Curve04_floor.mtl");
		mTile_Curve04_Wall.Load("material\\racing_mat\\stage_edit\\Curve04_wall.obj", "material\\racing_mat\\stage_edit\\Curve04_wall.mtl");
		mTile_Straight01_Floor.Load("material\\racing_mat\\stage_edit\\Straight01_floor.obj", "material\\racing_mat\\stage_edit\\Straight01_floor.mtl");
		mTile_Straight01_Wall.Load("material\\racing_mat\\stage_edit\\Straight01_wall.obj", "material\\racing_mat\\stage_edit\\Straight01_wall.mtl");
		mTile_Straight02_Floor.Load("material\\racing_mat\\stage_edit\\Straight02_floor.obj", "material\\racing_mat\\stage_edit\\Straight02_floor.mtl");
		mTile_Straight02_Wall.Load("material\\racing_mat\\stage_edit\\Straight02_wall.obj", "material\\racing_mat\\stage_edit\\Straight02_wall.mtl");
		mTile_Slope01_Floor.Load("material\\racing_mat\\stage_edit\\Slope01_floor.obj", "material\\racing_mat\\stage_edit\\Slope01_floor.mtl");
		mTile_Slope01_Wall.Load("material\\racing_mat\\stage_edit\\Slope01_wall.obj", "material\\racing_mat\\stage_edit\\Slope01_wall.mtl");
		mTile_Slope02_Floor.Load("material\\racing_mat\\stage_edit\\Slope02_floor.obj", "material\\racing_mat\\stage_edit\\Slope02_floor.mtl");
		mTile_Slope02_Wall.Load("material\\racing_mat\\stage_edit\\Slope02_wall.obj", "material\\racing_mat\\stage_edit\\Slope02_wall.mtl");
		mTile_Slope03_Floor.Load("material\\racing_mat\\stage_edit\\Slope03_floor.obj", "material\\racing_mat\\stage_edit\\Slope03_floor.mtl");
		mTile_Slope03_Wall.Load("material\\racing_mat\\stage_edit\\Slope03_wall.obj", "material\\racing_mat\\stage_edit\\Slope03_wall.mtl");
		mTile_Slope04_Floor.Load("material\\racing_mat\\stage_edit\\Slope04_floor.obj", "material\\racing_mat\\stage_edit\\Slope04_floor.mtl");
		mTile_Slope04_Wall.Load("material\\racing_mat\\stage_edit\\Slope04_wall.obj", "material\\racing_mat\\stage_edit\\Slope04_wall.mtl");
		mTile_Wide_Floor.Load("material\\racing_mat\\stage_edit\\Tile_WideF.obj", "material\\racing_mat\\stage_edit\\Tile_WideF.mtl");
		mTile_Wide_Wall.Load("material\\racing_mat\\stage_edit\\Tile_WideW.obj", "material\\racing_mat\\stage_edit\\Tile_WideW.mtl");
		mBlock_Floor.Load("material\\racing_mat\\stage_edit\\BlockF.obj", "material\\racing_mat\\stage_edit\\BlockF.mtl");
		mBlock_Wall.Load("material\\racing_mat\\stage_edit\\BlockW.obj", "material\\racing_mat\\stage_edit\\BlockW.mtl");
	}
	else if (CSceneTitle::mMode == 5){
		//コース05の読み込み
		mCource05Wall.Load("material\\racing_mat\\stage5\\cource05wall.obj", "material\\racing_mat\\stage5\\cource05wall.mtl");
		mCource05Mountain.Load("material\\racing_mat\\stage5\\cource05mountain.obj", "material\\racing_mat\\stage5\\cource05mountain.mtl");//全ての山共通
		mCource05Road.Load("material\\racing_mat\\stage5\\cource05road2.obj", "material\\racing_mat\\stage5\\cource05road2.mtl");
		mCource05Lake.Load("material\\racing_mat\\stage5\\cource05_lake.obj", "material\\racing_mat\\stage5\\cource05_lake.mtl");
		mCource05Grass_Floor.Load("material\\racing_mat\\stage5\\cource05grassF03.obj", "material\\racing_mat\\stage5\\cource05grassF03.mtl");
		mCource05Grass_Wall.Load("material\\racing_mat\\stage5\\cource05grass_wall.obj", "material\\racing_mat\\stage5\\cource05grass_wall.mtl");
		mCource05GoalTile.Load("material\\racing_mat\\stage5\\Checker_Tile.obj", "material\\racing_mat\\stage5\\Checker_Tile.mtl");
	}
	else if (CSceneTitle::mMode == 6){
		//
	}
	//コースエディターのタイルの読み込み
	mTile_Curve01_Floor.Load("material\\racing_mat\\stage_edit\\Curve01_floor.obj", "material\\racing_mat\\stage_edit\\Curve01_floor.mtl");
	mTile_Curve01_Wall.Load("material\\racing_mat\\stage_edit\\Curve01_wall.obj", "material\\racing_mat\\stage_edit\\Curve01_wall.mtl");
	mTile_Curve02_Floor.Load("material\\racing_mat\\stage_edit\\Curve02_floor.obj", "material\\racing_mat\\stage_edit\\Curve02_floor.mtl");
	mTile_Curve02_Wall.Load("material\\racing_mat\\stage_edit\\Curve02_wall.obj", "material\\racing_mat\\stage_edit\\Curve02_wall.mtl");
	mTile_Curve03_Floor.Load("material\\racing_mat\\stage_edit\\Curve03_floor.obj", "material\\racing_mat\\stage_edit\\Curve03_floor.mtl");
	mTile_Curve03_Wall.Load("material\\racing_mat\\stage_edit\\Curve03_wall.obj", "material\\racing_mat\\stage_edit\\Curve03_wall.mtl");
	mTile_Curve04_Floor.Load("material\\racing_mat\\stage_edit\\Curve04_floor.obj", "material\\racing_mat\\stage_edit\\Curve04_floor.mtl");
	mTile_Curve04_Wall.Load("material\\racing_mat\\stage_edit\\Curve04_wall.obj", "material\\racing_mat\\stage_edit\\Curve04_wall.mtl");
	mTile_Straight01_Floor.Load("material\\racing_mat\\stage_edit\\Straight01_floor.obj", "material\\racing_mat\\stage_edit\\Straight01_floor.mtl");
	mTile_Straight01_Wall.Load("material\\racing_mat\\stage_edit\\Straight01_wall.obj", "material\\racing_mat\\stage_edit\\Straight01_wall.mtl");
	mTile_Straight02_Floor.Load("material\\racing_mat\\stage_edit\\Straight02_floor.obj", "material\\racing_mat\\stage_edit\\Straight02_floor.mtl");
	mTile_Straight02_Wall.Load("material\\racing_mat\\stage_edit\\Straight02_wall.obj", "material\\racing_mat\\stage_edit\\Straight02_wall.mtl");
	mTile_Slope01_Floor.Load("material\\racing_mat\\stage_edit\\Slope01_floor.obj", "material\\racing_mat\\stage_edit\\Slope01_floor.mtl");
	mTile_Slope01_Wall.Load("material\\racing_mat\\stage_edit\\Slope01_wall.obj", "material\\racing_mat\\stage_edit\\Slope01_wall.mtl");
	mTile_Slope02_Floor.Load("material\\racing_mat\\stage_edit\\Slope02_floor.obj", "material\\racing_mat\\stage_edit\\Slope02_floor.mtl");
	mTile_Slope02_Wall.Load("material\\racing_mat\\stage_edit\\Slope02_wall.obj", "material\\racing_mat\\stage_edit\\Slope02_wall.mtl");
	mTile_Slope03_Floor.Load("material\\racing_mat\\stage_edit\\Slope03_floor.obj", "material\\racing_mat\\stage_edit\\Slope03_floor.mtl");
	mTile_Slope03_Wall.Load("material\\racing_mat\\stage_edit\\Slope03_wall.obj", "material\\racing_mat\\stage_edit\\Slope03_wall.mtl");
	mTile_Slope04_Floor.Load("material\\racing_mat\\stage_edit\\Slope04_floor.obj", "material\\racing_mat\\stage_edit\\Slope04_floor.mtl");
	mTile_Slope04_Wall.Load("material\\racing_mat\\stage_edit\\Slope04_wall.obj", "material\\racing_mat\\stage_edit\\Slope04_wall.mtl");
	mTile_Wide_Floor.Load("material\\racing_mat\\stage_edit\\Tile_WideF.obj", "material\\racing_mat\\stage_edit\\Tile_WideF.mtl");
	mTile_Wide_Wall.Load("material\\racing_mat\\stage_edit\\Tile_WideW.obj", "material\\racing_mat\\stage_edit\\Tile_WideW.mtl");
	mBlock_Floor.Load("material\\racing_mat\\stage_edit\\BlockF.obj", "material\\racing_mat\\stage_edit\\BlockF.mtl");
	mBlock_Wall.Load("material\\racing_mat\\stage_edit\\BlockW.obj", "material\\racing_mat\\stage_edit\\BlockW.mtl");

	BGM.Load("BGM\\revolumed_PopsGuitar_No.01.wav");
	
	//効果音の読み込み
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");

	//カメラ視点のY座標
	mCamY = 0.0f;
	//衝突判定の描画(デバッグ用)
	mPutCol = false;

	//一部テキストが点滅する時間
	mTextBlinkTime = 0;


	//カメラ視点
	mCamPoV = 1;


	//初期状態では敵の目標地点は描画しない
	isRendPoint = false;
	//初期状態ではポーズ状態無効
	isPause = false;

	//BGMはループ
	BGM.Repeat();


	//シーンの設定
	mScene = EEDIT;
	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//カメラの生成
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();


	int ess[3][4][5] = {
		{
			{ 00, 00, 00, 00, 21 },
			{ 00, 24, 00, 00, 00 },
			{ 00, 00, 00, 00, 00 },
			{ 00, 00, 00, 00, 00 },
		},
		{
			{ 00, 00, 00, 32, 00 },
			{ 00, 00, 34, 21, 33 },
			{ 00, 33, 00, 00, 00 },
			{ 00, 23, 22, 00, 00 },
		},
		{
			{ 24, 12, 32, 00, 00 },
			{ 31, 24, 21, 00, 00 },
			{ 33, 11, 31, 33, 33 },
			{ 23, 22, 00, 23, 22 },
		}
	};
	int size = 10;//コースの縮尺
	//for (int i = 3-1; i >= 0; i--){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 4; j++){
			for (int k = 0; k < 5; k++){
				if (ess[i][j][k] == 11){//直線:I字
					new CObjFloor(&mTile_Straight01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Straight01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 12){//直線:一字
					new CObjFloor(&mTile_Straight02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Straight02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 21){//カーブ:0時～3時
					new CObjFloor(&mTile_Curve03_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve03_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 22){//カーブ:3時～6時
					new CObjFloor(&mTile_Curve04_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve04_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 23){//カーブ:6時～9時
					new CObjFloor(&mTile_Curve01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 24){//カーブ:9時～12時
					new CObjFloor(&mTile_Curve02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Curve02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 31){//坂:
					new CObjFloor(&mTile_Slope01_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope01_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 32){//坂:
					new CObjFloor(&mTile_Slope02_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope02_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 33){//坂:
					new CObjFloor(&mTile_Slope03_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope03_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 34){//坂:
					new CObjFloor(&mTile_Slope04_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Slope04_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 91){//広い床
					new CObjFloor(&mTile_Wide_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mTile_Wide_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
				else if (ess[i][j][k] == 99){//でかいブロック
					new CObjFloor(&mBlock_Floor, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
					new CObjWall(&mBlock_Wall, CVector(-100.0f * size * k, 45.0f * size * (3 - 1 - i), -100.0f * size * j), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size));
				}
			}
		}
	}


	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
}


void CSceneEditor::Update() {
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向

	////視点を求める
	//if (mCamPoV == 1){
	//	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	//e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	//	//* mPlayer->mMatrixTranslate
	//	//	*CCameraPos::mpCamera->mMatrixTranslate
	//	//	+ CVector(0.0f, 0.0f, 0.0f);
	//	//c = CCameraPos::mpCamera->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	//e = CVector(0.0f, 0.0f, 0.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	//	*CCameraPos::mpCamera->mMatrixTranslate;
	//	//c = CCameraPos::mpCamera->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//	//	* CMatrix().RotateY(mPlayer->mRotation.mY);
	//	
	//}
	//else if (mCamPoV == 2){
	//	e = CVector(0.0f, 0.0f + 0.5f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	////注視点を求める
	//	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//		//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	//}
	//else if (mCamPoV == 3){//後方を映す視点
	//	e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	////注視点を求める
	//	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//		//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	//}
	//else{//1～3以外の数値が入っている時はとりあえず前方視点(1と同じ)
	//	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY)
	//		* mPlayer->mMatrixTranslate
	//		+ CVector(0.0f, 0.0f, 0.0f);
	//	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	//		* CMatrix().RotateY(mPlayer->mRotation.mY);
	//}
	////e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
	////	* CMatrix().RotateY(mPlayer->mRotation.mY)
	////	*mCam->mMatrixTranslate
	////	//* mPlayer->mMatrixTranslate
	////	+ CVector(0.0f, 0.0f, 0.0f);
	////c = mCam->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
	////	* CMatrix().RotateY(mPlayer->mRotation.mY);
	////上方向を求める
	//u = CVector(0.0f, 1.0f, 0.0f);// *mPlayer->mMatrixRotate;

	//e = CCameraPos::mpCamera->mPosition;
	e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale   // * mPlayer->mMatrixScale
				* CMatrix().RotateY(mPlayer->mRotation.mY)
				* mPlayer->mMatrixTranslate
				+ CVector(0.0f, 0.0f, 0.0f);

	c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale   //* mPlayer->mMatrixScale
		* CMatrix().RotateY(mPlayer->mRotation.mY);
	u = CVector(0.0f, 1.0f, 0.0f);//*mPlayer->mMatrixRotate;
	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	//タスクマネージャの更新・描画
	if (isPause == false){
		CTaskManager::Get()->Update();
	}
	CTaskManager::Get()->Render();
	//衝突処理
	CTaskManager::Get()->TaskCollision();
	//削除処理
	CTaskManager::Get()->Delete();

	//デバッグ用
#ifdef _DEBUG
	/*if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
		printf("ベストタイム:%d\n", mBestTime);
	}*/
	if (CKey::Once('2')){//Playerの座標を出力する
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	//if (CKey::Once('3')){//強制的に的の残数を0にする(本来の的は消えない)
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		printf("X:%f Y:%f Z:%f\n", mEnemys[i]->mPosition.mX, mEnemys[i]->mPosition.mY, mEnemys[i]->mPosition.mZ);
	//	}
	//	//CItem::mTargetAmount = 0;
	//}
	//if (CKey::Once('4')){//バックミラーのON・OFF切り替え
	//	isRender_BackMirror = !isRender_BackMirror;
	//}
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
	//if (CKey::Once('8')){
	//	//敵車すべてのmVPointの値を出力
	//	for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//		//printf("mEnemys[%d]->mVPoint…X:%.1f Y:%.1f Z:%.1f\n", i, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mX, mEnemys[i]->mVPoint.mZ);
	//		printf("mEnemys[%d]の中間地点…%d  %d週目\n", i, mEnemys[i]->mChecks, mEnemys[i]->mEnemyLap);
	//	}
	//}
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
	//GetNearestColor(1,2);

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
	//Bキーを押している間、メイン画面も後方視点になる
	if (CKey::Push('B')){
		mCamPoV = 3;
	}
	else{
		mCamPoV = 1;
	}
	//即時トップスピードに
	if (CKey::Once('G')){
		mPlayer->mCarSpeed = 20.0f;
	}
	//
	if (CKey::Once('V')){
		mPlayer->mChecks = 3;
	}
#endif

	if (CKey::Once('P')){
		isPause = !isPause;
	}

	////バックミラーの描画
	//if (isRender_BackMirror){
	//	RenderBackMirror();
	//}
	//ミニマップの描画
	RenderMiniMap();


	//2D描画開始
	Start2D(0, 800, 0, 600);

	//順位の描画
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//コース名を左下に表示
	CText::DrawString("COURCE EDIT", 20, 20, 10, 12);
		
	CPlayer::mpPlayer->CanMove = true;

	//ポーズ中表示される文字
	if (isPause){
		CText::DrawString("PAUSE", 280, 300, 10 * 3, 12 * 3, 3);
		CText::DrawString("P - Resume", 290, 200, 10, 12, 2);
		CText::DrawString("Esc - Back to Title", 250, 170, 10, 12, 2);
	}
	//2D描画終了
	End2D();
		
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
void CSceneEditor::RenderMiniMap() {
	glPushMatrix();
	glLoadIdentity();
	//一時的に2D視点に変更する
	glViewport(MINIMAP_AREA);//画面の描画エリアの指定
	glMatrixMode(GL_PROJECTION);	//行列をプロジェクションモードへ変更
	//行列退避
	glPushMatrix();
	glLoadIdentity();				//行列を初期化
	//2D画面の設定変更
	float size = 4000.0f;//コースの縮尺設定
	bool canscrollmap = false;//プレイヤーに合わせたマップ移動の有無
	//画面比率は800x600→4:3→1.33:1
	float ratio = 1.33f;
	if (canscrollmap){
		glOrtho(-size*ratio + mPlayer->mPosition.mX, size*ratio + mPlayer->mPosition.mX, -size - mPlayer->mPosition.mZ, size - mPlayer->mPosition.mZ, -size, size);//glOrtho(左、右、下、上、手前、奥)
	}
	else{
		glOrtho(-size*ratio, size*ratio, -size, size, -size, size);//glOrtho(左、右、下、上、手前、奥)
	}
	glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
	glLoadIdentity();				//行列を初期化
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	//X-Z平面をX-Y平面へ

	glDisable(GL_DEPTH_TEST);
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
			else if (i == 7){
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
	//CMatrix matenemys[ENEMYS_AMOUNT];
	//for (int i = 0; i < ENEMYS_AMOUNT; i++){
	//	matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
	//		* CMatrix().RotateX(0)
	//		* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
	//		* CMatrix().RotateZ(0)
	//		* mEnemys[i]->mMatrixTranslate;
	//	mCarsol_Enemy.Render(matenemys[i]);
	//}
	//ミニマップ状にプレイヤーを示すカーソルを描画
	CMatrix matplayer;
	matplayer = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mPlayer->mMatrixTranslate;
	mCarsol.Render(matplayer);




	glPopMatrix();
	glViewport(0, 0, 800, 600); //画面の描画エリアの指定
	glEnable(GL_DEPTH_TEST);

	//3D視点に戻す
	glViewport(0, 0, 800, 600);	//画面の描画エリアの指定
	glMatrixMode(GL_PROJECTION);	//行列をプロジェクションモードへ変更
	glLoadIdentity();				//行列を初期化
	//gluOrtho2D(-200 / 2, 200 / 2, -150 / 2, 150 / 2);	//2Dの画面を設定
	gluPerspective(75.0, 800.0 / 600.0, 1.0, 10000.0);	//3Dの画面を設定
	glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
	glLoadIdentity();
}
////バックミラーを表示
//void CSceneEditor::RenderBackMirror(){
//	glDisable(GL_CULL_FACE);//一時的に両面を描画可能にする
//	glDisable(GL_DEPTH_TEST);
//	glViewport(BACKMIRROR_FRAME_AREA);
//	//2D描画開始
//	Start2D(0, 800, 0, 600);
//	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//	glColor4fv(color);
//	//上記の2D描画範囲の指定値より大きめに白背景を描画する
//	int expand = 100; color[3] = 0.0f;
//	//白背景のよりも先に黒枠となるものを描画する
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(800 + expand, 600 + expand);
//	glVertex2d(0 - expand, 600 + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(800 + expand, 0 - expand);
//	glVertex2d(800 + expand, 600 + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D描画終了
//	End2D();
//
//	glViewport(BACKMIRROR_BG_WHITE_AREA);
//	//2D描画開始
//	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
//	color[0] = color[1] = color[2] = 0.8f; color[3] = 0.0f;
//	glColor4fv(color);
//	//上記の2D描画範囲の指定値より大きめに白背景を描画する
//	expand = 100;
//	//白背景を"先に"描画する
//	glBegin(GL_TRIANGLES);//久しぶり
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glVertex2d(0 - expand, 600 + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D描画終了
//	End2D();
//
//	//行列を退避させる
//	glPushMatrix();
//	//行列を単位行列にする
//	glLoadIdentity();
//	glViewport(BACKMIRROR_VIEW_AREA);
//	//カメラのパラメータを作成する
//	CVector e, c, u;//視点、注視点、上方向
//	e = CVector(0.0f, 17.0f + 13.0f, 40.0f - 41.0f) * CMatrix().RotateY(mCamY)* mPlayer->mMatrixScale
//		* CMatrix().RotateY(mPlayer->mRotation.mY)
//		* mPlayer->mMatrixTranslate;
//	c = mPlayer->mPosition + CVector(0.0f, 17.0f + 12.8f, 40.0f - 42.0f)* mPlayer->mMatrixScale
//		* CMatrix().RotateY(mPlayer->mRotation.mY);
//	u = CVector(0.0f, 1.0f, 0.0f);
//	//カメラののX座標を反転させる	
//	e = e * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	c = c * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	u = u * CMatrix().Scale(-1.0f, 1.0f, 1.0f);
//	//バックミラーのカメラの設定
//	gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
//	GLfloat translate[] = {
//		-1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1
//	};
//	glMultMatrixf(translate);
//	CTaskManager::Get()->Render();
//
//	//黒枠の見切れの補完
//	glViewport(BACKMIRROR_EXTRAFRAME_AREA);
//	//2D描画開始
//	Start2D(0, SCREENSIZE_X, 0, SCREENSIZE_Y);
//	color[0] = color[1] = color[2] = 0.0f;
//	glColor4fv(color);
//	//上記の2D描画範囲の指定値より大きめに白背景を描画する
//	expand = 100;
//	//風景の上に黒枠を描画する
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glVertex2d(0 - expand, SCREENSIZE_Y + expand);
//	glEnd();
//	glBegin(GL_TRIANGLES);
//	glVertex2d(0 - expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, 0 - expand);
//	glVertex2d(SCREENSIZE_X + expand, SCREENSIZE_Y + expand);
//	glEnd();
//	color[0] = color[1] = color[2] = color[3] = 1.0f;
//	glColor4fv(color);
//	//2D描画終了
//	End2D();
//
//	glEnable(GL_DEPTH_TEST);
//	glViewport(0, 0, SCREENSIZE_X, SCREENSIZE_Y); //画面の描画エリアをメインの画面に戻す
//	glEnable(GL_CULL_FACE);//表面のみの描画に戻す
//
//	//行列を戻す
//	glPopMatrix();
//}
//次のシーンの取得
CScene::EScene CSceneEditor::GetNextScene(){
	return mScene;
}