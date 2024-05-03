#include "EndScreen.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int EndScreen_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const char FONT_FILEPATH[] = "assets/all_8x8_white.png";

const float BG_RED = 0.2f,
BG_BLUE = 0.2f,
BG_GREEN = 0.2f,
BG_OPACITY = 1.0f;

//auto bgm_music = "assets/Indoor Areas.wav";
//const int LOOP_ONCE = 0;
//Mix_Music* g_music;

void EndScreen::initialise()
{
    int level_number = 0;

    m_state.next_scene_id = -1;
    GLuint map_texture_id = Utility::load_texture("assets/black.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, EndScreen_DATA, map_texture_id, 1.0f, 4, 1);

    // Can't remove this code atm
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->m_speed = 2.5f;
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start left 
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 4;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    // Text
    m_state.title_screen_text = new Entity();
    m_state.title_screen_text->m_texture_id = Utility::load_texture(FONT_FILEPATH);
    m_state.title_screen_text->set_position(glm::vec3(0.0f, 1.0f, 0.0f));

    // BGM and SFX
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //m_state.bgm = Mix_LoadMUS("assets/Title Screen.wav");
    //Mix_PlayMusic(m_state.bgm, 0);
}

void EndScreen::update(float delta_time)
{
    // check if enter is pressed
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_RETURN])
    {
        m_state.next_scene_id = 1;
    }
}

void EndScreen::render(ShaderProgram* program)
{
    Utility::draw_text(program, m_state.title_screen_text->m_texture_id, "You Win!", 0.5f, 0.1f, glm::vec3(3.0f, -1.0f, 0.0f));
    Utility::draw_text(program, m_state.title_screen_text->m_texture_id, "Debug Select: D+NUM", 0.5f, 0.1f, glm::vec3(2.0f, -2.0f, 0.0f));
    m_state.map->render(program);
}
