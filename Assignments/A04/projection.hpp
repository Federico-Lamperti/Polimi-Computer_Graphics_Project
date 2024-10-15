

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
    glm::mat4 shearZ;
    
    float w = 10;      //half-width
    float a = Ar;      //aspect ratio
    float n = -50;     //near plane
    float f = 50;      //far plane
    float l = -w;      //left
    float r = w;       //right
    float t = w/a;     //top
    float b = -w/a;    //bottom
	
	// Isometric projection
	S = glm::mat4(1);
    S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);
    S = glm::rotate(S, glm::radians(35.26f), glm::vec3(1,0,0));
    S = glm::rotate(S, glm::radians(-45.0f), glm::vec3(0,1,0));
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
	S = glm::mat4(1);
    S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);
    S = glm::rotate(S, glm::radians(25.0f), glm::vec3(1,0,0));
    S = glm::rotate(S, glm::radians(-45.0f), glm::vec3(0,1,0));
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
	S = glm::mat4(1);
    S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);
    S = glm::rotate(S, glm::radians(40.0f), glm::vec3(1,0,0));
    S = glm::rotate(S, glm::radians(-70.0f), glm::vec3(0,1,0));
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
    S = glm::mat4(1);
    S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);
    shearZ = glm::mat4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       -0.5*cos(glm::radians(45.0f)), -0.5*sin(glm::radians(45.0f)), 1, 0,
                       0, 0, 0, 1);
    S = S * shearZ;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}
