#include "LevelOne.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

const float BG_RED = 0.4f,
BG_BLUE = 0.4f,
BG_GREEN = 1.0f,
BG_OPACITY = 0.0f;

unsigned int LEVELONE_DATA[] =
{
    48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    48, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    48, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

LevelOne::~LevelOne()
{
    delete [] m_state.enemies;
    delete m_state.player;
    delete m_state.map;
    //Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelOne::initialise()
{
    m_state.next_scene_id = -1;
    m_number_of_enemies = ENEMY_COUNT;
    
    GLuint map_texture_id = Utility::load_texture("assets/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELONE_DATA, map_texture_id, 1.0f, 12, 11);
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(m_player_start_position);
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->m_speed = 2.5f;
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/taekshi.png");
    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 4, 5, 6, 7 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 8, 9, 10, 11 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 12, 13, 14, 15 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 1, 2, 3 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 4;
    m_state.player->m_animation_rows   = 4;
    m_state.player->set_height(0.4f);
    m_state.player->set_width(0.4f);

    // Background
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    /** BGM and SFX */
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //m_state.bgm = Mix_LoadMUS("assets/firsttest.wav");
    //Mix_PlayMusic(m_state.bgm, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //m_state.jump_sfx = Mix_LoadWAV("assets/jump.wav");
}

void LevelOne::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    
    if (m_state.player->get_position().y < -10.0f) m_state.next_scene_id = 1;
}

void LevelOne::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    //m_state.enemies->render(program);
}
