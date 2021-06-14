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
	/*printf("pmodel->mpVertex[0] = %f\n", pmodel->mpVertex[0]);
	for (int i = 0; i < pmodel->mMaterials.size(); i++){
		printf("mVertexNum : %f\n", pmodel->mMaterials[i].mVertexNum);
	}*/

	////（３）三角形ポリゴンの各頂点にmMatrixを掛けてワールド座標を求め、三角形の重心の座標を求める
	////各コライダの頂点をワールド座標へ変換//参考
	//v[0] = y->mV[0] * y->mMatrix * y->mpParent->mMatrix;
	//v[1] = y->mV[1] * y->mMatrix * y->mpParent->mMatrix;
	//v[2] = y->mV[2] * y->mMatrix * y->mpParent->mMatrix;
	for (int i = 0; i < triangle_size; i++){
		CVector v[3], sv;
		v[0] = pmodel->mTriangles[i].mV[0] * mMatrix;
		v[1] = pmodel->mTriangles[i].mV[1] * mMatrix;
		v[2] = pmodel->mTriangles[i].mV[2] * mMatrix;
		sv = CVector((v[0].mX + v[1].mX + v[2].mX) / 3.0f, (v[0].mY + v[1].mY + v[2].mY) / 3.0f, (v[0].mZ + v[1].mZ + v[2].mZ) / 3.0f);

		//（４）配列のベクトルの値に、三角形ポリゴンの重心座標を代入していく
		polygonarray[i] = sv;
		printf("%4d  　", i + 1);
		printf("%13f, %13f, %13f\n", polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ);
	}


	//実装２　先頭データの探索
	//重心座標の配列から、スタート位置に最も近い重心を求める。
	//求めた重心と配列の先頭を入れ替える。
	CVector spos = startPos;
	CVector vdist;
	CVector nearestvec = CVector(0.0f, 0.0f, 0.0f);
	int nearest_arraynum;
	for (int i = 0; i < triangle_size; i++){
		
		vdist = spos - polygonarray[i];
		//一番最初の重心座標は無条件に最も近いことにする
		if (i == 0){
			nearestvec = vdist;
			printf("%4d番目  距離：%8.2f\n", i, vdist.Length());
			nearest_arraynum = i;
		}
		else{
			if (vdist.Length() < nearestvec.Length()){
				printf("%4d番目  距離：%8.2f\n", i, vdist.Length());
				nearestvec = vdist;
				nearest_arraynum = i;
			}
		}
	}
	printf("最も近い座標の配列…%4d番目の配列\n", nearest_arraynum);
	printf("距離：%8.2f\n", nearestvec.Length());
	printf("距離：%8.2f\n", (spos - polygonarray[nearest_arraynum]).Length());
	//求めた重心と配列の先頭を入れ替える。
	CVector temp = polygonarray[0];
	polygonarray[0] = polygonarray[nearest_arraynum];
	polygonarray[nearest_arraynum] = temp;	
	printf("---変換後---\n");
	printf("%4d：距離：%8.2f\n", 0,(spos - polygonarray[0]).Length());
	printf("%4d：距離：%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	////各コライダの中心座標を求める
	////原点×コライダの変換行列×親の変換行列
	//CVector mpos = CVector() * m->mMatrix * m->mpParent->mMatrix;
	//CVector ypos = CVector() * y->mMatrix * y->mpParent->mMatrix;
	////中心から中心へのベクトルを求める
	//mpos = mpos - ypos;
	////中心の距離が半径の合計より小さいと衝突
	//if (m->mRadius + y->mRadius > mpos.Length()) {
	//	if (mpos.Length() == 0){
	//		//完全に同じ座標の時は半径の和の分、上に移動させる
	//		*a = CVector(0.0f, 1.0f, 0.0f) * (m->mRadius + y->mRadius);//CVector(0.0f, 0.1f, 0.0f);
	//	}
	//	else{
	//		*a = mpos.Normalize() * (m->mRadius + y->mRadius - mpos.Length());
	//	}
	//}

	//実装３　2番目データの探索
	//重心座標の2番目以降の配列について、先頭のデータから進行方向にある重心の中で、
	//最も先頭に近い重心を探し、配列の2番目と入れ替える。
	////ポイントへのベクトルを求める
	//CVector dir = mVPoint - mPosition;
	////左方向へのベクトルを求める
	//CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	//left.Dot(dir)

	CVector poly_forward = foward;
	float sdot = 0;
	int sArraynum = 0;
	for (int i = 0; i < triangle_size; i++){
		CVector dir = polygonarray[i] - polygonarray[0];
		//printf("内積：%8.2f\n", polygonarray[i].Dot(polygonarray[i] - polygonarray[0]));
		printf("[%4d]の内積：%8.2f", i, poly_forward.Dot(polygonarray[i] - polygonarray[0]));

		if (poly_forward.Dot(polygonarray[i] - polygonarray[0]) > 0){
			printf(" 　候補\n");
			if (sdot == 0 || sdot > poly_forward.Dot(polygonarray[i] - polygonarray[0])){
				sdot = poly_forward.Dot(polygonarray[i] - polygonarray[0]);
				sArraynum = i;
			}
			
		}
		else{
			printf("\n");
		}
	}
	//[490]か[492]辺りが2番目になるはず…？
	printf("[%d] %f\n", sArraynum, sdot);
	temp = polygonarray[1];
	polygonarray[1] = polygonarray[sArraynum];
	polygonarray[sArraynum] = temp;
	printf("---さらに変換後---\n");
	/*for (int i = 0; i < triangle_size; i++){
		printf("%4d：距離：%8.2f\n", i, (spos - polygonarray[i]).Length());
	}*/
	printf("%4d：距離：%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	printf("%4d：距離：%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());
	

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

	delete[] polygonarray;
}
