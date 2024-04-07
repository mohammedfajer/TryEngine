

// Scene
// Scene Graph
// Scene Hierarchy
// Game Objects
// Components
// Entities
// Systems
// ECS


// Simulation_Game will contain a scene state machine 
/// 

// Texturing
// Light Models
// Depth and Transparency
// Models Loading
// Draw Plane
// 




struct Transform {
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};
	glm::mat4 model = glm::mat4(1.0f);
	Transform() {
		model = get_world_transform();
	}
	glm::mat4 get_world_transform() {
		glm::mat4 world;
		world = glm::translate(glm::mat4(1.0f), position) *
            glm::mat4_cast(orientation) *
            glm::scale(glm::mat4(1), scale);
		return world;
	}
	void update() {
		model = get_world_transform();
		orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}
};

struct Game_Object {
	Transform transform;
    std::string name;
};

struct Scene_Settings
{
    bool use_gimbal_lock;
    bool use_local;
    bool use_global;
    bool use_grid_plane;
    bool freeze_mouse_motion;
    glm::vec4 background_color;
};

class IEngineScene 
{
    public:
    virtual void init() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
    virtual void mouse_callback(SDL_Window *window, real32 x_pos_in, real32 y_pos_in) = 0;
    virtual void scroll_callback(SDL_Window *window, real32 x_offset, real32 y_offset) = 0;
};

extern Engine_Mode g_mode;

global_variable Scene_Settings g_settings = {};

internal void 
update_render_scene_settings()
{
    // Dear Imgui Build Up
    ImGui::Begin("Scene Settings");
    
    if (ImGui::Button("Gimbal Lock")) {
        g_settings.use_gimbal_lock = !g_settings.use_gimbal_lock; // Toggle state when button is clicked
    }
    ImGui::SameLine();
    ImGui::Text(g_settings.use_gimbal_lock ? "ON" : "OFF"); // Display current state
    
    if(ImGui::Button("Use Local Space")) g_settings.use_local = !g_settings.use_local;
    ImGui::SameLine();
    ImGui::Text(g_settings.use_local ? "ON" : "OFF");
    
    if(ImGui::Button("Use World Space")) g_settings.use_global = !g_settings.use_global;
    ImGui::SameLine();
    ImGui::Text(g_settings.use_global ? "ON" : "OFF");
    
    if(ImGui::Button("Show Grid Plane")) g_settings.use_grid_plane = !g_settings.use_grid_plane;
    ImGui::SameLine();
    ImGui::Text(g_settings.use_grid_plane ? "ON" : "OFF");
    
    if(ImGui::Button("Freeze Mouse Motion")) g_settings.freeze_mouse_motion = !g_settings.freeze_mouse_motion;
    ImGui::SameLine();
    ImGui::Text(g_settings.freeze_mouse_motion ? "ON" : "OFF");
    
    static float bgcolor[3] = { 24, 24, 24 }; // Initialize position
    
    ImGui::SliderFloat3("Background Color", bgcolor, 0, 255.0f);
    g_settings.background_color = glm::vec4(bgcolor[0]/255.0f, bgcolor[1]/255.0f, bgcolor[2]/255.0f, 1.0f);
    
    ImGui::End();
    
}

struct Scene_Test : public IEngineScene 
{
    Scene_Test() : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    backpack_model("../data/3dmodels/backpack.obj"),
    mesh_shader("../data/shaders/mesh.vs", "../data/shaders/mesh.fs"),
    plane_grid_shader("../data/shaders/plane_grid.vs", "../data/shaders/plane_grid.fs"),
    framebuffer(SCR_WIDTH, SCR_HEIGHT)
    {
        
    } 
    
    
    Camera camera ;
    
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    
    bool first_mouse = true;
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    
    Framebuffer framebuffer;
    Shader mesh_shader;
    Shader plane_grid_shader;
    
    
    Model backpack_model;
    glm::mat4 projection;
    glm::mat4 view;
    
    Game_Object backpack;
    
    // Update, Render
    virtual void init();
    virtual void update(float delta_time);
    virtual void render();
    virtual void cleanup();
    
    // this scene specific
    
    virtual void mouse_callback(SDL_Window *window, real32 x_pos_in, real32 y_pos_in);
    virtual void scroll_callback(SDL_Window *window, real32 x_offset, real32 y_offset);
    
};

/*
Scene_Test test_scene;
g_active_scene = &test_scene;

scene_state_manager_init();

// Game Loop
// Scene Controller
  if(...) scene_state_manager_switch(new_scene);


scene_state_manager_update(delta_time);
scene_state_manager_render();

///
scene_state_manager_cleanup();

*/


global_variable IEngineScene *g_active_scene = nullptr;



