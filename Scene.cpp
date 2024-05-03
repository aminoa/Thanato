#include "Scene.h"

void Scene::player_initialize(Entity* player, glm::vec3 start_position)
{
    // Existing
    player->set_entity_type(PLAYER);
    player->set_position(start_position);
    player->set_movement(glm::vec3(0.0f));
    player->m_texture_id = Utility::load_texture("./assets/characters/ashva.png");
    
    // Walking
    player->m_walking[player->LEFT] = new int[4] { 3, 7, 3, 11 };
    player->m_walking[player->RIGHT] = new int[4] { 1, 5, 1, 9 };
    player->m_walking[player->UP] = new int[4] { 2, 6, 2, 10 };
    player->m_walking[player->DOWN] = new int[4] { 0, 4, 0, 8 };

    player->m_animation_indices = player->m_walking[player->RIGHT];  
    player->m_animation_frames = 4;
    player->m_animation_index  = 0;
    player->m_animation_time   = 0.5f;
    player->m_animation_cols   = 4;
    player->m_animation_rows   = 3;
    player->set_height(1.0f);
    player->set_width(1.0f);
}
