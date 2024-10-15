#define M_PI 3.141595f

// Main constants
const int WORLD_SIZE = 100000;
const int scaleFactor = 200;

// Planets positions
glm::vec3 sunPos = glm::vec3(scaleFactor * 0.0f, scaleFactor * 0.0f, scaleFactor * (-200.0f));
glm::vec3 moonPos = glm::vec3(scaleFactor * 2.5f, scaleFactor * (-1.5f), scaleFactor * (-60.0f));
glm::vec3 marsPos = glm::vec3(scaleFactor * (-2.0f), scaleFactor * (0.0f), scaleFactor * (-20.0f));
glm::vec3 earthPos = glm::vec3(scaleFactor * 5.0f, scaleFactor * 0.0f, scaleFactor *  (-60.0f));
glm::vec3 jupiterPos = glm::vec3(scaleFactor * 14.0f, scaleFactor * 0.0f, scaleFactor * 20.0f);
glm::vec3 venusPos = glm::vec3(scaleFactor * (-40.0f), scaleFactor * 0.0f, scaleFactor * (-100.0f));
glm::vec3 asteroidPos = glm::vec3(scaleFactor * 4.0f, scaleFactor * 0.0f, scaleFactor * 12.0f);


// Logic constants
const float FOV_Y = glm::radians(45.0f);
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = WORLD_SIZE;

const glm::vec3 STARTING_POS = glm::vec3(0.0, 0.0, 0.0);
const float MAX_SPEED = 250.0f;
const float MIN_SPEED = 50.0f;
const float ACCELERATION = 100.0f;
const float ROT_SPEED = glm::radians(100.0f);

const int LAMBDA = 10;

const float CAM_DIST = 10;
