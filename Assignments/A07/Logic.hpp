
void GameLogic(Assignment07 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World) {
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
    
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;
	
	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);
	
	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);   // Omega
	const float MOVE_SPEED = 2.0f;                  // Mu
    
    // Added a damping factor
    const float damping = 10.0f;

	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assignment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	static bool fire = false;
    static bool oldFire = fire;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
	// Current Player Position - static variables make sure that the ri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;
    static glm::vec3 newPos = Pos;

	// To be done in the assignment
    // Variables to hold the angles
    static float pitch = 0;
    static float yaw = 0;
    static float roll = 0;
    
    static float camYaw = 0;
    
    // Switch control mode
    static int mode = 0;
    if (fire && !oldFire) {
        if (mode == 0)
            mode = 1;
        else
            mode = 0;
        switch (mode) {
            case 0: { printf("Moving Object!\n");
                yaw = camYaw;
                break;
            }
            case 1: { printf("Moving Camera!\n");
                camYaw = yaw;
                break;
            }
        }
    }
    
    switch (mode) {
        case 0 : {  // Moving Object
            pitch = glm::clamp(pitch - ROT_SPEED * r.x * deltaT, minPitch, maxPitch);
            yaw -= ROT_SPEED * r.y * deltaT;
            roll -= ROT_SPEED * r.z * deltaT;
            
            // Vectors ux, uy and uz, represents the unitary movement in each axis
            glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1));
            glm::vec3 uy = glm::vec3(0,1,0);
            glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1));
            
            newPos += ux * MOVE_SPEED * m.x * deltaT;
            newPos += uy * MOVE_SPEED * m.y * deltaT;
            newPos += uz * MOVE_SPEED * m.z * deltaT;
            
            // Don't go under the map
            if (newPos.y < 0.0f)
                newPos.y = 0.0f;
            
            // Update Pos applying the damping factor
            Pos = Pos * exp(-damping * deltaT) + newPos * (1 - exp(-damping * deltaT));
            
            World = glm::translate(glm::mat4(1.0), Pos) *
            glm::rotate(glm::mat4(1.0), yaw , glm::vec3(0,1,0)) *
            glm::rotate(glm::mat4(1.0), 0.0f , glm::vec3(1,0,0)) *
            glm::rotate(glm::mat4(1.0), 0.0f , glm::vec3(0,0,1)) *
            glm::scale(glm::mat4(1.0), glm::vec3(1));
            
            // Cam position
            glm::vec3 c = World * glm::vec4(0.0f, camHeight + camDist * sin(pitch), camDist * cos(pitch), 1.0f);
            // Object position
            glm::vec3 a = World * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) + glm::vec4(0.0f, camHeight, 0.0f, 0.0f);
            // Up-vector
            glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
            
            glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
            Mp[1][1] *= -1;
            ViewPrj = Mp * glm::rotate(glm::mat4(1), -roll, glm::vec3(0, 0, 1)) * glm::lookAt(c, a, u);
            break;
        }
        case 1 : {  // Moving Camera
            pitch = glm::clamp(pitch - ROT_SPEED * r.x * deltaT, minPitch, maxPitch);
            camYaw -= ROT_SPEED * r.y * deltaT;
            roll -= ROT_SPEED * r.z * deltaT;
            
            World = glm::translate(glm::mat4(1.0), Pos) *
            glm::rotate(glm::mat4(1.0), yaw , glm::vec3(0,1,0)) *
            glm::rotate(glm::mat4(1.0), 0.0f , glm::vec3(1,0,0)) *
            glm::rotate(glm::mat4(1.0), 0.0f , glm::vec3(0,0,1)) *
            glm::scale(glm::mat4(1.0), glm::vec3(1));
            
            // Cam position
            glm::vec3 c = World * glm::vec4(camDist * cos(pitch) * sin(yaw - camYaw), camHeight + camDist * sin(pitch), camDist * cos(pitch) * cos(yaw - camYaw), 1);
            // Object position
            glm::vec3 a = World * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) + glm::vec4(0.0f, camHeight, 0.0f, 0.0f);
            // Up-vector
            glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
            
            glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
            Mp[1][1] *= -1;
            ViewPrj = Mp * glm::rotate(glm::mat4(1), -roll, glm::vec3(0, 0, 1)) * glm::lookAt(c, a, u);
            break;
        }
    }
}
