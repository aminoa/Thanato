#include "Scene.h"

class Pati : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~Pati();

    void initialise() override;
    //void update(float delta_time, ShaderProgram* program, glm::mat4& g_view_matrix);
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