internal void
scene_state_manager_init() 
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    g_active_scene->init();
}

internal void 
scene_state_manager_set(IEngineScene *scene) { 
    SM_ASSERT(scene != nullptr, "No Active Scene Set");
    g_active_scene = scene;
    scene_state_manager_init();
}

internal void
scene_state_manager_update(float delta_time) 
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    g_active_scene->update(delta_time);
}

internal void
scene_state_manager_render() 
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    
    
    
    imgui_start_render();
    
    
    
    //Start the Dear ImGui Frame
    
    
    
    g_active_scene->render();
    
    update_render_scene_settings();
    
    imgui_end_render();
}

internal void
scene_state_manager_cleanup() 
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    g_active_scene->cleanup();
}

internal void
scene_state_manager_switch(IEngineScene *new_scene) 
{
    SM_ASSERT(new_scene != nullptr, "New Scene is not set");
    g_active_scene = new_scene;
}

internal void
scene_state_manager_register_mouse_motion(SDL_Window *window, real32 x, real32 y)
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    g_active_scene->mouse_callback(window, x, y);
}

internal void
scene_state_manager_register_mouse_wheel(SDL_Window *window, real32 x, real32 y)
{
    SM_ASSERT(g_active_scene != nullptr, "No Active Scene Set");
    g_active_scene->scroll_callback(window, x, y);
}


// ====================================== TEST SCENE ======================================

// Update, Render
void Scene_Test::init()
{
}

void Scene_Test::update(float delta_time)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SM_ASSERT(keys != NULL, "keys are null");
    
    if (keys[SDL_SCANCODE_ESCAPE])
        quit = true;
    
    if (keys[SDL_SCANCODE_W])
    {
        camera.ProcessKeyboard(FORWARD, delta_time);
        SM_TRACE("W Key was pressed");
    }
    
    if (keys[SDL_SCANCODE_S])
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (keys[SDL_SCANCODE_A])
        camera.ProcessKeyboard(LEFT, delta_time);
    if (keys[SDL_SCANCODE_D])
        camera.ProcessKeyboard(RIGHT, delta_time);
    if(keys[SDL_SCANCODE_E])
        camera.ProcessKeyboard(UP, delta_time);
    if(keys[SDL_SCANCODE_Q])
        camera.ProcessKeyboard(DOWN, delta_time);
    
    
    if(keys[SDL_SCANCODE_1] || g_settings.freeze_mouse_motion) g_mode = Game;
    if(keys[SDL_SCANCODE_2] ) g_mode = Editor;
    
    
}

void Scene_Test::render()
{
    // Rendering
    
    
    framebuffer.bind();
    //glClearColor(24/255.0, 24/255.0, 24/255.0, 1.0f);
    
    glClearColor(g_settings.background_color.x, g_settings.background_color.y, g_settings.background_color.z, g_settings.background_color.w);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // don't forget to enable shader before setting uniforms
    mesh_shader.use();
    
    // draw wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    
    
    mesh_shader.setMat4("projection", projection);
    mesh_shader.setMat4("view", view);
    
    // render the loaded model
    
    mesh_shader.setMat4("model", backpack.transform.model);
    backpack_model.Draw(mesh_shader);
    
    // Reset polygon mode to fill
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if(g_settings.use_grid_plane)
    {
        plane_grid_shader.use();
        plane_grid_shader.setMat4("view", camera.GetViewMatrix());
        plane_grid_shader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    
    framebuffer.unbind();
    framebuffer.draw();
    
    //Start the Dear ImGui Frame
    //ImGui::NewFrame();
    
    imgui_setup_docspace();
    static bool show_demo_window;
    
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
        
        Space space = (g_settings.use_local ? Space::Self : Space::World);
        
        if(!g_settings.use_gimbal_lock)
            Rotate(backpack.transform.orientation, rotation[0], rotation[1], rotation[2], space);
        else
            backpack.transform.orientation = Rotate_Euler(pitch, yaw, roll);
        
        backpack.transform.update();
        
        ImGui::End();
    }
    
    
    
    // Rendering Game
    
}

void Scene_Test::cleanup()
{
}

void Scene_Test::mouse_callback(SDL_Window *window, real32 x_pos_in, real32 y_pos_in)
{
    if(g_mode == Game) return;
    
    static float lastX = x_pos_in;
    static float lastY = y_pos_in;
    static bool firstMouse = true;
    
    float xpos = static_cast<float>(x_pos_in);
    float ypos = static_cast<float>(y_pos_in);
    
    if (firstMouse) {
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

void Scene_Test::scroll_callback(SDL_Window *window, real32 x_offset, real32 y_offset)
{
    camera.ProcessMouseScroll(static_cast<float>(y_offset));
}

