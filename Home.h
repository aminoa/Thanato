#include "Scene.h"

class Home : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~Home();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};