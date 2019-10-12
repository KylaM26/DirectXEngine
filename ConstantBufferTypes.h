#pragma once

#include <DirectXMath.h>

struct CB_VS_VertexShader {
public:
	float xOffset;
	float yOffset; // Both 4 bytes each (becomes 16 for byte width) MAKE 16 BYTE CHUNKS
	CB_VS_VertexShader() : xOffset(0), yOffset(0) {}
};
