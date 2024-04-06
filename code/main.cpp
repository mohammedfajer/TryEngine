// Glad
#include "glad.c"

#include <cassert>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Imgui
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "main.h"
#include "os_services.cpp"
#include "shaders.cpp"
#include "mesh.cpp"
#include "model.cpp"
#include "camera.cpp"
#include "array.cpp"


// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

void processInput(SDL_Window *window);
void framebuffer_size_callback(SDL_Window* window, int width, int height);
void mouse_callback(SDL_Window* window, double xposIn, double yposIn);
void scroll_callback(SDL_Window* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool quit = false;

enum Engine_Mode {
	Editor,
	Game
};



enum class Space { Self, World };

// Function to rotate an object's orientation
void Rotate(glm::quat& orientation, float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self) {
    // Convert the Euler angles to quaternions
    glm::quat xRot = glm::angleAxis(glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yRot = glm::angleAxis(glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat zRot = glm::angleAxis(glm::radians(zAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Combine the rotations based on the space
    glm::quat rotation;
    if (relativeTo == Space::World) {
        // Rotate relative to the world axes
        orientation = orientation * xRot;
        orientation = orientation * yRot;
        orientation = orientation * zRot;
    } else {
        // Rotate relative to the object's local axes
        orientation = xRot * orientation ;
        orientation = yRot * orientation ;
        orientation = zRot * orientation ;
    }
    orientation = glm::normalize(orientation);    
}

void Rotate(glm::quat &orientation, glm::vec3 euler_angles, Space relativeTo = Space::Self) {
	// Convert the Euler angles to quaternions
    glm::quat xRot = glm::angleAxis(glm::radians(euler_angles[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yRot = glm::angleAxis(glm::radians(euler_angles[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat zRot = glm::angleAxis(glm::radians(euler_angles[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Combine the rotations based on the space
    glm::quat rotation;
    if (relativeTo == Space::World) {
        // Rotate relative to the world axes
        orientation = orientation * xRot;
        orientation = orientation * yRot;
        orientation = orientation * zRot;
    } else {
        // Rotate relative to the object's local axes
        orientation = xRot * orientation ;
        orientation = yRot * orientation ;
        orientation = zRot * orientation ;
    }
    orientation = glm::normalize(orientation);    
}


struct Transform {
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	// glm::vec3 euler_rotation = {0.0f, 0.0f, 0.0f};
	glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    
	glm::mat4 model = glm::mat4(1.0f);
    
    
	Transform() {
		
        
		model = get_world_transform();
		
	}
    
	glm::mat4 get_world_transform()
	{
		glm::mat4 world;
		world = glm::translate(glm::mat4(1.0f), position) *
            glm::mat4_cast(orientation) *
            glm::scale(glm::mat4(1), scale);
		return world;
	}
    
	void update()
	{
		model = get_world_transform();
		orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
    
    
};

struct Game_Object {
    
	Transform transform;
    std::string name;
    
};

struct Numbers {
    int *items;
    size_t count;
    size_t capacity;
};



#define da_append(arr, item)\
do{\
if(arr.count >= arr.capacity) {\
if(arr.capacity == 0) arr.capacity = 256;\
else arr.capacity *= 2;\
arr.items = static_cast<decltype(arr.items)> (realloc(arr.items, arr.capacity *sizeof(arr.items)));\
}\
arr.items[arr.count++] = item;\
} while(0)


void use_dynamic_array() {
    Numbers nums = {0};
    
    da_append(nums, 1);
    da_append(nums, 2);
    
    for(size_t i=0; i<nums.count; i++) std::cout << nums.items[i] << std::endl;
}


global_variable Engine_Mode g_Mode = Editor;

#include "framebuffer.cpp"

// This is the main window we dock into.
internal void setup_docspace() {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus;
	bool open = true;
	bool *p_open = &open;
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	ImGui::PopStyleVar(2);
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGui::DockSpace(ImGui::GetID("Dockspace"));
	ImGui::End();
}

int main(int argc, char* argv[]) {
    
    
    
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << std::endl;
		return -1;
	}
    
	// Create an SDL window and OpenGL context
	SDL_Window* window = SDL_CreateWindow("SDL2 OpenGL", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	if (!window) {
		std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}
    
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    
	if (!gl_context) {
		std::cerr << "Failed to create OpenGL context: "
			<< SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}
    
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    
	// Load OpenGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize glad!" << std::endl;
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
	}
    
    
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    // Setup Dear ImGui Context
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //Setup Dear ImGui Style
    
    ImGui::StyleColorsDark();
    
    //Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    //Dear ImGui Example State
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    
    // build and compile shaders
    // -------------------------
    Shader ourShader("../data/shaders/mesh.vs", "../data/shaders/mesh.fs");
    Shader planeShader("../data/shaders/plane_grid.vs", "../data/shaders/plane_grid.fs");
    std::cout << "Done Shaders....\n";
    
    // load models
    // -----------
    //Model ourModel("../data/3dmodels/Wuson/WusonOBJ.obj");
    Model ourModel("../data/3dmodels/backpack.obj");
    std::cout << "Done Model...\n";
    
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
    
    Framebuffer framebuffer(SCR_WIDTH, SCR_HEIGHT);
    
    
    // Show the window when ready
    SDL_ShowWindow(window);
    
    // Main loop
    while (!quit) {
        float currentFrame = static_cast<float>(SDL_GetTicks() * 0.0025F);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Dear ImGUi
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Call mouse and scroll callbacks for relevant events
            else if (event.type == SDL_MOUSEMOTION) {
                mouse_callback(window, event.motion.x, event.motion.y);
            }
            else if (event.type == SDL_MOUSEWHEEL) {
                scroll_callback(window, event.wheel.x, event.wheel.y);
            }
        }
        
        processInput(window);
        
        // Rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        
        framebuffer.bind();
        glClearColor(24/255.0, 24/255.0, 24/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        
        // don't forget to enable shader before setting uniforms
        ourShader.use();
        
        // draw wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        // render the loaded model
        
        
        
        
        static Game_Object backpack;
        ourShader.setMat4("model", backpack.transform.model);
        ourModel.Draw(ourShader);
        
        // Reset polygon mode to fill
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        planeShader.use();
        planeShader.setMat4("view", camera.GetViewMatrix());
        planeShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        framebuffer.unbind();
        framebuffer.draw();
        
        //Start the Dear ImGui Frame
        ImGui::NewFrame();
        
        setup_docspace();
        ImGui::ShowDemoWindow(&show_demo_window);
        
        ImGui::Begin("Scene");
        
        // we access the ImGui window size
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;
        
        framebuffer.rescale(window_width, window_height);
        glViewport(0, 0, window_width, window_height);
        
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        ImGui::GetWindowDrawList()->AddImage(
                                             (void *)(intptr_t)framebuffer.get_texture_id(),
                                             ImVec2(pos.x, pos.y),
                                             ImVec2(pos.x + window_width, pos.y + window_height),
                                             ImVec2(0, 1),
                                             ImVec2(1, 0)
                                             );
        
        ImGui::End();
        
        {
            ImGui::Begin("Transform");
            
            
            static float position[3] = { 0.0f, 0.0f, 0.0f }; // Initialize position
            static float rotation[3] = { 0.0f, 0.0f, 0.0f }; // Initialize rotation
            static float scale[3] = { 1.0f, 1.0f, 1.0f }; // Initialize scale
            
            ImGui::SliderFloat3("Position", position, -100.0f, 100.0f);
            ImGui::SliderFloat3("Rotation", rotation, -180.0f, 180.0f);
            ImGui::SliderFloat3("Scale", scale, 0.1f, 10.0f);
            
            backpack.transform.position.x = position[0];
            backpack.transform.position.y = position[1];
            backpack.transform.position.z = position[2];
            
            float pitch = glm::radians(rotation[0]);
            float yaw = glm::radians(rotation[1]);
            float roll = glm::radians(rotation[2]);
            
            // glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
            // glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            // glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));
            
            
            // // combine rotation matrices in the order of Z, Y, X
            // glm::mat4 rotation_matrix = rotX * rotY * rotZ;
            // glm::quat quaternion = glm::quat_cast(rotation_matrix);
            //backpack.transform.orientation = quaternion;
            
            
            // This still has gimbal lock issue e.g. yaw = 90
            //backpack.transform.orientation = glm::quat(glm::vec3(pitch, yaw, roll));
            
            // Local Space
            // backpack.transform.orientation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f))
            // 															* glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f))
            // 															* glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
            
            // Global Space
            // glm::quat quatYaw = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            // glm::quat quatPitch = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
            // glm::quat quatRoll = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
            
            // glm::quat rotationQuaternion = quatYaw * quatPitch * quatRoll;
            
            // backpack.transform.orientation = rotationQuaternion;
            
            
            // Rotate(backpack.transform.orientation, rotation[0], rotation[1], rotation[2], Space::World);
            
            // backpack.transform.update();
            
            
            Rotate(backpack.transform.orientation, rotation[0], rotation[1], rotation[2], Space::Self);
            //Rotate(backpack.transform.orientation, glm::vec3(rotation[0],rotation[1], rotation[2]));
            
            
            
            backpack.transform.update();
            
            ImGui::End();
        }
        
        ImGui::Render();
        
        
		
        
        
        // Rendering Game
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        
    	// Swap buffers
    	SDL_GL_SwapWindow(window);
        
        // Delay to regulate frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }
    
    // Clean up
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

void processInput(SDL_Window *window)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_ESCAPE])
        quit = true;
    
    if (keys[SDL_SCANCODE_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[SDL_SCANCODE_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[SDL_SCANCODE_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[SDL_SCANCODE_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(keys[SDL_SCANCODE_E])
        camera.ProcessKeyboard(UP, deltaTime);
    if(keys[SDL_SCANCODE_Q])
        camera.ProcessKeyboard(DOWN, deltaTime);
    
    // toggle engine mode
    if(keys[SDL_SCANCODE_1]) g_Mode = Editor;
    if(keys[SDL_SCANCODE_2]) g_Mode = Game;
}

void framebuffer_size_callback(SDL_Window *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(SDL_Window *window, double xposIn, double yposIn)
{
	if(g_Mode == Game) return;
    static float lastX = xposIn;
    static float lastY = yposIn;
    static bool firstMouse = true;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(SDL_Window *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
