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
	void activate() { m_is_active = true; }
	void deactivate() { m_is_active = false; }

	glm::vec3 get_position() { return m_position; }
	void set_position(glm::vec3 new_position) { m_position = new_position; }

	void update_textbox(glm::mat4& g_view_matrix);
	void render_textbox(ShaderProgram* program);
	void write_dialogue(std::string& text);
	void update_text(std::string text, float screen_size, float spacing);
	void render_text(ShaderProgram* program, glm::vec3 position);

	glm::mat4 m_model_matrix;

	GLuint m_textbox_texture_id;
	GLuint m_text_texture_id;
	bool m_is_active = false;

	glm::vec3 text_position;
	glm::vec3 m_position;
	std::string m_text_buffer;

    std::vector<float> vertices;
    std::vector<float> texture_coordinates;
	size_t text_size;
};

