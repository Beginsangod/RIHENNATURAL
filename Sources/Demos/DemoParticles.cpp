#include "DemoParticles.h"
#include "Core/Log.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace nkentseu {

    // DemoParticles methods
    DemoParticles::DemoParticles(const ApplicationProperties& props) : Application(props) {
        
    }

    void DemoParticles::Setup() {
        SetWindowTitle("🔥 box move Demo ");
        SetWindowSize(1024, 768);
        walker.push_back(math::Vector2 (10, 10));
    }

    void DemoParticles::Update(float deltaTime) {
        walker[0] += math::Vector2(math::Random::Range<int>(-1,1),math::Random::Range<int>(-1,1));
    }

    void DemoParticles::Draw() {
        // Implementation dépendante de votre moteur graphique
        Background(graphics::Color(255, 255, 255, 255));
        Fill(graphics::Color(255, 255, 255, 255));
        Stroke(graphics::Color(138, 128, 10, 255));

        int width = GetWindowWidth();
        int height = GetWindowHeight();

        int size = 20;
        int col = width / size;
        int line = height / size;

        for (int i = 0; i < col; i++)
        {
            for (int j = 0; j < line; j++)
            {
                Rect(math::Vector2(i*size,j*size), math::Vector2(size,size));
            }
        }
        Fill(graphics::Color::Red);
        for (auto walk:walker)
        {
            Rect(math::Vector2(walk.x * size, walk.y * size), math::Vector2(size,size));
        }
    }  

} // namespace nkentseu