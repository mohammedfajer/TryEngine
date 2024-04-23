

// ===================================== WINDOW ==================================
struct Window {
    SDL_Window *display_window;
};

internal void 
window_setup(Window *window)
{
    // Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::string msg = "Failed to initialize SDL2: " + std::string(SDL_GetError());
        SM_ERROR(msg.c_str());
        
	}
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	// Create an SDL window
	window->display_window = SDL_CreateWindow("SDL2 OpenGL", SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT,
                                              SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	if (!window->display_window) 
    {
        std::string msg = "Failed to create SDL window: " + std::string(SDL_GetError());
		SDL_Quit();
        SM_ERROR(msg.c_str());
	}
}

internal void
window_framebuffer_size_callback(SDL_Window *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// ====================================== GAME ==================================

// Global State ....

struct Simulation_Game {
    
    Window *window;
    SDL_GLContext gl_context;
    
    // Timer here maybe ?
    const char *glsl_version;
    
    SDL_Event event;
};


internal void
game_setup(Simulation_Game *game)
{
    SDL_Window *window = game->window->display_window;
    
    SM_ASSERT(window != nullptr, "Window is null, we can't proceed");
    
	game->gl_context = SDL_GL_CreateContext(window);
    
	if (!game->gl_context) 
    {
        std::string msg = "Failed to create OpenGL context: " + std::string(SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		SM_ERROR(msg.c_str());
	}
    
    SDL_GL_MakeCurrent(window, game->gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    
	// Load OpenGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        
        std::string msg = "Failed to initialize glad!";
        
        SDL_GL_DeleteContext(game->gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        SM_ERROR(msg.c_str());
	}
    
    
    game->glsl_version = "#version 130";
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
    
     glEnable(GL_MULTISAMPLE);  
   
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    
    
    SM_TRACE("Finished Setting Up The Game...");
    
}

internal void
game_cleanup(Simulation_Game *game)
{
    SDL_GL_DeleteContext(game->gl_context);
    SDL_DestroyWindow(game->window->display_window);
    SDL_Quit();
}

extern bool quit;

internal void scene_state_manager_register_mouse_motion(SDL_Window *window, real32 x, real32 y);
internal void scene_state_manager_register_mouse_wheel(SDL_Window *window, real32 x, real32 y);


internal void
game_input(Simulation_Game *game)
{
    SDL_Window *window = game->window->display_window;
    
    SM_ASSERT(window != nullptr, "Window is null");
    
    // Event handling
    
    while (SDL_PollEvent(&game->event)) {
        // Dear ImGUi
        ImGui_ImplSDL2_ProcessEvent(&game->event);
        if (game->event.type == SDL_QUIT) {
            quit = true;
        }
        // Call mouse and scroll callbacks for relevant events
        else if (game->event.type == SDL_MOUSEMOTION) {
            
            
            scene_state_manager_register_mouse_motion(window, game->event.motion.x, game->event.motion.y);
            
        }
        else if (game->event.type == SDL_MOUSEWHEEL) {
            
            
            scene_state_manager_register_mouse_wheel(window, game->event.wheel.x, game->event.wheel.y);
        }
        
        
    }
    
    
}

internal void
game_update(Simulation_Game *game)
{
}

internal void
game_render(Simulation_Game *game)
{
}


// ========================================== DEAR IMGUI ==================================

// This is the main window we dock into.
internal 
void imgui_setup_docspace()
{
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

internal void
imgui_start_render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    
    //Start the Dear ImGui Frame
    ImGui::NewFrame();
    
    
}

internal void 
imgui_end_render() {
    ImGui::Render();
}


internal 
void imgui_setup(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version)
{
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
    
}

internal 
void imgui_render()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // Update and Render additional Platform Windows
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

internal
void imgui_cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
