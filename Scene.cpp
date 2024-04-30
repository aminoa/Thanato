#include "Scene.h"

void Scene::player_initialize(Entity* player)
{
    // Existing
    player->set_entity_type(PLAYER);
    player->set_position(m_player_start_position);
    player->set_movement(glm::vec3(0.0f));
    player->m_texture_id = Utility::load_texture("assets/taekshi.png");
    
    // Walking
    player->m_walking[player->LEFT] = new int[4] { 4, 5, 6, 7 };
    player->m_walking[player->RIGHT] = new int[4] { 8, 9, 10, 11 };
    player->m_walking[player->UP] = new int[4] { 12, 13, 14, 15 };
    player->m_walking[player->DOWN] = new int[4] { 0, 1, 2, 3 };

    player->m_animation_indices = player->m_walking[player->RIGHT];  
    player->m_animation_frames = 4;
    player->m_animation_index  = 0;
    player->m_animation_time   = 0.0f;
    player->m_animation_cols   = 4;
    player->m_animation_rows   = 4;
    player->set_height(0.4f);
    player->set_width(0.4f);
}
