#include "CCollider.h"
//コリジョンマネージャのインクルード
#include "CCollisionManager.h"

//コンストラクタ
//CCollider(親, 位置, 回転, 拡縮, 半径)
CCollider::CCollider(CCharacter *parent, CVector position, CVector rotation, CVector scale, float radius)
: CCollider()
{
	//親設定
	mpParent = parent;
	//CTransform設定
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	CTransform::Update();//行列更新
	//半径設定
	mRadius = radius;
	//コリジョンリストに追加
	CollisionManager.Add(this);
	ChangePriority();
}

CCollider::~CCollider() {
	//コリジョンリストから削除
	CollisionManager.Remove(this);
}

//描画
void CCollider::Render() {

	CMatrix m;

	glPushMatrix();
	glMultMatrixf((mMatrix * mpParent->mMatrix).mM[0]);

	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ライトオフ
	glDisable(GL_LIGHTING);

	//DIFFUSE赤色設定
	float c[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	//?
	//コライダタイプの判定
	switch (mType) {
	case ESPHERE:
		m.Scale(1 / mpParent->mScale.mX, 1 / mpParent->mScale.mY, 1 / mpParent->mScale.mZ);
		glMultMatrixf(m.mM[0]);
		//球描画
		glutWireSphere(mRadius, 16, 16);
		break;
	case ETRIANGLE:
		m.Scale(1.01f, 1.01f, 1.01f);
		glMultMatrixf(m.mM[0]);
		//三角形描画
		glBegin(GL_TRIANGLES);
		glVertex3f(mV[0].mX, mV[0].mY, mV[0].mZ);
		glVertex3f(mV[1].mX, mV[1].mY, mV[1].mZ);
		glVertex3f(mV[2].mX, mV[2].mY, mV[2].mZ);
		glEnd();
		break;
	case ELINE:
		m.Scale(1.01f, 1.01f, 1.01f);
		glMultMatrixf(m.mM[0]);
		//線分描画
		glBegin(GL_LINES);
		glVertex3f(mV[0].mX, mV[0].mY, mV[0].mZ);
		glVertex3f(mV[1].mX, mV[1].mY, mV[1].mZ);
		glEnd();
		break;
	}

	//ライトオン
	glEnable(GL_LIGHTING);
	//アルファブレンド無効
	glDisable(GL_ALPHA);

	glPopMatrix();
}

//衝突判定
//Collision(コライダ1, コライダ2)
//retrun:true（衝突している）false(衝突していない)
bool CCollider::Collision(CCollider *m, CCollider *y) {
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = CVector() * m->mMatrix * m->mpParent->mMatrix;
	CVector ypos = CVector() * y->mMatrix * y->mpParent->mMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - ypos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + y->mRadius > mpos.Length()) {
		//衝突している
		return  true;
	}
	//衝突していない
	return false;
}

CCollider::CCollider()
: mpParent(0)
, mType(ESPHERE)
, mTag(ENONE)
{

}

//コンストラクタ（三角コライダ）
//CCollider(親, 頂点1, 頂点2, 頂点3)
CCollider::CCollider(CCharacter *parent, const CVector &v0, const CVector &v1, const CVector &v2)
: CCollider()
{
	//
	SetTriangle(parent, v0, v1, v2);
}

void CCollider::SetTriangle(CCharacter *parent, const CVector &v0, const CVector &v1, const CVector &v2) {
	mType = ETRIANGLE;
	mpParent = parent;//親設定
	//三角形頂点設定
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
	//スケール1倍
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();//行列更新
	//コリジョンリストに追加
	CollisionManager.Add(this);
	ChangePriority();
}

//コンストラクタ（線分コライダ）
//CCollider(親, 頂点1, 頂点2)
CCollider::CCollider(CCharacter *parent, const CVector &v0, const CVector &v1) {
	SetLine(parent, v0, v1);
}

//線分コライダの設定
//SetLine(親, 頂点1, 頂点2)
void CCollider::SetLine(CCharacter *parent, const CVector &v0, const CVector &v1) {
	mType = ELINE;
	mpParent = parent;//親設定
	//線分の頂点設定
	mV[0] = v0;
	mV[1] = v1;
	//スケール1倍
	mScale = CVector(1.0f, 1.0f, 1.0f);
	CTransform::Update();//行列更新
	//コリジョンリストに追加
	CollisionManager.Add(this);
	ChangePriority();
}

//CollisionTriangleLine(三角コライダ, 線分コライダ, 調整値)
//retrun:true（衝突している）false(衝突していない)
//調整値:衝突しない位置まで戻す値
bool CCollider::CollisionTriangleLine(CCollider *t, CCollider *l, CVector *a) {
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * t->mMatrix * t->mpParent->mMatrix;
	v[1] = t->mV[1] * t->mMatrix * t->mpParent->mMatrix;
	v[2] = t->mV[2] * t->mMatrix * t->mpParent->mMatrix;
	sv = l->mV[0] * l->mMatrix * l->mpParent->mMatrix;
	ev = l->mV[1] * l->mMatrix * l->mpParent->mMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];

	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している

	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が面の向こう側
		*a = normal * -dots;
	}
	else {
		//終点が面の向こう側
		*a = normal * -dote;
	}
	return true;
}

