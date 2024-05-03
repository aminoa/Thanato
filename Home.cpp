#include "Home.h"
#include "Utility.h"
#include "TextBox.h"

#define u8 Uint8

const float BG_RED = 0.0f,
BG_BLUE = 0.0f,
BG_GREEN = 0.0f,
BG_OPACITY = 0.0f;

unsigned int BACKGROUND_MAP_HOME_DATA[] =
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

unsigned int OBJECT_MAP_HOME_DATA[] =
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,50,82,51,52,-1,-1,-1,-1,41,41,-1,-1,-1,11,-1,144,-1,14,-1,
	-1,66,67,67,67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,46,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,161,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,96,97,98,99,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,112,113,114,115,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

Home::~Home()
{
    delete [] m_state.enemies;
    delete m_state.player;
    delete m_state.map;
    //Mix_FreeChunk(m_state.jump_sfx);
    //Mix_FreeMusic(m_state.bgm);
}

void Home::initialise()
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
    GLuint object_map_texture_id = Utility::load_texture("assets/tilesets/objects_indoor_house.png"); 

    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, BACKGROUND_MAP_HOME_DATA, map_texture_id, 1.0f, TILESET_WIDTH, TILESET_HEIGHT, true);
    m_state.object_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, OBJECT_MAP_HOME_DATA, object_map_texture_id, 1.0f, 512 / 32, 512 / 32, false);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    m_state.player = new Entity();
    player_initialize(m_state.player);

    // Interactable Object
    m_state.interactables = new Entity[m_number_of_interactables];
    m_state.interactables[0].set_entity_type(INTERACTABLE);
    m_state.interactables[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[0].set_position(glm::vec3(3.0f, -4.0f, 0.0f));
    m_state.interactables[0].set_dialogue("Sonic: Hey Ashva!. Good to see you again in New Brunswick... New Jersey (not Canada!). Last night, you were telling me that that you were afraid of something bad happening but didn't tell me what. If you want to talk to your mom, Ann's outside. Feel free to explore the environment here though before you head out. PS: try to avoid any puddles/holes in the ground. ");

    m_state.interactables[1].set_entity_type(INTERACTABLE);
    m_state.interactables[1].set_position(glm::vec3(9.0f, -2.0f, 0.0f));
    m_state.interactables[1].set_dialogue("(Thoughts): There are a ton of scattered papers around here. You decide to read the first one... 'Rutgers University Bachelors of History Report Card - March 20th, 2023 - Em, Ashva. World History I: A, Law and History: A, History of Exploration: A'. You may have done well academically, assauging your family fears of going outside of more traditional career paths, but you don't feel happy looking at it.");

    m_state.interactables[2].set_entity_type(INTERACTABLE);
    m_state.interactables[2].set_position(glm::vec3(16.0f, -2.0f, 0.0f));
    m_state.interactables[2].set_dialogue("(Thoughts): You see your old computer here, a bit dusty - you played too much 'League of Legends' on it back in high school, but stopped in college. Now that you think about it, your old animations are probably still there. Wow, Ashva the animator, that would have been crazy... well, history isn't too bad, it could have been computer science, oh shit");

    m_state.interactables[3].set_entity_type(INTERACTABLE);
    m_state.interactables[3].set_position(glm::vec3(16.0f, -6.0f, 0.0f));
    m_state.interactables[3].set_dialogue("(Thoughts): Your collections of history books - partly through 'These Truths' to improve your 16 year old understanding of US history which you forget half of. The shelf gives you a weird perspective on life though, right? All of these books chart the life stories of different notable people but none of them are here now. They were born, did some stuff, and then that's it, their lives were over. The same is gonna happen to you.");

    m_state.loading_zones = new Entity[1];
    m_state.loading_zones[0].set_entity_type(LOADING_ZONE);
    //m_state.loading_zones[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.loading_zones[0].set_position(glm::vec3(9.5f, -9.0f, 0.0f));
    m_state.loading_zones[0].scene_swap = 0;

    // Background
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    /** BGM and SFX */
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //m_state.bgm = Mix_LoadMUS("assets/firsttest.wav");
    //Mix_PlayMusic(m_state.bgm, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //m_state.jump_sfx = Mix_LoadWAV("assets/jump.wav");

    m_state.text_buffer = "";
}

void Home::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    //m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.object_map);
    
    // check collision with object map 
    m_state.player->check_collision_x(m_state.object_map);
    m_state.player->check_collision_y(m_state.object_map);

    const u8* key_state = SDL_GetKeyboardState(NULL);
    
    for (int i = 0; i < m_number_of_interactables; ++i) 
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

void Home::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.object_map->render(program);
    m_state.player->render(program);
    m_state.interactables->render(program);
    m_state.loading_zones->render(program);
    //m_state.enemies->render(program);
}