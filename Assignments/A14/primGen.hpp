void Assignment14::createCubeMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
    // The primitive built here is a cube centered in the origin, whose edge lenght is 2.
    
	// Fill vDef with the positions of the vertices and of the normal vectors of the mesh
    // Cube Base
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});     // vertex 0 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});      // vertex 1 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});    // vertex 2 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});     // vertex 3 - Position and Normal
    // Cube Top
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});       // vertex 4 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});        // vertex 5 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});      // vertex 6 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});       // vertex 7 - Position and Normal
    // Cube Right Side
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});      // vertex 8 - Position and Normal
    vDef.push_back({{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});       // vertex 9 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});       // vertex 10 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});        // vertex 11 - Position and Normal
    // Cube Left Side
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});    // vertex 12 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});     // vertex 13 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});     // vertex 14 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});      // vertex 15 - Position and Normal
    // Cube Front Side
    vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});       // vertex 16 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});      // vertex 17 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});        // vertex 18 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});       // vertex 19 - Position and Normal
    // Cube Back Side
    vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});     // vertex 20 - Position and Normal
    vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});    // vertex 21 - Position and Normal
    vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});      // vertex 22 - Position and Normal
    vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});     // vertex 23 - Position and Normal
    
	// Fill vIdx with the indexes of the vertices of the triangles
    for (int i = 0; i < 6; i++) {
        vIdx.push_back(i * 4); vIdx.push_back(i * 4 + 1); vIdx.push_back(i * 4 + 2);        // First triangle
        vIdx.push_back(i * 4 + 1); vIdx.push_back(i * 4 + 2); vIdx.push_back(i * 4 + 3);    // Second triangle
    }
}

void Assignment14::createFunctionMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.
	
    int resolution = 600;               // Number of steps to compute the final image
    float deltaRes = 6.0f/resolution;   // Distance between steps
    Vertex vertex;                      // Vertex that will be stored in buffer
    
    // Fill vDef with the positions of the vertices and of the normal vectors of the mesh
    for (int xf = 0; xf <= resolution ; xf++) {
        for (int zf = 0; zf <= resolution ; zf++) {
            // Position
            float x = -3.0f + xf * deltaRes;
            float z = -3.0f + zf * deltaRes;
            glm::vec3 pos(x, sin(x) * cos(z), z);
            
            // Normal
            glm::vec3 partialX(1.0f, cos(x) * cos(z), 0.0f);
            glm::vec3 partialZ(0.0f, sin(x) * -sin(z), 1.0f);
            glm::vec3 norm = glm::normalize(glm::cross(partialZ, partialX));
            
            vertex.pos = pos;
            vertex.norm = norm;
            vDef.push_back(vertex);
        }
    }
    
    // Fill vIdx with the indexes of the vertices of the triangles
    for (int xf = 0; xf < resolution; xf++) {
            for (int zf = 0; zf < resolution; zf++) {
                int up = xf * (resolution + 1) + zf;
                int down = (xf + 1) * (resolution + 1) + zf;

                // Build the squares
                vIdx.push_back(down); vIdx.push_back(down + 1); vIdx.push_back(up);      // First Triangle
                vIdx.push_back(down + 1); vIdx.push_back(up); vIdx.push_back(up + 1);    // Second Triangle
            }
        }
}

void Assignment14::createCylinderMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.
    
    // Number of Vertexes
    int vertexes = 64;
    // Angles
    float currAngle = 0.0f;
    float deltaAngle = 360.0/vertexes;
    
    // Fill vDef with the positions of the vertices and of the normal vectors of the mesh
    vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});      // Top Center
    vDef.push_back({{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});    // Bottom Center
    
    for (int i = 0; i < vertexes; i++) {
        float currRad = glm::radians(currAngle);
        // Top Vertexes
        vDef.push_back({{cos(currRad), 1.0f, sin(currRad)}, {0.0f, 1.0f, 0.0f}});                     // Base Vertex
        vDef.push_back({{cos(currRad), 1.0f, sin(currRad)}, {cos(currRad), 0.0f, sin(currRad)}});     // Side Vertex
        
        // Bottom Vertexes
        vDef.push_back({{cos(currRad), -1.0f, sin(currRad)}, {0.0f, -1.0f, 0.0f}});                   // Base Vertex
        vDef.push_back({{cos(currRad), -1.0f, sin(currRad)}, {cos(currRad), 0.0f, sin(currRad)}});    // Side Vertex
        
        // Update Angle
        currAngle += deltaAngle;
    }
    
    // Fill vIdx with the indexes of the vertices of the triangles
    for (int i = 0; i < vertexes * 4; i += 4) {
        // Top Base
        vIdx.push_back(0); vIdx.push_back(i + 2); vIdx.push_back(i + 6);
        // Bottom Base
        vIdx.push_back(1); vIdx.push_back(i + 4); vIdx.push_back(i + 8);

        // Rectangular Side
        vIdx.push_back(i + 3); vIdx.push_back(i + 5); vIdx.push_back(i + 7);
        vIdx.push_back(i + 5); vIdx.push_back(i + 7); vIdx.push_back(i + 9);
    }
	
    // Code to print the last portion of the cylinder
    // vDef.size() = 258
    // Top Base
    vIdx.push_back(0); vIdx.push_back(2); vIdx.push_back(254);
    // Bottom Base
    vIdx.push_back(1); vIdx.push_back(4); vIdx.push_back(256);

    // Rectangular Side
    vIdx.push_back(3); vIdx.push_back(5); vIdx.push_back(255);
    vIdx.push_back(5); vIdx.push_back(255); vIdx.push_back(257);
}
