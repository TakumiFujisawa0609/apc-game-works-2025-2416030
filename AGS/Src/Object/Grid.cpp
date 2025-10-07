#include <DxLib.h>
#include "Grid.h"

// コンストラクタ
Grid::Grid(void)
{

}
// デストラクタ
Grid::~Grid(void)
{

}


void Grid::Init(void)
{

}

void Grid::Update(void)
{

}

void Grid::Draw(void)
{
	float num;
	VECTOR sPos = { 0.0f, -HLEN, 0.0f };
	VECTOR ePos = { 0.0f, HLEN, 0.0f };

	for (int i = -HNUM; i < HNUM; i++) {

		// z
		num = static_cast<float>(i);
		sPos = { -HLEN, 0.0f, num * TERM };
		ePos = { HLEN, 0.0f, num * TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(
			{ HLEN, 0.0f, num * TERM },
			30.0f, 32, 0xff0000, 0xff0000, TRUE);

		// x
		sPos = { num * TERM, 0.0f, -HLEN };
		ePos = { num * TERM, 0.0f, HLEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(
			{ num * TERM, 0.0f, HLEN },
			30.0f, 32, 0x0000ff, 0x0000ff, TRUE);
	}

	// y
	sPos = { 0.0f, -HLEN, 0.0f };
	ePos = { 0.0f, HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
}

void Grid::Release(void)
{

}