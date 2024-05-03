#pragma once
#include "Scene.h"

class TitleScreen : public Scene
{
public:
	int ENEMY_COUNT = 0;

	~TitleScreen();

	void initialise() override;
	void update(float delta_time) override;
	void render(ShaderProgram* program) override;
};

