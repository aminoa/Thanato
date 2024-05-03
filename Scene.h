#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"
#include "TextBox.h"

struct GameState
{
    Map *map;
    Map *object_map;
    Entity *player;
    Entity *enemies;
    Entity *interactables;
    Entity* loading_zones;
    
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;

    std::string text_buffer;
    int next_scene_id;

    Entity* title_screen_text;
};

class Scene {
public:
    int m_number_of_enemies = 0;
    int m_number_of_interactables = 1;
    glm::vec3 m_player_start_position = glm::vec3(2.0f, -2.0f, 0.0f);
    int level_number = 1;

    int LEVEL_WIDTH;
    int LEVEL_HEIGHT;
    int LEVEL_LEFT_EDGE;

    GameState m_state;
    
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    GameState const get_state() const { return m_state; }
    
    void player_initialize(Entity* player);

};
