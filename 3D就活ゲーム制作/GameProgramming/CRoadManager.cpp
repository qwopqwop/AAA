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
		//一番最初の重心座標は無条件に最も近いものとする
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
	

	//実装３　2番目データの探索
	//重心座標の2番目以降の配列について、先頭のデータから進行方向にある重心の中で、
	//最も先頭に近い重心を探し、配列の2番目と入れ替える。
	CVector poly_forward = foward;
	float sdot = 0;
	int sArraynum = 0;
	bool isfirst = true;
	for (int i = 0 + 1; i < triangle_size; i++){
		CVector dir = polygonarray[i] - polygonarray[0];
		//printf("内積：%8.2f\n", polygonarray[i].Dot(polygonarray[i] - polygonarray[0]));
		printf("[%4d]の内積：%8.2f", i, poly_forward.Dot(polygonarray[i] - polygonarray[0]));

		//内積がプラスになるものが候補
		if (poly_forward.Dot(polygonarray[i] - polygonarray[0]) > 0){
			//その中で最もpolygonarray[0]と近いものが[1]となる
			printf(" 　候補\n");

			printf(" 距離：%8.2f\n", (polygonarray[i] - polygonarray[0]).Length());
			vdist = polygonarray[i] - polygonarray[0];
			//一番最初のプラス内積はすぐ設定
			if (isfirst){
				nearestvec = vdist;
				printf("%4d番目  距離：%8.2f\n", i, vdist.Length());
				sArraynum = i;
				isfirst = false;
			}
			else{
				if (vdist.Length() < nearestvec.Length()){
					printf("%4d番目  距離：%8.2f\n", i, vdist.Length());
					nearestvec = vdist;
					sArraynum = i;
				}
			}

		}
		else{
			printf("\n");
		}
	}
	printf("[%d] %f\n", sArraynum,  nearestvec.Length());
	temp = polygonarray[1];
	polygonarray[1] = polygonarray[sArraynum];
	polygonarray[sArraynum] = temp;
	printf("---さらに変換後---\n");
	/*for (int i = 0; i < triangle_size; i++){
		printf("%4d：距離：%8.2f\n", i, (spos - polygonarray[i]).Length());
	}*/
	printf("%4d：距離：%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	printf("%4d：距離：%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());
