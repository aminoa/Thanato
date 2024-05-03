#include "Scene.h"

class Brunswick : public Scene {
public:

    ~Brunswick();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
