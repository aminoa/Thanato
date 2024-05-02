#pragma once
#include "TextBox.h"
#include "Utility.h"
#include "ShaderProgram.h"

TextBox::TextBox()
{
    m_texture_id = Utility::load_texture("assets/textbox.png");
	m_is_active = false;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_model_matrix = glm::mat4(1.0f);
}

TextBox::~TextBox()
{
}

void TextBox::draw_string(std::string& text, ShaderProgram& g_shader_program, glm::mat4& g_view_matrix, float camera_scale, GLuint text_texture_id)
{
	glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / camera_scale) - 3, -4.0, 0);
	//Utility::draw_text(&g_shader_program, text_texture_id, text, 0.5f, 0.0f, text_position);
}

//void TextBox::draw_text(std::string& text, ShaderProgram& g_shader_program, glm::mat4& g_view_matrix, float camera_scale, GLuint text_texture_id)
//{
//	glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / camera_scale) - 3, -4.0, 0);
//	Utility::draw_text(&g_shader_program, text_texture_id, text, 0.5f, 0.0f, text_position);
//}

//void update_text(std::string& text, ShaderProgram& g_shader_program, glm::mat4& g_view_matrix, float camera_scale, GLuint text_texture_id)
//{
//	glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / camera_scale) - 3, -4.0, 0);
//	Utility::draw_text(&g_shader_program, text_texture_id, text, 0.5f, 0.0f, text_position);
//}

void TextBox::update(glm::mat4& g_view_matrix)
{
	glm::vec3 textbox_position = glm::vec3(-g_view_matrix[3].x * (1 / 0.5), -8.0f, 0);
	set_position(textbox_position);

	m_model_matrix = glm::mat4(1.0f);
	m_model_matrix = glm::translate(m_model_matrix, m_position);
}

void TextBox::render(ShaderProgram* program)
{
	// Render textbox
	//glm::vec3 textbox_position = glm::vec3(-g_view_matrix[3].x * (1 / CAMERA_SCALE) , -7.0, 0);
	////g_textbox->set_position(textbox_position);
	////g_textbox->render(&g_shader_program);
	//
	//// Render text
	//glm::vec3 text_position = glm::vec3(-g_view_matrix[3].x * (1 / CAMERA_SCALE) - 6, -6.5, 0);
	//Utility::draw_text(program, m_texture_id, "Hello World", 0.5f, 0.0f, text_position);

    if (!m_is_active) return;
	program->set_model_matrix(m_model_matrix);

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

    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
