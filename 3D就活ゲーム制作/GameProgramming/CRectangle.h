#ifndef CRECTANGLE_H
#define CRECTANGLE_H
#include "CTexture.h"
#include "glut.h"
#include <vector>

class CRectangle {
public:
	////�f�t�H���g�R���X�g���N�^
	//CRectangle();

	//virtual void Update(){} //virtual�͉��z

	//virtual void Render();  //���z�֐��ɂ���

	//int x;
	//int y, w, h;

	static void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

	static void DrawRectangle(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);

	static void Render(int x, int y, int w, int h);

	//static void Render2D(const CTexture &t, int left, int right, int bottom, int top);
};

////�ϒ��z��̒�`
//extern std::vector<CRectangle*> VectorRect;

#endif