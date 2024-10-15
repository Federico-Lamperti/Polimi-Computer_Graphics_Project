 

void SetupProjectionMatrices(Assignment05 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
    
    //float w = 10;      //half-width
    float a = Ar;        //aspect ratio
    float n = 0.1;       //near plane
    float f = 50.0;      //far plane
    float fov_y;
    float l;       //left
    float r;       //right
    float t;       //top
    float b;       //bottom
	
	// Fov-y = 60 deg
	S = glm::mat4(1);
    fov_y = glm::radians(60.0f);
    S = glm::perspective(fov_y, a, n, f);
    S[1][1] *= -1;
    A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	S = glm::mat4(1);
    fov_y = glm::radians(105.0f);
    S = glm::perspective(fov_y, a, n, f);
    S[1][1] *= -1;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	S = glm::mat4(1);
    fov_y = glm::radians(25.0f);
    S = glm::perspective(fov_y, a, n, f);
    S[1][1] *= -1;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-y = 45 deg
	S = glm::mat4(1);
    fov_y = glm::radians(45.0f);
    float tan_y = tan(fov_y/2);
    //screen coordinates
    t = (1/a) * n * tan_y;
    b = (1/a) * -n * tan_y;
    l = 0;                      // left is the center
    r = 2 * a * n * tan_y;      // right is 2X
    S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::frustum(l, r, b, t, n, f);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}
