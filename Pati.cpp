#include "Pati.h"
#include "Utility.h"
#include "TextBox.h"

#define u8 Uint8

const float BG_RED = 0.0f,
BG_BLUE = 0.0f,
BG_GREEN = 0.0f,
BG_OPACITY = 0.0f;

unsigned int BACKGROUND_MAP_Pati_DATA[] =
{
    16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    142,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,5458,144,
    273,273,273,273,273,273,273,273,273,-1,-1,273,273,273,273,273,273,273,273,273
};

Pati::~Pati()
{
    delete[] m_state.enemies;
    delete m_state.player;
    delete m_state.map;
    //Mix_FreeChunk(m_state.jump_sfx);
    //Mix_FreeMusic(m_state.bgm);
}

void Pati::initialise()
{
    LEVEL_WIDTH = 20.0f;
    LEVEL_HEIGHT = 10.0f;
    LEVEL_LEFT_EDGE = 5.0f;

    int TILESET_WIDTH = 4050 / 32;
    int TILESET_HEIGHT = 3456 / 32;

    m_state.next_scene_id = -1;
    m_number_of_interactables = 4;

    // Every scene will have two maps, a background map and an object map - map rendering won't need to use tile ids, 
    // Background map will have no collsion, object map will have collision, will render background map, then object map, specify type in initialization or something
    // TODO: I need to think of a way to get interactions working with the object map

    GLuint map_texture_id = Utility::load_texture("assets/tilesets/indoor_house.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, BACKGROUND_MAP_Pati_DATA, map_texture_id, 1.0f, TILESET_WIDTH, TILESET_HEIGHT, true);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    m_state.player = new Entity();
    player_initialize(m_state.player, glm::vec3(9.3f, -7.0f, 0.0f));

    // Interactable Object
    m_state.interactables = new Entity[m_number_of_interactables];
    m_state.interactables[0].set_entity_type(INTERACTABLE);
    m_state.interactables[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[0].set_position(glm::vec3(3.0f, -4.0f, 0.0f));
    m_state.interactables[0].set_dialogue("Pati: Get out. Now.");

    m_state.interactables[1].set_entity_type(INTERACTABLE);
    m_state.interactables[1].set_position(glm::vec3(9.0f, -2.0f, 0.0f));
    m_state.interactables[1].set_dialogue("");

    m_state.loading_zones = new Entity[1];
    m_state.loading_zones[0].set_entity_type(LOADING_ZONE);
    //m_state.loading_zones[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.loading_zones[0].set_position(glm::vec3(9.5f, -9.0f, 0.0f));
    m_state.loading_zones[0].scene_swap = LevelIndex::END_SCREEN;

    // Background
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    /** BGM and SFX */
    //g_music = Mix_LoadMUS(bgm_music);

    m_state.text_buffer = "";
}

void Pati::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    //m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.object_map);

    const u8* key_state = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < 2; ++i)
    {
        m_state.interactables[i].update(delta_time, m_state.player, m_state.interactables, m_number_of_interactables, m_state.map);
        if (m_state.player->check_collision(&m_state.interactables[i]) && key_state[SDL_SCANCODE_C])
        {
            m_state.player->m_locked = true;
            std::cout << "Interaction occured" << std::endl;
            std::string dialogue = m_state.interactables[i].get_dialogue();
            m_state.text_buffer = dialogue;
        }
    }

    for (int i = 0; i < 1; ++i)
    {
        m_state.loading_zones[i].update(delta_time, m_state.player, m_state.loading_zones, 1, m_state.map);
        if (m_state.player->check_collision(&m_state.loading_zones[i]))
        {
            m_state.next_scene_id = m_state.loading_zones[i].scene_swap;
        }
    }

}

void Pati::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.interactables->render(program);
    m_state.loading_zones->render(program);
    //m_state.enemies->render(program);
}
