#include "Brunswick.h"
#include "Utility.h"
#include "TextBox.h"

#define u8 Uint8

const float BG_RED = 0.0f,
BG_BLUE = 0.0f,
BG_GREEN = 0.0f,
BG_OPACITY = 0.0f;

//auto bgm_music = "assets/Indoor Areas.wav";
//const int LOOP_FOREVER = -1;
//Mix_Music* g_music;

unsigned int BACKGROUND_MAP_Brunswick_DATA[] =
{
    192, 192, 193, 974, 974, 974, 974, 904, 906, 905, 974, 963, 964, 965, 974, 187, 188, 188, 188, 188,
            192, 731, 193, 974, 974, 930, 974, 974, 974, 974, 974, 1000, 1001, 1002, 974, 187, 188, 620, 188, 656,
            192, 192, 193, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 187, 188, 188, 188, 188,
            192, 192, 193, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 187, 188, 188, 188, 188,
            303, 1020, 378, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 372, 373, 373, 373, 1020,
            937, 937, 937, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 974, 937, 937, 937, 937, 937,
            1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011,
            705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705,
            715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715, 715,
            717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717, 717
};

//unsigned int OBJECT_MAP_Brunswick_DATA[] =
//{
//	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,605,-1,-1,-1,-1,-1,-1,-1,-1,-1,605,-1,-1,
//	-1,-1,-1,-1,-1,-1,-1,-1,605,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,605,-1,-1,-1,-1,-1
//};

Brunswick::~Brunswick()
{
    delete [] m_state.enemies;
    delete m_state.player;
    delete m_state.map;
    //Mix_FreeChunk(m_state.jump_sfx);
    //Mix_FreeMusic(m_state.bgm);
}

void Brunswick::initialise()
{
    m_number_of_interactables = 4;

    LEVEL_WIDTH = 20.0f;
    LEVEL_HEIGHT = 10.0f;
    LEVEL_LEFT_EDGE = 5.0f;


    int TILESET_WIDTH = 628 / 16;
	int TILESET_HEIGHT = 475 / 16;

    m_state.next_scene_id = -1;
    m_number_of_interactables = 4;

    GLuint map_texture_id = Utility::load_texture("assets/tilesets/roguelikeCity_magenta.png");

    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, BACKGROUND_MAP_Brunswick_DATA, map_texture_id, 1.0f, 37, 28, true);
    //m_state.object_map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, OBJECT_MAP_Brunswick_DATA, map_texture_id, 1.0f, 37, 28, false);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    m_state.player = new Entity();
    // 19, -5
    player_initialize(m_state.player, glm::vec3(19.0f, -5.0f, 0.0f));

    // 3 hedgehog AIs 
    m_state.interactables = new Entity[m_number_of_interactables];
    m_state.interactables[0].set_entity_type(INTERACTABLE);
    m_state.interactables[0].set_ai_type(WALKER);
    m_state.interactables[0].set_ai_state(WALKING);
    m_state.interactables[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[0].set_position(glm::vec3(9.0f, -4.0f, 0.0f));
    m_state.interactables[0].set_dialogue("Pet: Wow, another tile calamity! It seems that there was a really bad storm that messed up all the architecture outside and broke the graphics. Oh well, you'll probably want to go inside to the other house while the weather is still bad out. It'll clear in a couple of days. Oh, Ann? I haven't seen here recently unfortunately, I'll let you know though if I hear anything.");

    m_state.interactables[1].set_entity_type(INTERACTABLE);
    m_state.interactables[1].set_ai_type(WALKER);
    m_state.interactables[1].set_ai_state(WALKING);
    m_state.interactables[1].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[1].set_dialogue("Walt: Me, Pet, and Scoop are doing a little dance outside at the moment. While the storm is going, I'm not sure if there's a ton you can do in this area. But you can at least enjoy the vibes!");
    m_state.interactables[1].set_position(glm::vec3(5.0f, -5.0f, 0.0f));

    m_state.interactables[2].set_entity_type(INTERACTABLE);
    m_state.interactables[2].set_ai_type(WALKER);
    m_state.interactables[2].set_ai_state(WALKING);
    m_state.interactables[2].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.interactables[2].set_dialogue("Scoop: Don't go onto the road or else the world will reset! You probably don't want to do that yet, you're too young. I'd tell you not to kill yourself but that's a bit too trite to say.");
    m_state.interactables[2].set_position(glm::vec3(7.0f, -6.0f, 0.0f));

    m_state.interactables[3].set_entity_type(INTERACTABLE);
    m_state.interactables[3].set_dialogue("(Thoughts): Hmmm, the door seems to be locked. You'll need to find another way into the house");
    m_state.interactables[3].set_position(glm::vec3(1.0f, -4.3f, 0.0f));

    m_state.loading_zones = new Entity[1];
    m_state.loading_zones[0].set_entity_type(LOADING_ZONE);
    //m_state.loading_zones[0].m_texture_id = Utility::load_texture("assets/sonic.png");
    m_state.loading_zones[0].set_position(glm::vec3(1.0f, -1.3f, 0.0f));
    m_state.loading_zones[0].scene_swap = LevelIndex::PATI;

    // Background
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    /** BGM and SFX */
    m_state.text_buffer = "";
}

void Brunswick::update(float delta_time)
{
   m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    //m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.object_map);
    
    // Death via road

    if (m_state.player->get_position().y < -7.1f)
    {
		m_state.next_scene_id = 2;
	}

    // check collision with object map 
    //m_state.player->check_collision_x(m_state.object_map);
    //m_state.player->check_collision_y(m_state.object_map);

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

        //randomly set the hedgehog to either move left or right
		if (rand() % 2 == 0)
		{
			m_state.interactables[i].set_velocity(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else
		{
			m_state.interactables[i].set_velocity(glm::vec3(1.0f, 0.0f, 0.0f));
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

void Brunswick::render(ShaderProgram *program)
{
    m_state.loading_zones->render(program);
    m_state.map->render(program);
    //m_state.object_map->render(program);
    m_state.player->render(program);
    
    for (int i = 0; i < 3; ++i)
    {
		m_state.interactables[i].render(program);
	}

    //m_state.enemies->render(program);
}