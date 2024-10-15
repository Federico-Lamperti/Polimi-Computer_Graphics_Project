#include "Starter.hpp"
#include "Constants.hpp"

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)

struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
};

struct OverlayUniformBlock {
    alignas(4) float visible;
};

// The vertices data structures
struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};

struct VertexOverlay {
    glm::vec2 pos;
    glm::vec2 UV;
};


class MeshLoader;
void GameLogic(MeshLoader* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World, float deltaT, glm::vec3 m, glm::vec3 r, bool fire, glm::vec3 c);
void createSphereMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, float radius);


class MeshLoader : public BaseProject {
protected:

    // Current aspect ratio (used by the callback that resized the window
    float Ar;

    // Descriptor Layouts ["classes" of what will be passed to the shaders]
    DescriptorSetLayout DSLspaceship, DSLspace, DSLobjects, DSLoverlay;

    // Vertex formats
    VertexDescriptor VDspaceship, VDspace, VDobjects, VDoverlay;

    // Pipelines [Shader couples]
    Pipeline Pspaceship, Pspace, Pplanets, Psun, Poverlay;
    
    // Models
    Model<Vertex> Mspaceship, Mspace, Msun, M1, M2, M3, M4, M5, M6;
    Model<VertexOverlay> Moverlay;
    
    // Descriptor sets
    DescriptorSet DSspaceship, DSspace, DSsun, DS1, DS2, DS3, DS4, DS5, DS6, DSoverlay;
    
    // Textures
    Texture Tspaceship, Tspace, Tsun, T1, T2, T3, T4, T5, T6, Toverlay;

    // Other application parameters
    int startScreen = 1;

    // Main application parameters
    void setWindowParameters() {
        // window size, titile and initial background
        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "Spaceship Simulator";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

        // Descriptor pool sizes
        uniformBlocksInPool = 19;
        texturesInPool = 10;
        setsInPool = 15;

        Ar = 4.0f / 3.0f;
    }

    // Window changes size
    void onWindowResize(int w, int h) {
        Ar = (float)w / (float)h;
    }

