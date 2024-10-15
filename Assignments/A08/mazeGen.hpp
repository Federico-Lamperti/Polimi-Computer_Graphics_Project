void Assignment08::createMazeMesh(int row, int col, char **maze) {
	// The procedure gets in input the number of rows <row> of the maze, and the number of columns <col>
	// Element <maze[r][c]>, with 0 <= r <= row-1 and 0 <= c <= col-1 contains:
	//		Symbol ' ' if there is an empty space
	//		Symbol '#' if there is a wall
	//
	// Example: The following piece of code executes the instruction in the ... section
	//          if there is a wall in position r=5, c=7
	//
	// int r, c;
	// r = 5; c = 7;
	// if(maze[r][c] == '#') {
	//		...
	// }
	//
	// The example below creates a square, with four vertices and two triangles.
	// It must be deleted and replaced with your code creating the mesh of the maze.
	
    // Floor generation
	// Fill array vPos with the positions of the vertices of the mesh
	vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(0.0);	    // vertex 0
	vPos.push_back(row); vPos.push_back(0.0); vPos.push_back(0.0);	    // vertex 1
	vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(col);	    // vertex 2
	vPos.push_back(row); vPos.push_back(0.0); vPos.push_back(col);	    // vertex 3

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);  // First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);  // Second triangle
    
    // Maze Generation
    int vertexes[row+1][col+1];          // Matrix to store if the vertex needs to be pushed
    for (int i = 0; i < row+1; i++){
        for (int j = 0; j < col+1; j++){
            vertexes[i][j] = 0;
        }
    }
    
    for (int i = 0; i<row; i++){
        for (int j = 0; j<col; j++){
            if(maze[i][j] == '#'){
                // Vertex Nord-Ovest
                if(vertexes[i][j] == 0)
                    vertexes[i][j] = 1;
                else
                    vertexes[i][j] = 0;
                // Vertex Sud-Ovest
                if(vertexes[i+1][j] == 0)
                    vertexes[i+1][j] = 1;
                else
                    vertexes[i+1][j] = 0;
                // Vertex Nord-Est
                if(vertexes[i][j+1] == 0)
                    vertexes[i][j+1] = 1;
                else
                    vertexes[i][j+1] = 0;
                // Vertex Sud-Est
                if(vertexes[i+1][j+1] == 0)
                    vertexes[i+1][j+1] = 1;
                else
                    vertexes[i+1][j+1] = 0;
            }
        }
    }
    
    int count = 4;    // # of vertex inside vIdx
    int k = 0;        // 2 triangles at a time
    int index = 5;    // to store the index of the vertexes inside the buffer
    
    // Row parallel Walls
    for (int i = 0; i < row+1; i++){
        for (int j = 0; j < col+1; j++){
            if (vertexes[i][j] == 1) {
                vPos.push_back(i); vPos.push_back(0.0); vPos.push_back(j);    // Vertex 4 + 2n
                vPos.push_back(i); vPos.push_back(1.0); vPos.push_back(j);    // Vertex 5 + 2n
                k++;
                // Insert upper vertexes' indexes inside the matrix
                vertexes[i][j] = index;
                index += 2;
            }
            if (k == 2) {
                k = 0;
                vIdx.push_back(count); vIdx.push_back(count+1); vIdx.push_back(count+2);
                vIdx.push_back(count+1); vIdx.push_back(count+2); vIdx.push_back(count+3);
                count += 4;
            }
        }
    }
    
    // Column parallel Walls
    int vertex_index = 0;    // To push the index inside the vIdx buffer
    k = 0;
    for(int i = 0; i < row+1; i++){
        for(int j = 0; j < col+1; j++){
            if(vertexes[j][i] != 0){
                if(k == 1){
                    k = 0;
                    vIdx.push_back(vertex_index-1); vIdx.push_back(vertex_index); vIdx.push_back(vertexes[j][i]-1);
                    vIdx.push_back(vertexes[j][i]); vIdx.push_back(vertexes[j][i]-1); vIdx.push_back(vertex_index);
                } else {
                    k++;
                }
                vertex_index = vertexes[j][i];
            }
        }
    }
    for (int i = 0; i<row+1; i++) {
        for (int j=0; j<col+1; j++) {
            std::cout<<vertexes[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    
    // Ceiling
    int v1 = -1, v2 = -1, v3 = -1, v4 = -1;   // Indexes for the vertexes
    
    // Ceiling by rows
    for (int i = 0; i < row; i+= 2){
        for (int j = 0; j < col; j++){
            if (maze[i][j] == '#' && maze[i][j+1] == '#') {
                if (v1 == -1 && v2 == -1) {
                    if (vertexes[i][j] != 0)
                        v1 = vertexes[i][j];
                    else
                        v1 = vertexes[i][j+1];
                    if(vertexes[i+1][j] == 0)
                        v2 = vertexes[i+1][j+1];
                    else
                        v2 = vertexes[i+1][j];
                }
            }
            else if ((maze[i][j] == '#' && maze[i][j+1] == ' ') || j == col-1){
                if(maze[i][j-1] == '#'){
                    if(vertexes[i][j] != 0)
                        v3 = vertexes[i][j];
                    else
                        v3 = vertexes[i][j+1];
                    if(vertexes[i+1][j] != 0)
                        v4 = vertexes[i+1][j];
                    else
                        v4 = vertexes[i+1][j+1];
                    // Print Triangles
                    vIdx.push_back(v1); vIdx.push_back(v2); vIdx.push_back(v4);
                    vIdx.push_back(v1); vIdx.push_back(v3); vIdx.push_back(v4);
                    // Reset indexes
                    v1 = -1;
                    v2 = -1;
                    v3 = -1;
                    v4 = -1;
                }
            }
        }
    }
    
    // Ceiling by columns
    for (int j = 0; j < col; j+=2) {
        for (int i = 0; i < row; i++) {
            if (i < row-1 && maze[i][j] == '#' && maze[i+1][j] == '#') {
                if (v1 == -1 && v2 == -1) {
                    if (vertexes[i][j] != 0)
                        v1 = vertexes[i][j];
                    else
                        v1 = vertexes[i+1][j];
                    if(vertexes[i][j+1] != 0)
                        v2 = vertexes[i][j+1];
                    else
                        v2 = vertexes[i+1][j+1];
                }
            } else if ((i < row-1 && maze[i][j] == '#' && maze[i+1][j] == ' ') || i == row-1) {
                if(i > 0 && maze[i-1][j] == '#'){
                    if(vertexes[i][j] != 0)
                        v3 = vertexes[i][j];
                    else
                        v3 = vertexes[i+1][j];
                    if(vertexes[i][j+1] != 0)
                        v4 = vertexes[i][j+1];
                    else
                        v4 = vertexes[i+1][j+1];
                    // Print Triangles
                    vIdx.push_back(v1); vIdx.push_back(v2); vIdx.push_back(v4);
                    vIdx.push_back(v1); vIdx.push_back(v3); vIdx.push_back(v4);
                    // Reset indexes
                    v1 = -1;
                    v2 = -1;
                    v3 = -1;
                    v4 = -1;
                }
            }
        }
    }
}
