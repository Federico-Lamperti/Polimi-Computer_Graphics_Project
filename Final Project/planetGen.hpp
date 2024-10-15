void createSphereMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx, float radius) {
    int slices = 128;
    int rings = 256;
 
    for (int r = 0; r <= rings; r++) {
        float theta = r * M_PI / rings;

        for (int s = 0; s <= slices; s++) {
            float phi = s * 2 * M_PI / slices;
            
            float x = glm::cos(phi) * glm::sin(theta) * radius;
            float y = glm::cos(theta)* radius;
            float z = glm::sin(phi) * glm::sin(theta)* radius;

            float u = ((float) s) / ((float) slices);
            float v = 1.0f - ((float) r) / ((float) rings);

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
