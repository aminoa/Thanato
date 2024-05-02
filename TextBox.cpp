#pragma once
#include "TextBox.h"
#include "Utility.h"
#include "ShaderProgram.h"
#define LOG(argument) std::cout << argument << '\n'
#define STB_IMAGE_IMPLEMENTATION
#define NUMBER_OF_TEXTURES 1 // to be generated, that is
#define LEVEL_OF_DETAIL 0    // base image level; Level n is the nth mipmap reduction image
#define TEXTURE_BORDER 0     // this value MUST be zero
#define FONTBANK_SIZE 10

TextBox::TextBox()
{
	m_text_texture_id = Utility::load_texture("assets/all_8x8.png");
    m_textbox_texture_id = Utility::load_texture("assets/textbox.png");
	m_is_active = false;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_model_matrix = glm::mat4(1.0f);
	text_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_text_buffer = "";
}

//void TextBox::write_dialogue(std::string& text, bool &m_locked)
//{
//	//m_text_buffer = text;
//	while (m_locked)
//	{
//		m_text_buffer = text;
//	}
//
//}

//void update_text(std::string& text, ShaderProgram& g_shader_program, glm::mat4& g_view_matrix, float camera_scale, GLuint text_texture_id)
//{
//	glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / camera_scale) - 3, -4.0, 0);
//	Utility::draw_text(&g_shader_program, text_texture_id, text, 0.5f, 0.0f, text_position);
//}

void TextBox::update_textbox(glm::mat4& g_view_matrix)
{
	glm::vec3 textbox_position = glm::vec3(-g_view_matrix[3].x * (1 / 0.5), -8.0f, 0);
	set_position(textbox_position);

	text_position = glm::vec3(-g_view_matrix[3].x * (1 / 0.5) - 6, -6.5, 0);

	m_model_matrix = glm::mat4(1.0f);
	m_model_matrix = glm::translate(m_model_matrix, m_position);
}

void TextBox::render_textbox(ShaderProgram* program)
{
    if (!m_is_active) return;

	program->set_model_matrix(m_model_matrix);

	// for the textbox
    float vertices[12];
    float tex_coords[12];

	int WIDTH_ADJUST = 9.0f;
	int HEIGHT_ADJUST = 2.0f;

	vertices[0] = -WIDTH_ADJUST; vertices[1] = -HEIGHT_ADJUST; // Bottom left
	vertices[2] = WIDTH_ADJUST; vertices[3] = -HEIGHT_ADJUST; // Bottom right
	vertices[4] = WIDTH_ADJUST; vertices[5] = HEIGHT_ADJUST; // Top right
	vertices[6] = -WIDTH_ADJUST; vertices[7] = -HEIGHT_ADJUST; // Bottom left (repeated for triangle strip)
	vertices[8] = WIDTH_ADJUST; vertices[9] = HEIGHT_ADJUST; // Top right (repeated for triangle strip)
	vertices[10] = -WIDTH_ADJUST; vertices[11] = HEIGHT_ADJUST; // Top left
	tex_coords[0] = 0.0; tex_coords[1] = 1.0; tex_coords[2] = 1.0; tex_coords[3] = 1.0; tex_coords[4] = 1.0; tex_coords[5] = 0.0;
	tex_coords[6] = 0.0; tex_coords[7] = 1.0; tex_coords[8] = 1.0; tex_coords[9] = 0.0; tex_coords[10] = 0.0; tex_coords[11] = 0.0;

    glBindTexture(GL_TEXTURE_2D, m_textbox_texture_id);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

// TODO: refactor this to render above the screen (such that it doesn't move with the other text)
void TextBox::update_text(std::string text, float screen_size, float spacing)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    vertices.clear();
    texture_coordinates.clear();

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int spritesheet_index = (int)(text[i] - 32) % (FONTBANK_SIZE * FONTBANK_SIZE);  // ascii value of character
        float offset = (screen_size + spacing) * i;

        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float)(spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float)(spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
            });
    }

    text_size = text.size();

}

void TextBox::render_text(ShaderProgram* program, glm::vec3 position)
{
    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    program->set_model_matrix(model_matrix);
    glUseProgram(program->get_program_id());

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, m_text_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text_size * 6));

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}