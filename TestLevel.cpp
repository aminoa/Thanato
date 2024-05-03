#include "TestLevel.h"
#include "Utility.h"

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
TestLevel::~TestLevel()
{
    delete [] m_state.enemies;
    delete m_state.player;
    delete m_state.map;
    //Mix_FreeChunk(m_state.jump_sfx);
    //Mix_FreeMusic(m_state.bgm);
}

void TestLevel::initialise()
{
    LEVEL_WIDTH = 14.0f;
    LEVEL_HEIGHT = 8.0f;
    LEVEL_LEFT_EDGE = 5.0f;

    m_state.next_scene_id = -1;
    m_number_of_enemies = ENEMY_COUNT;
    
    GLuint map_texture_id = Utility::load_texture("assets/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELONE_DATA, map_texture_id, 1.0f, 12, 11, true);
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    m_state.player = new Entity();
    player_initialize(m_state.player, glm::vec3(2.0f, -2.0f, 0.0f));    

    // Interactable Object
    m_state.interactables = new Entity[m_number_of_interactables];
    m_state.interactables[0].set_entity_type(INTERACTABLE);
    m_state.interactables[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[0].set_position(glm::vec3(3.0f, -2.0f, 0.0f));
    //m_state.interactables[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    // Background
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    /** BGM and SFX */
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //m_state.bgm = Mix_LoadMUS("assets/firsttest.wav");
    //Mix_PlayMusic(m_state.bgm, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //m_state.jump_sfx = Mix_LoadWAV("assets/jump.wav");
}

void TestLevel::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < m_number_of_interactables; ++i) 
    {
		m_state.interactables[i].update(delta_time, m_state.player, m_state.interactables, m_number_of_interactables, m_state.map);
    }
}

void TestLevel::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.interactables->render(program);
    //m_state.enemies->render(program);
}