
void Assignment15::createBoxMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a box centered in the origin, with proportions respecting the texture.
    // Texture dimensions
    float textureWidth = 1024.0f;
    float textureHeight = 512.0f;
    // Object Proportions
    float objectWidth = 1 / 3.667;
    float objectHeight = 1;
    float objectDepth = 1;

    // Fill vPos with positions, normals and UVs
    // Base
    vDef.push_back({{objectWidth, -objectHeight, -objectDepth}, {0.0f, -1.0f, 0.0f}, {96.0 / textureWidth, 512.0 / textureHeight}});
    vDef.push_back({{objectWidth, -objectHeight, objectDepth}, {0.0f, -1.0f, 0.0f}, {442.0 / textureWidth, 512.0 / textureHeight}});
    vDef.push_back({{-objectWidth, -objectHeight, -objectDepth}, {0.0f, -1.0f, 0.0f}, {96.0 / textureWidth, 421.0 / textureHeight}});
    vDef.push_back({{-objectWidth, -objectHeight, objectDepth}, {0.0f, -1.0f, 0.0f}, {442.0 / textureWidth, 421.0 / textureHeight}});
    // Top
    vDef.push_back({{objectWidth, objectHeight, -objectDepth}, {0.0f, 1.0f, 0.0f}, {442.0 / textureWidth, 90.0 / textureHeight}});
    vDef.push_back({{objectWidth, objectHeight, objectDepth}, {0.0f, 1.0f, 0.0f}, {96.0 / textureWidth, 90.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, -objectDepth}, {0.0f, 1.0f, 0.0f}, {442.0 / textureWidth, 0.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, objectDepth}, {0.0f, 1.0f, 0.0f}, {96.0 / textureWidth, 0.0 / textureHeight}});
    // Right Side
    vDef.push_back({{objectWidth, -objectHeight, -objectDepth}, {1.0f, 0.0f, 0.0f}, {442.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{objectWidth, -objectHeight, objectDepth}, {1.0f, 0.0f, 0.0f}, {96.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{objectWidth, objectHeight, -objectDepth}, {1.0f, 0.0f, 0.0f}, {442.0 / textureWidth, 96.0 / textureHeight}});
    vDef.push_back({{objectWidth, objectHeight, objectDepth}, {1.0f, 0.0f, 0.0f}, {96.0 / textureWidth, 96.0 / textureHeight}});
    // Left Side
    vDef.push_back({{-objectWidth, -objectHeight, -objectDepth}, {-1.0f, 0.0f, 0.0f}, {540.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{-objectWidth, -objectHeight, objectDepth}, {-1.0f, 0.0f, 0.0f}, {887.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, -objectDepth}, {-1.0f, 0.0f, 0.0f}, {540.0 / textureWidth, 96.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, objectDepth}, {-1.0f, 0.0f, 0.0f}, {887.0 / textureWidth, 96.0 / textureHeight}});
    // Front Side
    vDef.push_back({{objectWidth, -objectHeight, objectDepth}, {0.0f, 0.0f, 1.0f}, {92.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{-objectWidth, -objectHeight, objectDepth}, {0.0f, 0.0f, 1.0f}, {0.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{objectWidth, objectHeight, objectDepth}, {0.0f, 0.0f, 1.0f}, {92.0 / textureWidth, 96.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, objectDepth}, {0.0f, 0.0f, 1.0f}, {0.0 / textureWidth, 96.0 / textureHeight}});
    // Back Side
    vDef.push_back({{objectWidth, -objectHeight, -objectDepth}, {0.0f, 0.0f, -1.0f}, {445.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{-objectWidth, -objectHeight, -objectDepth}, {0.0f, 0.0f, -1.0f}, {536.0 / textureWidth, 415.0 / textureHeight}});
    vDef.push_back({{objectWidth, objectHeight, -objectDepth}, {0.0f, 0.0f, -1.0f}, {445.0 / textureWidth, 96.0 / textureHeight}});
    vDef.push_back({{-objectWidth, objectHeight, -objectDepth}, {0.0f, 0.0f, -1.0f}, {536.0 / textureWidth, 96.0 / textureHeight}});

	// Fill vIdx with the indices of the vertices of the triangles
    for (int i = 0; i < 6; i++) {
        vIdx.push_back(i * 4); vIdx.push_back(i * 4 + 1); vIdx.push_back(i * 4 + 2);        // First triangle
        vIdx.push_back(i * 4 + 1); vIdx.push_back(i * 4 + 2); vIdx.push_back(i * 4 + 3);    // Second triangle
    }
}


#define M_PI 3.141595f
void Assignment15::createSphereMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a sphere of radius 1, centered in the origin, on which the Mars texture is applied seamless.
    
    int slices = 128;
    int rings = 256;

    for (int r = 0; r <= rings; r++) {
        float theta = r * M_PI / rings;

        for (int s = 0; s <= slices; s++) {
            float phi = s * 2 * M_PI / slices;

            float x = glm::cos(phi) * glm::sin(theta);
            float y = glm::cos(theta);
            float z = glm::sin(phi) * glm::sin(theta);

            float u = ((float) s) / ((float) slices);
            float v = ((float) r) / ((float) rings);

            glm::vec3 point = { x, y, z };
            vDef.push_back({ point, glm::normalize(point), {u, v} });
        }
    }

    // Fill vIdx Buffer
    for (int r = 0; r < rings; r++) {
        for (int s = 0; s <= slices; s++) {
            int current = r * (slices + 1) + s;
            int next = current + slices + 1;

            vIdx.push_back(current); vIdx.push_back(next); vIdx.push_back(current + 1);
            vIdx.push_back(next); vIdx.push_back(next + 1); vIdx.push_back(current + 1);
        }
    }
    
}

