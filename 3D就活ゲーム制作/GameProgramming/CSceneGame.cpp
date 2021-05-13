#include "CSceneGame.h"
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

extern CSound BGM;

CSceneGame::~CSceneGame() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneGame::Init() {
	//シーンの設定
	mScene = ESTAGE1;
	
	//的の残数の初期化
	CItem::mTargetAmount = 0;

	//テキストフォントの読み込みと設定
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	//背景の読み込み
	mSky.Load("sky.obj", "sky.mtl");
	//岩の読み込み
	mRock.Load("Rock1.obj", "Rock1.mtl");
	//車の読み込み
	mRover.Load("untitled.obj", "cube.mtl");
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
	//銃の読み込み
	mItemGun.Load("銃.obj", "material\\gun.mtl");

	mCube2.Load("cube.obj", "cube2.mtl");
	mCube3.Load("cube.obj", "cube3.mtl");

	mWater.Load("cube.obj", "water_sumple.mtl");

	//ボートの読み込み
	mBoat.Load("ふね.obj", "cube.mtl");
	//ワープ地点の光
	mWarpPoint.Load("cube.obj", "material\\warp.mtl");

	//バネの読み込み
	mSpringS.Load("ばね.obj", "cube.mtl");
	mSpringL.Load("ばね伸び.obj", "cube.mtl");

	CBullet::mBullet.Load("銃弾.obj", "銃弾.mtl");

	//ステージ1BGMの読み込み
	BGM.Load("BGM\\game_maoudamashii_7_event46.wav");

	mPlayer = new CPlayer();
	mPlayer->mpModel = &mRover;

	mCamY = 0.0f;
	mPutCol = false;

	CTaskManager::Get()->ChangePriority(mPlayer, 15);

	BGM.Repeat();
}


void CSceneGame::Update() {
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(-2.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY+90) * mPlayer->mMatrix;
	//注視点を求める
	c = mPlayer->mPosition;
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f) * mPlayer->mMatrixRotate;

	//カメラの設定
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);

	//背景の描画
	mSky.Render();

	//タスクマネージャの更新・描画
	CTaskManager::Get()->Update();
	CTaskManager::Get()->Render();

	//コリジョンマネージャの衝突処理
	CollisionManager.Collision();
	//TaskManager.Delete();
	CTaskManager::Get()->Delete();

	
	//デバッグ用
#ifdef _DEBUG
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
#endif


	//2D描画開始
	Start2D(0, 800, 0, 600);

	CText::DrawString("STAGE 1", 20, 20, 10, 12);

	//2D描画終了
	End2D();

//	new CObj(&mWarpPoint, CVector(-143.0f, 0.0f, 94.0f), CVector(0.0f, 0.0f, 0.0f), CVector(6.0f, 20.0f, 6.0f), 20);
	//特定の地点(光(のつもり)の範囲内)に行くとStage2へ移行
	if ((CPlayer::mpPlayer->mPosition.mX > -149.0f && CPlayer::mpPlayer->mPosition.mX < -137.0f)
		&& (CPlayer::mpPlayer->mPosition.mZ > 88.0f && CPlayer::mpPlayer->mPosition.mZ < 100.0f)
		&& (CPlayer::mpPlayer->mHaveBoat)){
		mScene = ESTAGE2;
	}
	//デバッグコマンド
	if (CKey::Once(VK_RETURN)){
		mScene = ESTAGE2;
	}

	if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
	}
	if (CKey::Once('2')){//Playerの座標を出力する
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Once('3')){//強制的に的の残数を0にする(本来の的は消えない)
		CItem::mTargetAmount = 0;
	}


	return;
}


//次のシーンの取得
CScene::EScene CSceneGame::GetNextScene(){
	return mScene;
}

