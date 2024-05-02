/**
* Author: Aneesh Maganti
* Assignment: Thanato
* Date due: 2024-05-03, 3:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

enum LevelIndex { TEST_LEVEL, HOME };

#define u8 Uint8
#define u32 Uint32

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define DIALOGUE_DELAY 1500 // 1 second

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "Effects.h"
#include "Textbox.h"

// Levels
#include "TestLevel.h"
#include "Home.h"

const char FONT_FILEPATH[] = "assets/all_8x8.png";
const char TEXTBOX_FILEPATH[] = "assets/textbox.png";

// ––––– CONSTANTS ––––– //
const int WINDOW_SCALE_FACTOR = 1; // TODO: change to 2 for the release
const int WINDOW_WIDTH  = 640 * WINDOW_SCALE_FACTOR,
          WINDOW_HEIGHT = 480 * WINDOW_SCALE_FACTOR;
//
const float BG_RED     = 0.0f,
            BG_BLUE    = 0.0f,
            BG_GREEN   = 0.0f,
            BG_OPACITY = 0.0f;
const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;
const float CAMERA_SCALE = 0.5f;

GLuint g_text_texture_id = Utility::load_texture("assets/all_8x8.png");

// Rendering
SDL_Window* g_display_window;
bool g_game_is_running = true;
ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

// Scenes
Scene* g_current_scene;
Scene* g_levels[10];
//TestLevel* g_levelOne;

//Effects *g_effects;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;
bool g_is_colliding_bottom = false;
u32 g_dialogue_timer = 0;

//text
Entity* g_text_texture;
//Entity* g_state_text;
TextBox* g_textbox;
bool g_in_text_state = false;

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene *scene)
{    
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Thanato",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Level Setup 
    g_levels[LevelIndex::TEST_LEVEL] = new TestLevel();
    g_levels[LevelIndex::HOME] = new Home();
    
    switch_to_scene(g_levels[LevelIndex::HOME]);
    
    //g_effects = new Effects(g_projection_matrix, g_view_matrix);
    // Special effect added
    //g_effects->start(FADEIN, 0.25f);

    //text
	//g_state_text = new Entity();
	//g_state_text->m_texture_id = Utility::load_texture(FONT_FILEPATH);
	//g_state_text->set_position(glm::vec3(0.0f, 1.0f, 0.0f));
    g_textbox = new TextBox();
    g_textbox->deactivate();
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						g_game_is_running = false;
						break;
                    // Fullscreen
                    case SDLK_F11:
                        if (SDL_GetWindowFlags(g_display_window) & SDL_WINDOW_FULLSCREEN)
                        {
							SDL_SetWindowFullscreen(g_display_window, 0);
						}
                        else
                        {
							SDL_SetWindowFullscreen(g_display_window, SDL_WINDOW_FULLSCREEN);
						}
						break;
                }

            default:
                break;
        }
    }

    const u8* key_state = SDL_GetKeyboardState(NULL);

    // Debug Map Switch
    if (key_state[SDL_SCANCODE_D] && key_state[SDL_SCANCODE_1]) switch_to_scene(g_levels[LevelIndex::TEST_LEVEL]);
    if (key_state[SDL_SCANCODE_D] && key_state[SDL_SCANCODE_2]) switch_to_scene(g_levels[LevelIndex::HOME]);

    if (g_current_scene->m_state.player->m_locked)
    {
        g_current_scene->m_state.player->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
        return;
    }

    if (key_state[SDL_SCANCODE_LEFT])
    {
        g_current_scene->m_state.player->set_velocity(glm::vec3(-1.0f, 0.0f, 0.0f));
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
    }
    else if (key_state[SDL_SCANCODE_RIGHT])
    {
        g_current_scene->m_state.player->set_velocity(glm::vec3(1.0f, 0.0f, 0.0f));
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
    }
    else if (key_state[SDL_SCANCODE_UP])
    {
        g_current_scene->m_state.player->set_velocity(glm::vec3(0.0f, 1.0f, 0.0f));
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->UP];
	}
    else if (key_state[SDL_SCANCODE_DOWN])
    {
        g_current_scene->m_state.player->set_velocity(glm::vec3(0.0f, -1.0f, 0.0f));
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->DOWN];
	} 
    else
    {
		g_current_scene->m_state.player->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}

    // Running
    if (key_state[SDL_SCANCODE_X]) 
    {
        g_current_scene->m_state.player->m_speed = 4.0f;
        g_current_scene->m_state.player->SECONDS_PER_FRAME = 8;
	}
    else
    {
		g_current_scene->m_state.player->m_speed = 2.0f;
        g_current_scene->m_state.player->SECONDS_PER_FRAME = 4;
    }

}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_shader_program.get_program_id());


    g_current_scene->render(&g_shader_program);

    if (g_textbox->m_is_active)
    {
		g_textbox->render_textbox(&g_shader_program);
		glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / 0.5) - 8, -7, 0);
		g_textbox->render_text(&g_shader_program, text_position);
	}
    
    SDL_GL_SwapWindow(g_display_window);
}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }


    while (delta_time >= FIXED_TIMESTEP) {
        g_current_scene->update(FIXED_TIMESTEP);
        //g_effects->update(FIXED_TIMESTEP);

        const u8* key_state = SDL_GetKeyboardState(NULL);

        if (g_current_scene->m_state.player->m_locked)
        {
            g_textbox->activate();
            int MAX_CHARACTER_COUNT = 33;
            // split text_buffer into vector of strings of max length MAX_CHARACTER_COUNT
            std::vector<std::string> text_lines;
            std::string text = g_current_scene->m_state.text_buffer;
            while (text.length() > MAX_CHARACTER_COUNT)
            {
				std::string line = text.substr(0, MAX_CHARACTER_COUNT);
				text_lines.push_back(line);
				text = text.substr(MAX_CHARACTER_COUNT);
			}

            // add remaining text to vector
            text_lines.push_back(text);
            
            //g_dialogue_timer = SDL_GetTicks();

            //g_textbox->activate();
            while (!text_lines.empty())
            {
                u8* key_state = (u8*) SDL_GetKeyboardState(NULL);
                u32 current_time = SDL_GetTicks();
				// draw one line at a time, wait for z input to draw next line (have a waiting period too)
                // want release of Z to draw a line

                while (key_state[SDL_SCANCODE_Z] && current_time - g_dialogue_timer > DIALOGUE_DELAY)
				{
                    g_dialogue_timer = current_time;
					g_current_scene->m_state.text_buffer = text_lines[0];

                    //g_textbox->update_text(text_lines[0], 0.5f, 0.0f);
                    g_textbox->update_text(text_lines[0], 0.5f, 0.0f);
					g_textbox->update_textbox(g_view_matrix);

					text_lines.erase(text_lines.begin());
                    
                    // sleep for 1 second
				}

                render();
				SDL_Delay(DIALOGUE_DELAY);
				key_state = (u8*) SDL_GetKeyboardState(NULL);
            }

            g_textbox->deactivate();
			g_current_scene->m_state.player->m_locked = false;
        }

        g_is_colliding_bottom = g_current_scene->m_state.player->m_collided_bottom;
        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;
    //g_textbox->update(delta_time, g_current_scene->m_state.player, g_current_scene->m_state.interactables, g_current_scene->m_number_of_interactables, g_current_scene->m_state.map);

    // Prevent the camera from showing anything outside of the "edge" of the level
    g_view_matrix = glm::mat4(1.0f);
    g_view_matrix = glm::scale(g_view_matrix, glm::vec3(CAMERA_SCALE, CAMERA_SCALE, 1.0f));

    // TODO: need more checks to keep camera in bounds 
    if (g_current_scene->m_state.player->get_position().x > g_current_scene->LEVEL_LEFT_EDGE) {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, 3.75, 0));
    }
    else {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
    }

    //g_view_matrix = glm::translate(g_view_matrix, g_effects->m_view_offset);
}


void shutdown()
{    
    SDL_Quit();
    for (Scene* level : g_levels) { delete level; }
    //delete g_effects;
}

// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_game_is_running)
    {
        process_input();
        update();
        
        if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);
        
        render();
    }
    
    shutdown();
    return 0;
}