    // Load and setup all Vulkan Models and Textures, create DSL and load shaders
    void localInit() {
        // Descriptor Layouts [what will be passed to the shaders]
        DSLspaceship.init(this, {
            // this array contains the bindings:
            // first  element : the binding number
            // second element : the type of element (buffer or texture)
            //                  using the corresponding Vulkan constant
            // third  element : the pipeline stage where it will be used
            //                  using the corresponding Vulkan constant
                    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                    {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                    {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
            });
        
        DSLspace.init(this, {
                    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                    {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                    {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
            });
        
        DSLobjects.init(this, {
                    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                    {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                    {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
            });

        DSLoverlay.init(this, {
                    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                    {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
            });

        // Vertex descriptors
        VDspaceship.init(this, {
            // this array contains the bindings
            // first  element : the binding number
            // second element : the stride of this binging
            // third  element : whether this parameter change per vertex or per instance
            //                  using the corresponding Vulkan constant
            {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
            }, {
                // this array contains the location
                // first  element : the binding number
                // second element : the location number
                // third  element : the offset of this element in the memory record
                // fourth element : the data type of the element
                //                  using the corresponding Vulkan constant
                // fifth  elmenet : the size in byte of the element
                // sixth  element : a constant defining the element usage
                //                   POSITION - a vec3 with the position
                //                   NORMAL   - a vec3 with the normal vector
                //                   UV       - a vec2 with a UV coordinate
                //                   OTHER    - else
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                       sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
                       sizeof(glm::vec3), NORMAL},
                {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
                       sizeof(glm::vec2), UV}
            });
        
        VDspace.init(this, {
                {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
            }, {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                       sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
                       sizeof(glm::vec3), NORMAL},
                {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
                       sizeof(glm::vec2), UV}
            });
        
        VDobjects.init(this, {
                {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
            }, {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                       sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
                       sizeof(glm::vec3), NORMAL},
                {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
                       sizeof(glm::vec2), UV}
            });
        
        VDoverlay.init(this, {
                {0, sizeof(VertexOverlay), VK_VERTEX_INPUT_RATE_VERTEX}
            }, {
                {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, pos),
                      sizeof(glm::vec2), OTHER},
                {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, UV),
                      sizeof(glm::vec2), UV}
            });

        // Pipelines [Shader couples]
        // The second parameter is the pointer to the vertex definition
        // Third and fourth parameters are respectively the vertex and fragment shaders
        // The last array, is a vector of pointer to the layouts of the sets that will
        // be used in this pipeline
        Pspaceship.init(this, &VDspaceship, "shaders/XwingVert.spv", "shaders/XwingFrag.spv", { &DSLspaceship });
        
        Pspace.init(this, &VDspace, "shaders/SkyBoxVert.spv", "shaders/SkyBoxFrag.spv", { &DSLspace });
        Pspace.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT, false);
        
        Pplanets.init(this, &VDobjects, "shaders/PlanetsVert.spv", "shaders/PlanetsFrag.spv", { &DSLobjects });
        Pplanets.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_NONE, false);

        Psun.init(this, &VDobjects, "shaders/SunVert.spv", "shaders/SunFrag.spv", { &DSLobjects });
        Psun.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_NONE, false);

        Poverlay.init(this, &VDoverlay, "shaders/OverlayVert.spv", "shaders/OverlayFrag.spv", { &DSLoverlay });
        Poverlay.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_NONE, false);

        // Create models
        // The second parameter is the pointer to the vertex definition for this model
        // The third parameter is the file name
        // The last is a constant specifying the file type: currently only OBJ or GLTF
        Mspaceship.init(this, &VDspaceship, "models/X-WING-baker.obj", OBJ);
        Mspace.init(this, &VDspace, "models/SkyBoxCube.obj", OBJ);

        createSphereMesh(Msun.vertices, Msun.indices, scaleFactor * 10.0f);
        Msun.initMesh(this, &VDobjects);

        createSphereMesh(M1.vertices, M1.indices, scaleFactor * 0.1f);
        M1.initMesh(this, &VDobjects);

        createSphereMesh(M2.vertices, M2.indices, scaleFactor * 0.5f);
        M2.initMesh(this, &VDobjects);

        createSphereMesh(M3.vertices, M3.indices, scaleFactor * 1.0f);
        M3.initMesh(this, &VDobjects);

        createSphereMesh(M4.vertices, M4.indices, scaleFactor * 1.0f);
        M4.initMesh(this, &VDobjects);

        createSphereMesh(M5.vertices, M5.indices, scaleFactor * 0.9f);
        M5.initMesh(this, &VDobjects);

        createSphereMesh(M6.vertices, M6.indices, scaleFactor * 0.1f);
        M6.initMesh(this, &VDobjects);

        // Creates a mesh with direct enumeration of vertices and indices
        Moverlay.vertices = { {{-1.0f, -1.0f}, {0.0f, 0.0f}}, {{-1.0f, 1.0f}, {0.0f, 1.0f}},
                            {{ 1.0f, -1.0f}, {1.0f, 0.0f}}, {{ 1.0f, 1.0f}, {1.0f, 1.0f}} };
        Moverlay.indices = { 0, 1, 2,    1, 2, 3 };
        Moverlay.initMesh(this, &VDoverlay);

        // Create the textures
        Tspaceship.init(this, "textures/XwingColors.png");
        const char* T2fn[] = { "textures/sky/bkg1_right.png", "textures/sky/bkg1_left.png",
                              "textures/sky/bkg1_top.png",   "textures/sky/bkg1_bot.png",
                              "textures/sky/bkg1_front.png", "textures/sky/bkg1_back.png" };
        Tspace.initCubic(this, T2fn);

        Tsun.init(this, "textures/sun.png");

        T1.init(this, "textures/moon.png");
        T2.init(this, "textures/mars.jpg");
        T3.init(this, "textures/earth.png");
        T4.init(this, "textures/jupiter.png");
        T5.init(this, "textures/venus.png");
        T6.init(this, "textures/Asteroid.png");

        Toverlay.init(this, "textures/SplashScreen.png");
    }

    // Creation of pipelines and Descriptor Sets
    void pipelinesAndDescriptorSetsInit() {
        // This creates a new pipeline
        Pspaceship.create();
        Pspace.create();
        Psun.create();
        Pplanets.create();
        Poverlay.create();

        // Here you define the data set
        DSspaceship.init(this, &DSLspaceship, {
            // the second parameter, is a pointer to the Uniform Set Layout of this set
            // the last parameter is an array, with one element per binding of the set.
            // first  elmenet : the binding number
            // second element : UNIFORM or TEXTURE (an enum) depending on the type
            // third  element : only for UNIFORMs, the size of the corresponding C++ object. For texture, just put 0
            // fourth element : only for TEXTUREs, the pointer to the corresponding texture object. For uniforms, use nullptr
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &Tspaceship},
                    {2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
            });
        DSspace.init(this, &DSLspace, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &Tspace},
                    {2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
            });

        DSsun.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &Tsun}
            });

        DS1.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T1}
                    });

        DS2.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T2}
            });

        DS3.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T3}
            });

        DS4.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T4}
            });

        DS5.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T5}
            });

        DS6.init(this, &DSLobjects, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
                    {2, TEXTURE, 0, &T6}
            });

        DSoverlay.init(this, &DSLoverlay, {
                    {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                    {1, TEXTURE, 0, &Toverlay}
            });
    }

    // Destroy pipelines and Descriptor Sets
    void pipelinesAndDescriptorSetsCleanup() {
        // Cleanup pipelines
        Pspaceship.cleanup();
        Pspace.cleanup();
        Psun.cleanup();
        Pplanets.cleanup();
        Poverlay.cleanup();

        // Cleanup descriptor sets
        DSspaceship.cleanup();
        DSspace.cleanup();
        DSsun.cleanup();
        DS1.cleanup();
        DS2.cleanup();
        DS3.cleanup();
        DS4.cleanup();
        DS5.cleanup();
        DS6.cleanup();
        DSoverlay.cleanup();
    }

    // Destroy all the Models, Texture and DSL
    void localCleanup() {
        // Cleanup textures
        Tspaceship.cleanup();
        Tspace.cleanup();
        Tsun.cleanup();
        T1.cleanup();
        T2.cleanup();
        T3.cleanup();
        T4.cleanup();
        T5.cleanup();
        T6.cleanup();
        Toverlay.cleanup();

        // Cleanup models
        Mspaceship.cleanup();
        Mspace.cleanup();
        Msun.cleanup();
        M1.cleanup();
        M2.cleanup();
        M3.cleanup();
        M4.cleanup();
        M5.cleanup();
        M6.cleanup();
        Moverlay.cleanup();

        // Cleanup descriptor set layouts
        DSLspaceship.cleanup();
        DSLspace.cleanup();
        DSLobjects.cleanup();
        DSLoverlay.cleanup();

        // Destroies the pipelines
        Pspaceship.destroy();
        Pspace.destroy();
        Psun.destroy();
        Pplanets.destroy();
        Poverlay.destroy();
    }

    // Command buffer creation
    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
        switch (startScreen) {
            case 0: {
                // binds the pipeline
                Pspaceship.bind(commandBuffer);
                // For a pipeline object, this command binds the corresponing pipeline to the command buffer passed in its parameter

                // binds the descriptor set
                DSspaceship.bind(commandBuffer, Pspaceship, 0, currentImage);
                // For a Dataset object, this command binds the corresponing dataset
                // to the command buffer and pipeline passed in its first and second parameters.
                // The third parameter is the number of the set being bound
                // As described in the Vulkan tutorial, a different dataset is required for each image in the swap chain.
                // This is done automatically in file Starter.hpp, however the command here needs also the index
                // of the current image in the swap chain, passed in its last parameter

                // binds the model
                Mspaceship.bind(commandBuffer);
                // For a Model object, this command binds the corresponing index and vertex buffer
                // to the command buffer passed in its parameter

                // record the drawing command in the command buffer
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(Mspaceship.indices.size()), 1, 0, 0, 0);
                // the second parameter is the number of indexes to be drawn. For a Model object,
                // this can be retrieved with the .indices.size() method.

                Pspace.bind(commandBuffer);
                DSspace.bind(commandBuffer, Pspace, 0, currentImage);
                Mspace.bind(commandBuffer);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(Mspace.indices.size()), 1, 0, 0, 0);

                Psun.bind(commandBuffer);
                Msun.bind(commandBuffer);
                DSsun.bind(commandBuffer, Psun, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(Msun.indices.size()), 1, 0, 0, 0);

                Pplanets.bind(commandBuffer);
                M1.bind(commandBuffer);
                DS1.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);
                M2.bind(commandBuffer);
                DS2.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
                M3.bind(commandBuffer);
                DS3.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M3.indices.size()), 1, 0, 0, 0);
                M4.bind(commandBuffer);
                DS4.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M4.indices.size()), 1, 0, 0, 0);
                M5.bind(commandBuffer);
                DS5.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M5.indices.size()), 1, 0, 0, 0);
                M6.bind(commandBuffer);
                DS6.bind(commandBuffer, Pplanets, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M6.indices.size()), 1, 0, 0, 0);
                break;
            }

            case 1: {
                Poverlay.bind(commandBuffer);
                Moverlay.bind(commandBuffer);
                DSoverlay.bind(commandBuffer, Poverlay, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(Moverlay.indices.size()), 1, 0, 0, 0);
                
                break;
            }
        }
    }

    // Update the uniforms
    void updateUniformBuffer(uint32_t currentImage) {
        
        // Press ESC to quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        switch (startScreen) {
            case 0: {
                float deltaT;
                glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
                bool fire = false;
                getSixAxis(deltaT, m, r, fire);

                glm::mat4 ViewPrj;
                glm::mat4 WM;
                glm::vec3 cameraPos = glm::vec3(0, 0, 0);

                GameLogic(this, Ar, ViewPrj, WM, deltaT, m, r, fire, cameraPos);

                UniformBufferObject ubo{};
                GlobalUniformBufferObject gubo{};

                gubo.lightDir = glm::normalize(sunPos);
                gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                gubo.eyePos = cameraPos;

                ubo.mMat = WM * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DSspaceship.map(currentImage, &ubo, sizeof(ubo), 0);
                DSspaceship.map(currentImage, &gubo, sizeof(gubo), 2);
                // the .map() method of a DataSet object, requires the current image of the swap chain as first parameter
                // the second parameter is the pointer to the C++ data structure to transfer to the GPU
                // the third parameter is its size
                // the fourth parameter is the location inside the descriptor set of this uniform block

                ubo.mMat = glm::scale(glm::mat4(1), glm::vec3(WORLD_SIZE));
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DSspace.map(currentImage, &ubo, sizeof(ubo), 0);
                DSspace.map(currentImage, &gubo, sizeof(gubo), 2);

                // Sun
                ubo.mMat = glm::translate(glm::mat4(1.0f), sunPos);  // Change sunPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DSsun.map(currentImage, &ubo, sizeof(ubo), 0);
                DSsun.map(currentImage, &gubo, sizeof(gubo), 1);

                // Moon
                ubo.mMat = glm::translate(glm::mat4(1.0f), moonPos);  // Change moonPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS1.map(currentImage, &ubo, sizeof(ubo), 0);
                DS1.map(currentImage, &gubo, sizeof(gubo), 1);
                // Mars
                ubo.mMat = glm::translate(glm::mat4(1.0f), marsPos);  // Change marsPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS2.map(currentImage, &ubo, sizeof(ubo), 0);
                DS2.map(currentImage, &gubo, sizeof(gubo), 1);
                // Earth
                ubo.mMat = glm::translate(glm::mat4(1.0f), earthPos);  // Change earthPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS3.map(currentImage, &ubo, sizeof(ubo), 0);
                DS3.map(currentImage, &gubo, sizeof(gubo), 1);
                // Jupiter
                ubo.mMat = glm::translate(glm::mat4(1.0f), jupiterPos);  // Change jupiterPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS4.map(currentImage, &ubo, sizeof(ubo), 0);
                DS4.map(currentImage, &gubo, sizeof(gubo), 1);
                // Venus
                ubo.mMat = glm::translate(glm::mat4(1.0f), venusPos);  // Change venusPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS5.map(currentImage, &ubo, sizeof(ubo), 0);
                DS5.map(currentImage, &gubo, sizeof(gubo), 1);
                // Asteroid
                ubo.mMat = glm::translate(glm::mat4(1.0f), asteroidPos);  // Change asteroidPos to change pos in space
                ubo.mvpMat = ViewPrj * ubo.mMat;
                ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
                DS6.map(currentImage, &ubo, sizeof(ubo), 0);
                DS6.map(currentImage, &gubo, sizeof(gubo), 1);
                break;
            }
            case 1: {

                static bool debounce = false;
                static int curDebounce = 0;

                if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                    if (!debounce) {
                        debounce = true;
                        curDebounce = GLFW_KEY_SPACE;
                        startScreen = 0;
                        std::cout << "Game Started\n";
                        RebuildPipeline();
                    }
                }
                else {
                    if ((curDebounce == GLFW_KEY_SPACE) && debounce) {
                        debounce = false;
                        curDebounce = 0;
                    }
                }
                OverlayUniformBlock ubo;
                ubo.visible = 1;
                DSoverlay.map(currentImage, &ubo, sizeof(ubo), 0);
            }
        }
    }
};

#include "Logic.hpp"
#include "planetGen.hpp"

int main() {
    MeshLoader app;
    
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
