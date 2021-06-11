#include "CRoadManager.h"
#include "CPoint.h"
#include "CEnemy.h"

//CRoadManager(モデルデータのポインタ,位置,回転,拡大縮小,スタート位置,進行方向)
CRoadManager::CRoadManager(CModel *model, const CVector& position, const CVector& rotation, const CVector& scale, const CVector& startPos, const CVector& foward)
: CObjFloor(model, position, rotation, scale)
{
	Init(model, position, rotation, scale, startPos, foward);
}

void CRoadManager::Init(CModel* pmodel, const CVector& pos, const CVector& rot, const CVector& scale, const CVector& startPos, const CVector& foward)
{
	//mMatrixの更新
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	CCharacter::Update();

	/*printf("//（１）pmodelから三角形ポリゴンの数を取得する\n");
	printf("//（２）ポリゴンの数分ベクトルの配列を作成する。\n");
	printf("//（３）三角形ポリゴンの各頂点にmMatrixを掛けてワールド座標を求め、三角形の重心の座標を求める\n");
	printf("//（４）配列のベクトルの値に、三角形ポリゴンの重心座標を代入していく\n");*/
	//実装１　三角形ポリゴンの重心座標を求めて配列にする
	//（１）pmodelから三角形ポリゴンの数を取得する
	int triangle_size = pmodel->mTriangles.size();
	printf("三角形ポリゴンの数：%d\n", triangle_size);
	
	//（２）ポリゴンの数分ベクトルの配列を作成する。
	CVector *polygonarray;
	polygonarray = new CVector[triangle_size];
	printf("pmodel->mpVertex[0] = %f\n", pmodel->mpVertex[0]);
	for (int i = 0; i < pmodel->mMaterials.size(); i++){
		printf("mVertexNum : %f\n", pmodel->mMaterials[i].mVertexNum);
	}


	////（３）三角形ポリゴンの各頂点にmMatrixを掛けてワールド座標を求め、三角形の重心の座標を求める
	////各コライダの頂点をワールド座標へ変換//参考
	//v[0] = y->mV[0] * y->mMatrix * y->mpParent->mMatrix;
	//v[1] = y->mV[1] * y->mMatrix * y->mpParent->mMatrix;
	//v[2] = y->mV[2] * y->mMatrix * y->mpParent->mMatrix;
	int men = 0;
	for (int i = 0; i < triangle_size; i++){
		polygonarray[i] * mMatrix;
		men++;
		printf("%4d  ",men);
		printf("%f, %f, %f\n", polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ);
	}

	//（４）配列のベクトルの値に、三角形ポリゴンの重心座標を代入していく


	//実装２　先頭データの探索
	//重心座標の配列から、スタート位置に最も近い重心を求める。
	//求めた重心と配列の先頭を入れ替える。

	//実装３　2番目データの探索
	//重心座標の2番目以降の配列について、先頭のデータから進行方向にある重心の中で、
	//最も先頭に近い重心を探し、配列の2番目と入れ替える。

	//実装４　3番目以降のデータの並び変え
	//配列の3番目以降について以下の手順で並び変える
	//（１）現在位置を3番目にする
	//（２）現在位置の一つ前の値に最も近い重心を探し、現在位置と入れ替える
	//（３）現在位置を一つ後ろにして、（２）を繰り返す

	//実装５　重心の配列からCPointを生成する
	//（１）配列の最後と最後から1つ前の重心より、中間座標を求め、CPointを生成する。
	//（２）配列の後3つ目から前に向けて、2つずつ中間座標を求めCPointを生成する。
	//（３）生成し終わると、最初に作成したCPointの次ポインタに最後のCPointのポインタを代入する

	////最初の目標を設定
	//CEnemy::mPoint = 最後のCPointのポインタの次ポインタ;
}
