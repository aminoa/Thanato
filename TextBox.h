#pragma once
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "Utility.h"
#include "glm/mat4x4.hpp"

class TextBox
{
public:
	TextBox();
	~TextBox();
	void draw_string(std::string& text, ShaderProgram& g_shader_program, glm::mat4& g_view_matrix, float camera_scale, GLuint text_texture_id);
	void render(ShaderProgram* program);

	void activate() { m_is_active = true; }
	void deactivate() { m_is_active = false; }

	glm::vec3 get_position() { return m_position; }
	void set_position(glm::vec3 new_position) { m_position = new_position; }

	void update(glm::mat4& g_view_matrix);
	glm::mat4 m_model_matrix;

private:
	bool m_is_active = false;
	GLuint m_texture_id;

	glm::vec3 m_position;

};

