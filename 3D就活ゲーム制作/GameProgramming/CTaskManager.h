#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H
//タスククラスのインクルード
#include "CTask.h"

/*
タスクマネージャ
タスクリストの管理
*/
class CTaskManager {
private:
	static CTaskManager *instance;
protected:
	CTask *mpHead;	//先頭ポインタ
public:
	static CTaskManager *Get();

//	CTask *mpTail;	//最後ポインタ
	//デフォルトコンストラクタ
	CTaskManager();
	//デストラクタ
	virtual ~CTaskManager();
	//リストに追加
	//Add(タスクのポインタ)
	void Add(CTask *task);
	//更新
	void Update();
	//描画
	void Render();
	//リストから削除
	//Remove(タスクのポインタ)
	void Remove(CTask *task);
	//タスクの削除
	void Delete();
	//優先度の変更
	void ChangePriority(CTask *t, int p);

	
	
	void Destroy();

	void Disabled();

	void TaskCollision();
};

//タスクマネージャyの外部参照
extern CTaskManager TaskManager;

#endif
