#pragma once

#include <DirectXMath.h>
#include "IncredibleMath.h"

using namespace IncredibleMath;

struct Vertex {
public:
	Vector3 vertex;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 texture;

	Vertex(Vector3 v, Vector3 c) : vertex(v), pos(v.x, v.y, v.z), color(c.r, c.g, c.b) {
		texture = DirectX::XMFLOAT2(0, 0);
	}

	Vertex(Vector3 v, Vector2 tc) : vertex(v), pos(v.x, v.y, v.z), texture(tc.x, tc.y) { 
		color = DirectX::XMFLOAT3(0, 0, 0);
	}
};

