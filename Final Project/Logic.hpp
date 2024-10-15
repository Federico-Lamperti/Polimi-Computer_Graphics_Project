static glm::vec3 Pos = STARTING_POS;
static float move_speed = MIN_SPEED;
static float Yaw = 0.0f;
static float oldYaw = 0.0f;
static float Pitch = 0.0f;
static float oldPitch = 0.0f;
static float Roll = 0.0f;
static float oldRoll = 0.0f;
static glm::quat rot = glm::quat(0, 0, 0, 1);


void ThirdPerson(MeshLoader* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World, float deltaT, glm::vec3 m, glm::vec3 r, glm::vec3 c) {

    // Update movement speed
    move_speed = move_speed + m.z * ACCELERATION * deltaT;
    if (move_speed >= MAX_SPEED)
        move_speed = MAX_SPEED;
    else if (move_speed <= MIN_SPEED)
        move_speed = MIN_SPEED;

    // Calculate rotation (with damping)
    Yaw = -ROT_SPEED * r.y * deltaT;
    Pitch = -ROT_SPEED * r.x * deltaT;
    Roll = -ROT_SPEED * m.x * deltaT;

    Yaw = oldYaw * exp(-1 * LAMBDA * deltaT) + Yaw * (1 - exp(-1 * LAMBDA * deltaT));
    oldYaw = Yaw;

    Pitch = oldPitch * exp(-1 * LAMBDA * deltaT) + Pitch * (1 - exp(-1 * LAMBDA * deltaT));
    oldPitch = Pitch;

    Roll = oldRoll * exp(-1 * LAMBDA * deltaT) + Roll * (1 - exp(-1 * LAMBDA * deltaT));
    oldRoll = Roll;

    glm::quat quatPitch = glm::rotate(glm::mat4(1), Pitch, glm::vec3(1, 0, 0));
    glm::quat quatYaw = glm::rotate(glm::mat4(1), Yaw, glm::vec3(0, 1, 0));
    glm::quat quatRoll = glm::rotate(glm::mat4(1), Roll, glm::vec3(0, 0, 1));
    rot = rot * quatYaw * quatPitch * quatRoll;

    // Update world matrix
    glm::vec3 dir = glm::vec3(glm::mat4(rot) * glm::vec4(0, 0, 1, 1));
    Pos -= dir * move_speed * deltaT;

    World = glm::translate(glm::mat4(1), Pos) * glm::mat4_cast(rot);

    // Calculate view matrix (with damping)
    c = World * glm::vec4(0, 0, CAM_DIST, 1);
    glm::vec3 a = World * glm::vec4(0, 0, 0, 1);
    glm::vec3 up = glm::vec3(glm::mat4(rot) * glm::vec4(0, 1, 0, 1));

    static glm::vec3 oldC = glm::vec3(0, 0, 0);
    c = oldC * exp(-1 * LAMBDA * deltaT) + c * (1 - exp(-1 * LAMBDA * deltaT));
    oldC = c;

    static glm::vec3 oldA = glm::vec3(0, 0, 0);
    a = oldA * exp(-1 * (LAMBDA - 2) * deltaT) + a * (1 - exp(-1 * (LAMBDA - 2) * deltaT));
    oldA = a;

    static glm::vec3 oldUp = glm::vec3(0, 1, 0);
    up = oldUp * exp(-1 * LAMBDA * deltaT) + up * (1 - exp(-1 * LAMBDA * deltaT));
    oldUp = up;

    glm::mat4 Mv = glm::lookAt(c, a, up);

    // Update Projection matrix
    glm::mat4 Mp = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    Mp[1][1] *= -1;

    ViewPrj = Mp * Mv;
}


void FirstPerson(MeshLoader* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World, float deltaT, glm::vec3 m, glm::vec3 r, glm::vec3 c) {
    
    // Update movement speed
    move_speed = move_speed + m.z * ACCELERATION * deltaT;
    if (move_speed >= MAX_SPEED)
        move_speed = MAX_SPEED;
    else if (move_speed <= MIN_SPEED)
        move_speed = MIN_SPEED;

    // Calculate rotation (with damping)
    Yaw = -ROT_SPEED * r.y * deltaT;
    Pitch = -ROT_SPEED * r.x * deltaT;
    Roll = -ROT_SPEED * m.x * deltaT;

    Yaw = oldYaw * exp(-1 * LAMBDA * deltaT) + Yaw * (1 - exp(-1 * LAMBDA * deltaT));
    oldYaw = Yaw;

    Pitch = oldPitch * exp(-1 * LAMBDA * deltaT) + Pitch * (1 - exp(-1 * LAMBDA * deltaT));
    oldPitch = Pitch;

    Roll = oldRoll * exp(-1 * LAMBDA * deltaT) + Roll * (1 - exp(-1 * LAMBDA * deltaT));
    oldRoll = Roll;

    glm::quat quatPitch = glm::rotate(glm::mat4(1), Pitch, glm::vec3(1, 0, 0));
    glm::quat quatYaw = glm::rotate(glm::mat4(1), Yaw, glm::vec3(0, 1, 0));
    glm::quat quatRoll = glm::rotate(glm::mat4(1), Roll, glm::vec3(0, 0, 1));
    rot = rot * quatYaw * quatPitch * quatRoll;

    // Calculate the view matrix
    glm::vec3 dir = glm::vec3(glm::mat4(rot) * glm::vec4(0, 0, -1, 1));
    Pos += dir * move_speed * deltaT;

    glm::vec3 a = Pos + dir;
    glm::vec3 up = glm::vec3(glm::mat4(rot) * glm::vec4(0, 1, 0, 1));
    c = Pos;

    glm::mat4 Mv = glm::lookAt(c, a, up);

    // Update World and Projection matrix
    World = glm::mat4(1);

    glm::mat4 Mp = glm::perspective(FOV_Y, Ar, NEAR_PLANE, FAR_PLANE);
    Mp[1][1] *= -1;

    ViewPrj = Mp * Mv;
}


void GameLogic(MeshLoader *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World, float deltaT, glm::vec3 m, glm::vec3 r, bool fire, glm::vec3 c) {

    // Press [SPACE] to change visual
    static int view = 0;
    static bool oldFire = false;
    if (fire && !oldFire) {
        view++;
        if (view == 2)
            view = 0;
    }
    oldFire = fire;

    switch (view) {
        case 0: ThirdPerson(A, Ar, ViewPrj, World, deltaT, m, r, c); break;
        case 1: FirstPerson(A, Ar, ViewPrj, World, deltaT, m, r, c); break;
    }
}