//	printf("[%d]    %.2f  %.2f  %.2f", sArraynum, polygonarray[sArraynum].mX, polygonarray[sArraynum].mY, polygonarray[sArraynum].mZ);
	////[490]か[492]辺りが2番目になるはず…？
	//printf("[%d] %f\n", sArraynum, sdot);
	//temp = polygonarray[1];
	//polygonarray[1] = polygonarray[sArraynum];
	//polygonarray[sArraynum] = temp;
	//printf("---さらに変換後---\n");
	for (int i = 0; i < triangle_size; i++){
		printf("%4d：距離：%8.2f\n", i, (spos - polygonarray[i]).Length());
	}
	//printf("%4d：距離：%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	//printf("%4d：距離：%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());


	printf("＝＝＝＝ここから実装(４)＝＝＝＝\n");
	//実装４　3番目以降のデータの並び変え
	//配列の3番目以降について以下の手順で並び変える
	//（１）現在位置を3番目にする
	//（２）現在位置の一つ前の値に最も近い重心を探し、現在位置と入れ替える
	//（３）現在位置を一つ後ろにして、（２）を繰り返す	
	sArraynum = 0;
	for (int i = 2; i < triangle_size; i++){
		//i番目より前のデータは並び変えの対象にならない
		isfirst = true;
		for (int j = i; j < triangle_size; j++){
			vdist = polygonarray[i - 1] - polygonarray[j];
			if (isfirst){
				nearestvec = vdist;
				printf("%4d番目  距離：%8.2f\n", j, vdist.Length());
				sArraynum = j;
				isfirst = false;

			}
			else{

				if (vdist.Length() < nearestvec.Length()){
					printf("%4d番目  距離：%8.2f\n", j, vdist.Length());
					nearestvec = vdist;
					sArraynum = j;
				}
			}
		}
		temp = polygonarray[i];
		polygonarray[i] = polygonarray[sArraynum];
		polygonarray[sArraynum] = temp;
		printf("----Change----\n");
	}

	//配列全ての座標を出力する
	for (int i = 0; i < triangle_size; i++){
		printf("[%3d]   X:%10.2f  Y:%10.2f  Z:%10.2f　　 スタート地点からの距離：%8.2f\n", i, polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ, (spos - polygonarray[i]).Length());
	}


	//実装５　重心の配列からCPointを生成する
	//（１）配列の最後と最後から1つ前の重心より、中間座標を求め、CPointを生成する。
	//（２）配列の後3つ目から前に向けて、2つずつ中間座標を求めCPointを生成する。
	//（３）生成し終わると、最初に作成したCPointの次ポインタに最後のCPointのポインタを代入する
	////最初の目標を設定
	//CEnemy::mPoint = 最後のCPointのポインタの次ポインタ;
	printf("実装５　-中心座標編-\n");
	CPoint *next, *first;
	CVector center;
	isfirst = true;
	for (int i = triangle_size - 1; i > 0; i -= 2){
		//中心座標
		center = (polygonarray[i] + polygonarray[i - 1]) * (1.0f / 2.0f);
		printf("X:%10.2f  Y:%10.2f  Z:%10.2f\n", center.mX, center.mY, center.mZ);
		if (isfirst){
			first = next = new CPoint(center, 100.0f, nullptr);
			isfirst = false;
		}
		else{
			next = new CPoint(center, 100.0f, next);
		}		
	}
	//最初に生成したポインタの次ポインタの設定
	first->Set(center, 100.0f, next);
	//CEnemy::mPoint = 最後のCPointのポインタの次ポインタ;
	
	//CPoint *next, *first;
	////後ろから前に向かって生成していく
	//first = next = CEnemy::mPoint54 = new CPoint(CVector(-3543.0f, 64.0f, 16338.0f), 100.0f, nullptr);
	////CPoint生成時に次ポインタを設定していく
	//next = CEnemy::mPoint53 = new CPoint(CVector(-2086.0f, 64.0f, 17555.0f), 100.0f, next);
	//next = CEnemy::mPoint52 = new CPoint(CVector(-244.0f, 64.0f, 18457.0f), 100.0f, next);
	//next = CEnemy::mPoint51 = new CPoint(CVector(2191.0f, 64.0f, 18783.0f), 100.0f, next);
	//next = CEnemy::mPoint50 = new CPoint(CVector(4095.0f, 64.0f, 18200.0f), 100.0f, next);
	//next = CEnemy::mPoint49 = new CPoint(CVector(5310.0f, 64.0f, 17000.0f), 100.0f, next);
	//next = CEnemy::mPoint48 = new CPoint(CVector(6410.0f, 64.0f, 15188.0f), 100.0f, next);
	//next = CEnemy::mPoint47 = new CPoint(CVector(6945.0f, 64.0f, 13182.0f), 100.0f, next);
	//next = CEnemy::mPoint46 = new CPoint(CVector(6830.0f, 64.0f, 10230.0f), 100.0f, next);
	//next = CEnemy::mPoint45 = new CPoint(CVector(7260.0f, 64.0f, 8450.0f), 100.0f, next);
	//next = CEnemy::mPoint44 = new CPoint(CVector(8371.0f, 64.0f, 7235.0f), 140.0f, next);
	//next = CEnemy::mPoint43 = new CPoint(CVector(9530.0f, 64.0f, 7065.0f), 120.0f, next);
	//next = CEnemy::mPoint42 = new CPoint(CVector(9990.0f, 64.0f, 7720.0f), 120.0f, next);
	//next = CEnemy::mPoint41 = new CPoint(CVector(10595.0f, 64.0f, 8612.0f), 120.0f, next);
	//next = CEnemy::mPoint40 = new CPoint(CVector(12230.0f, 64.0f, 8710.0f), 100.0f, next);
	//next = CEnemy::mPoint39 = new CPoint(CVector(13473.0f, 64.0f, 8125.0f), 100.0f, next);
	//next = CEnemy::mPoint38 = new CPoint(CVector(14424.0f, 64.0f, 6273.0f), 100.0f, next);
	//next = CEnemy::mPoint37 = new CPoint(CVector(15461.0f, 64.0f, 165.0f), 100.0f, next);
	//next = CEnemy::mPoint36 = new CPoint(CVector(14829.0f, 64.0f, -2543.0f), 100.0f, next);
	//next = CEnemy::mPoint35 = new CPoint(CVector(12680.0f, 64.0f, -6967.0f), 100.0f, next);
	//next = CEnemy::mPoint34 = new CPoint(CVector(10123.0f, 64.0f, -7222.0f), 100.0f, next);
	//next = CEnemy::mPoint33 = new CPoint(CVector(8055.0f, 64.0f, -6644.0f), 100.0f, next);
	//next = CEnemy::mPoint32 = new CPoint(CVector(7484.0f, 64.0f, -6189.0f), 100.0f, next);
	//next = CEnemy::mPoint31 = new CPoint(CVector(2925.5f, 375.0f, 955.0f), 100.0f, next);
	//next = CEnemy::mPoint30 = new CPoint(CVector(2494.5f, 500.0f, 1206.0f), 100.0f, next);
	//next = CEnemy::mPoint29 = new CPoint(CVector(2091.5f, 584.0f, 897.0f), 100.0f, next);
	//next = CEnemy::mPoint28 = new CPoint(CVector(1002.5f, 955.0f, -645.0f), 100.0f, next);
	//next = CEnemy::mPoint27 = new CPoint(CVector(-1279.0f, 1575.0f, -2653.0f), 100.0f, next);
	//next = CEnemy::mPoint26 = new CPoint(CVector(-4360.0f, 2420.0f, -4840.0f), 100.0f, next);
	//next = CEnemy::mPoint25 = new CPoint(CVector(-6842.0f, 2983.0f, -5783.0f), 100.0f, next);
	//next = CEnemy::mPoint24 = new CPoint(CVector(-9216.0f, 3625.0f, -6044.0f), 100.0f, next);
	//next = CEnemy::mPoint23 = new CPoint(CVector(-13104.0f, 4671.0f, -5555.0f), 100.0f, next);
	//next = CEnemy::mPoint22 = new CPoint(CVector(-14540.0f, 4960.0f, -5115.0f), 100.0f, next);
	//next = CEnemy::mPoint21 = new CPoint(CVector(-16072.0f, 4970.0f, -3791.0f), 100.0f, next);
	//next = CEnemy::mPoint20 = new CPoint(CVector(-15975.0f, 4870.0f, -1342.0f), 100.0f, next);
	//next = CEnemy::mPoint19 = new CPoint(CVector(-14827.0f, 3966.0f, 1878.0f), 100.0f, next);
	//next = CEnemy::mPoint18 = new CPoint(CVector(-13393.0f, 3333.0f, 3628.0f), 100.0f, next);
	//next = CEnemy::mPoint17 = new CPoint(CVector(-9549.0f, 2700.0f, 4348.0f), 100.0f, next);
	//next = CEnemy::mPoint16 = new CPoint(CVector(-7810.0f, 2425.0f, 4349.0f), 100.0f, next);
	//next = CEnemy::mPoint15 = new CPoint(CVector(-7272.0f, 2312.0f, 4513.0f), 100.0f, next);
	//next = CEnemy::mPoint14 = new CPoint(CVector(-6883.0f, 2161.0f, 5038.0f), 100.0f, next);
	//next = CEnemy::mPoint13 = new CPoint(CVector(-7355.0f, 2075.0f, 6084.0f), 100.0f, next);
	//next = CEnemy::mPoint12 = new CPoint(CVector(-9377.0f, 2233.0f, 6699.0f), 100.0f, next);
	//next = CEnemy::mPoint11 = new CPoint(CVector(-9640.0f, 2148.0f, 7427.0f), 100.0f, next);
	//next = CEnemy::mPoint10 = new CPoint(CVector(-9434.0f, 1850.0f, 8411.0f), 100.0f, next);
	//next = CEnemy::mPoint9 = new CPoint(CVector(-8595.0f, 1570.0f, 9218.0f), 100.0f, next);
	//next = CEnemy::mPoint8 = new CPoint(CVector(-7943.0f, 1421.0f, 9162.0f), 100.0f, next);
	//next = CEnemy::mPoint7 = new CPoint(CVector(-7207.0f, 1320.0f, 8818.0f), 100.0f, next);
	//next = CEnemy::mPoint6 = new CPoint(CVector(-5666.0f, 1050.0f, 8679.0f), 100.0f, next);
	//next = CEnemy::mPoint5 = new CPoint(CVector(-4640.0f, 900.0f, 9125.0f), 100.0f, next);
	//next = CEnemy::mPoint4 = new CPoint(CVector(-4156.0f, 675.0f, 10020.0f), 100.0f, next);
	//next = CEnemy::mPoint3 = new CPoint(CVector(-4254.0f, 555.0f, 10627.0f), 100.0f, next);
	//next = CEnemy::mPoint2 = new CPoint(CVector(-4879.0f, 500.0f, 11355.0f), 100.0f, next);
	//next = CEnemy::mPoint = new CPoint(CVector(-5492.0f, 130.0f, 13030.0f), 100.0f, next);
	////最初に生成したポインタの次ポインタの設定
	//first->Set(CVector(-3543.0f, 64.0f, 16338.0f), 100.0f, next);


	delete[] polygonarray;
}
