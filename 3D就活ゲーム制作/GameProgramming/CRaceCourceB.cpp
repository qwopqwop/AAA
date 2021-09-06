#include "CRaceCourceB.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CRoadManager.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CObjJumper.h"
#include "CSceneTitle.h"

void CRaceCourceB::Init(){
	//シーンの設定
	mScene = ERACE2;

	CSceneRace::Init();

	////敵のポイント(目的地)の設定
	//CEnemy::mPointSize = 17;//ポイント数の設定
	//CPoint *next, *first;
	////後ろから前に向かって生成していく
	//first = next = CEnemy::mPoint17 = new CPoint(CVector(2128.0f, 30.0f, -3026.0f), 200.0f, nullptr);
	////CPoint生成時に次ポインタを設定していく
	//next = CEnemy::mPoint17 = new CPoint(CVector(2128.0f, 30.0f, -3026.0f), 200.0f, next);
	//next = CEnemy::mPoint16 = new CPoint(CVector(1317.0f, 30.0f, -3865.0f), 200.0f, next);
	//next = CEnemy::mPoint15 = new CPoint(CVector(529.0f, 30.0f, -3565.0f), 200.0f, next);
	//next = CEnemy::mPoint14 = new CPoint(CVector(-721.0f, 30.0f, -1926.0f), 200.0f, next);
	//next = CEnemy::mPoint13 = new CPoint(CVector(-1500.0f, 30.0f, -80.0f), 200.0f, next);
	//next = CEnemy::mPoint12 = new CPoint(CVector(-2652.0f, 30.0f, 113.0f), 200.0f, next);
	//next = CEnemy::mPoint11 = new CPoint(CVector(-3242.0f, 30.0f, 1253.0f), 200.0f, next);
	//next = CEnemy::mPoint10 = new CPoint(CVector(-2528.0f, 30.0f, 1832.0f), 200.0f, next);
	//next = CEnemy::mPoint9 = new CPoint(CVector(-1645.0f, 30.0f, 3612.0f), 200.0f, next);
	//next = CEnemy::mPoint8 = new CPoint(CVector(-700.0f, 30.0f, 4935.0f), 200.0f, next);
	//next = CEnemy::mPoint7 = new CPoint(CVector(463.0f, 30.0f, 5010.0f), 200.0f, next);
	//next = CEnemy::mPoint6 = new CPoint(CVector(1212.0f, 30.0f, 4516.0f), 200.0f, next);
	//next = CEnemy::mPoint5 = new CPoint(CVector(1719.0f, 30.0f, 3250.0f), 200.0f, next);
	//next = CEnemy::mPoint4 = new CPoint(CVector(3240.0f, 30.0f, 2207.0f), 200.0f, next);
	//next = CEnemy::mPoint3 = new CPoint(CVector(3763.0f, 30.0f, 510.0f), 200.0f, next);
	//next = CEnemy::mPoint2 = new CPoint(CVector(3068.0f, 30.0f, -355.0f), 200.0f, next);
	//next = CEnemy::mPoint = new CPoint(CVector(2377.0f, 30.0f, -753.0f), 200.0f, next);
	////最初に生成したポインタの次ポインタの設定
	//first->Set(CVector(2128.0f, 30.0f, -3026.0f), 200.0f, next);

	//プレイヤーの生成
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//カメラの生成
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();

	//コース全体のサイズ感を設定
	float mtsize = 42.0f;
	float height = 13.5f;
	////new CObjFloor(&mCource05Road, CVector(0.0f, 21.0f, 0.0f), CVector(), CVector(mtsize, height, mtsize));
	//mpGrounds[0] = new CRoadManager(&mCource05Road, CVector(0.0f, 21.0f, 0.0f), CVector(), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, -1.0f), 120.0f);//
	
	mpGrounds[0] = new CRoadManager(&mCource02Road, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, 1.0f), 120.0f);
	//mpGrounds[0] = new CObjFloor(&mCource02Road, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f));

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
		/*mStartPoint[0] = 2222.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -2510.0f;
		mStartRotation = 0.0f;*/
		//初期の配置座標を設定する
		mEnemys[i]->mPosition = CVector(2222.0f, -13.538f, -2510.0f - 30.0f - 80.0f*i);
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mX -= 80.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	/* ※透明度の高い物から先に描画する */
	//中間地点(順に通らないと1周したことにならないし、順番を飛ばしてもいけない)
	new CObjCheckPoint(&mCheckPoint, CVector(2893.0f, -100.0f, 2473.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(-1020.0f, -100.0f, 4594.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(-1277.0f, -100.0f, -448.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 3);

	//ここから
	//コースの生成
//	
	//new CRoadManager(&mCource02Road, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f), mPlayer->mPosition, CVector(0.0f, 0.0f, -1.0f));//
	mpGrounds[1] = new CObjWall(&mCource02Wall, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f));
	mpGrounds[2] = new CObjJumper(&mCource02Jump, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f));
	//ゴール地点
	mpGrounds[3] = new CObjNonCol(&mMiniGoal, CVector(2315.0f, -1.0f, -2300.0f), CVector(0.0f, 0.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f));
	mpGrounds[4] = new CObjNonCol(&mMiniGoal, CVector(2107.0f, -1.0f - 0.05f, -2300.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f));
	//ここまでがmpGroundにあたる

	//優先度変更
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		if (mEnemys[i]->mpModel == &mCarRed){
			printf("CPU%d RED\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarBlue){
			printf("CPU%d BLUE\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarBlack){
			printf("CPU%d BLACK\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarCyan){
			printf("CPU%d CYAN\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarGreen){
			printf("CPU%d GREEN\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarGray){
			printf("CPU%d GRAY\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarPink){
			printf("CPU%d PINK\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarWhite){
			printf("CPU%d WHITE\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarYellow){
			printf("CPU%d YELLOW\n", i + 1);
		}
	}
}
void CRaceCourceB::Update(){
	CSceneRace::Update();
}