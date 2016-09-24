#include "pch.h"
#include "Landscape.h"
#include <Kore/Math/Random.h>

using namespace Kore;

Kore::VertexBuffer* landscapeVertices;
Kore::IndexBuffer* landscapeIndices;

void createLandscape() {
	Kore::Image* map = new Kore::Image("map.png", true);

	const float size = 50;
	const int w = 50;
	const int h = 50;

	VertexStructure structure;
	structure.add("pos", Float3VertexData);
	structure.add("tex", Float2VertexData);
	structure.add("nor", Float3VertexData);
	landscapeVertices = new Kore::VertexBuffer((w + 1) * (h + 1), structure);

	float* vertices = landscapeVertices->lock();
	int i = 0;
	for (int y = 0; y <= h; ++y) {
		for (int x = 0; x <= w; ++x) {
			int color = 0xff00 & map->at(static_cast<int>(x / (float)(w + 1) * map->width), static_cast<int>(y / (float)(h + 1) * map->height));
			color >>= 8;
			vertices[i++] = -size / 2 + size / w * x; vertices[i++] = color / 255.0f * 10.0f; vertices[i++] = -size / 2 + size / h * y; vertices[i++] = 0; vertices[i++] = 0; vertices[i++] = 0; vertices[i++] = 1; vertices[i++] = 0;
		}
	}
	landscapeVertices->unlock();

	landscapeIndices = new IndexBuffer(w * h * 6);
	int* indices = landscapeIndices->lock();
	i = 0;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int baseindex = y * (w + 1) + x;
			indices[i++] = baseindex;
			indices[i++] = baseindex + 1;
			indices[i++] = baseindex + (w + 1);

			indices[i++] = baseindex + 1;
			indices[i++] = baseindex + (w + 1);
			indices[i++] = baseindex + (w + 1) + 1;
		}
	}
	landscapeIndices->unlock();
}

void renderLandscape(Kore::ConstantLocation mLocation, Kore::ConstantLocation nLocation) {
	Graphics::setMatrix(mLocation, mat4::Identity());
	Graphics::setMatrix(nLocation, mat4::Identity());

	Graphics::setVertexBuffer(*landscapeVertices);
	Graphics::setIndexBuffer(*landscapeIndices);
	Graphics::drawIndexedVertices();
}