//CollisionTriangleLine(三角コライダ, 線分コライダ, 調整値)
//retrun:true（衝突している）false(衝突していない)
//調整値:衝突しない位置まで戻す値
bool CCollider::CollisionTriangleSphere(CCollider *t, CCollider *s, CVector *a) {
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * t->mMatrix * t->mpParent->mMatrix;
	v[1] = t->mV[1] * t->mMatrix * t->mpParent->mMatrix;
	v[2] = t->mV[2] * t->mMatrix * t->mpParent->mMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();

	sv = s->mV[0] * s->mMatrix * s->mpParent->mMatrix + normal * s->mRadius;
	ev = sv - normal * s->mRadius * 2;
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];

	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している

	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が面の向こう側
		*a = normal * -dots;
	}
	else {
		//終点が面の向こう側
		*a = normal * -dote;
	}
	return true;
}

void CCollider::ChangePriority(){
	CMatrix matrix = mMatrix;//コライダの行列退避
	CVector position;//原点からの距離計算用

	if (mpParent){
		//親がいれば親の合成行列も適用
		matrix = matrix * mpParent->mMatrix;
	}

	switch (mType){
	case ESPHERE://球コライダの時、球の中心座標を求める
		position = CVector() * matrix;
		break;
	case ETRIANGLE://三角形の重心座標を求める
		//三角形の頂点を移動させて合計
		position = mV[0] * matrix + mV[1] * matrix + mV[2] * matrix;
		position = position * (1.0 / 3.0f);//合計値の3分の1が重心
		break;
	case ELINE://線分の中心座標を求める
		//始点と終点を移動させて合計
		position = mV[0] * matrix + mV[1] * matrix;
		position = position*(1.0 / 2.0f);//合計値の2分の1が中心
		break;
	}
	SetPriority(position.Length());
	CollisionManager.Remove(this);//リストから削除する
	CollisionManager.Add(this);//リストに追加する
}

//ChangePriority(優先度)
void CCollider::ChangePriority(int priority){
	SetPriority(priority);
	CollisionManager.Remove(this); //リストから削除する
	CollisionManager.Add(this); //リストに追加する
}

//衝突判定
//Collision(コライダ1, コライダ2, 調整値)
//return:true（衝突している）false(衝突していない)
bool CCollider::Collision(CCollider *m, CCollider *y ,CVector *a) {
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = CVector() * m->mMatrix * m->mpParent->mMatrix;
	CVector ypos = CVector() * y->mMatrix * y->mpParent->mMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - ypos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + y->mRadius > mpos.Length()) {
		if (mpos.Length() == 0){
			//完全に同じ座標の時は半径の和の分、上に移動させる
			*a = CVector(0.0f, 1.0f, 0.0f) * (m->mRadius + y->mRadius);//CVector(0.0f, 0.1f, 0.0f);
		}
		else{
			*a = mpos.Normalize() * (m->mRadius + y->mRadius - mpos.Length());
		}
		//mpos.Length() 
		//衝突している
		return  true;		
	}
	//衝突していない
	return false;
}

//角度を計算する物
CVector CCollider::CalculateEulerAngle(CCollider *m, CCollider *y, CMatrix matrixrotate, double pi){
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = y->mV[0] * y->mMatrix * y->mpParent->mMatrix;
	v[1] = y->mV[1] * y->mMatrix * y->mpParent->mMatrix;
	v[2] = y->mV[2] * y->mMatrix * y->mpParent->mMatrix;
	/*面の法線を、外積を正規化して求める*/
	// 斜面の法線ベクトルからY軸ベクトルを求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	// 車体の進行方向から、Z軸ベクトルを求める
	CVector preZvec = CVector(0.0f, 0.0f, 1.0f) * matrixrotate;
	// Y軸ベクトルとZ軸ベクトルの外積を計算し、X軸ベクトルを求める
	CVector Xvec = (normal).Cross(preZvec).Normalize();
	// X軸ベクトルとY軸ベクトルの外積を計算し、Z軸ベクトルを求める
	CVector Zvec = (Xvec).Cross(normal).Normalize();				
	// 回転値を求める
	float rad = asin(Zvec.mY);//5.
	float rotX = rad * 180 / pi * -1;//X軸は反転
	float rotY = atan2(Zvec.mX, Zvec.mZ) * 180 / pi;//6.
	float rotZ = atan2(Xvec.mY, normal.mY) * 180 / pi;//7.
	// 求めた回転の値を返す(主に車体に適用)
	return CVector(rotX, rotY, rotZ);
}