#include "Scene.h"

class TestLevel : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~TestLevel();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
