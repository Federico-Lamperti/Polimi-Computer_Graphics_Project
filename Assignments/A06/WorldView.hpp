
glm::mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Rho, glm::vec3 Pos) {
	// Creates a view projection matrix, with near plane at 0.1, and far plane at 50.0, and
	// aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
	// vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
	// defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
	// elevation (Pitch), and <Rho> the roll.
	glm::mat4 M = glm::mat4(1);
    glm::mat4 P = glm::mat4(1);
    
    float n = 0.1;
    float f = 50.0;
    float fov_y = glm::radians(60.0f);
    P = glm::perspective(fov_y, Ar, n, f);
    P[1][1] *= -1;
    M = glm::rotate(M, -Rho, glm::vec3(0,0,1)) *
        glm::rotate(M, -Beta, glm::vec3(1,0,0)) *
        glm::rotate(M, -Alpha, glm::vec3(0,1,0)) *
        glm::translate(M, -Pos);
    M = P * M;
	return M;
}

glm::mat4 MakeWorldMatrix(glm::vec3 pos, glm::quat rQ, glm::vec3 size) {
	// creates and returns a World Matrix that positions the object at <pos>,
	// orients it according to <rQ>, and scales it according to the sizes
	// given in vector <size>
	glm::mat4 W = glm::mat4(1);
    glm::mat4 Q = glm::mat4(glm::normalize(rQ));
    W = glm::translate(W, pos) * Q * glm::scale(W, size);
	return W;
}
