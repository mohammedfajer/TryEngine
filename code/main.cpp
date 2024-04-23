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
#include "vectors.cpp"
#include "matrices.cpp"
#include "os_services.cpp"
#include "shaders.cpp"
#include "mesh.cpp"
#include "model.cpp"
#include "camera.cpp"
#include "array.cpp"
#include "rotate.cpp"
#include "framebuffer.cpp"
#include "engine.cpp"
#include "utilities.cpp"
#include "game.cpp"

// timing
global_variable float deltaTime = 0.0f;
global_variable float lastFrame = 0.0f;

global_variable bool quit = false;
global_variable Engine_Mode g_mode = Editor;
global_variable Window g_window;
global_variable Simulation_Game g_game;


int main(int argc, char* argv[]) {
    
    window_setup(&g_window);
    SDL_Window *window = g_window.display_window;
    
    g_game.window = &g_window;
    game_setup(&g_game);
    
    Array<IEngineScene*, 6> scenes;
    
    Scene_Test test_scene;
    Lighting_Scene lighting_scene;
    Lighting_Maps_Scene lighting_maps_scene;
    Light_Casters_Directional_Light_Scene light_casters_directional_light_scene; 
    Light_Casters_Point_Light_Scene light_casters_point_light_scene;
    Light_Casters_Spot_Light_Scene light_casters_spot_light_scene;
    
    scenes.add(&test_scene);
    scenes.add(&lighting_scene);
    scenes.add(&lighting_maps_scene);
    scenes.add(&light_casters_directional_light_scene);
    scenes.add(&light_casters_point_light_scene);
    scenes.add(&light_casters_spot_light_scene);
    
    SM_TRACE("Game Setup Done...");
    
    imgui_setup(window, g_game.gl_context, g_game.glsl_version);
    
    window_framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
    
    scene_state_manager_set(&test_scene);
    
    // Show the window when ready
    SDL_ShowWindow(window);
    //SDL_SetRelativeMouseMode( SDL_TRUE );

    glEnable(GL_DEPTH_TEST);
    
    // Main loop
    while (!quit) {
        float currentFrame = static_cast<float>(SDL_GetTicks() * 0.0025F);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Event handling
        game_input(&g_game);
        
        scene_state_manager_update(deltaTime);
        
        scene_switch(scenes);
        
        scene_state_manager_render();
        
        imgui_render();
        
    	// Swap buffers
    	SDL_GL_SwapWindow(window);
        
        // Delay to regulate frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }
    
    // Clean up
    
    imgui_cleanup();
    
    game_cleanup(&g_game);
    
    return 0;
}

