#pragma once
#include "Core/Application.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector2.h"
#include "Graphics/Color.h"
#include <vector>

namespace nkentseu {
    class DemoEcosystem : public Application {
        private:
            struct Creature {
                math::Vector2 position;
                graphics::Color color;
                float size;
                int type; // 0: papillon, 1: feuille, 2: insecte
                float tx, ty; // Pour le bruit de Perlin
                math::Vector2 velocity;
            };
            
            std::vector<Creature> creatures;
        
        public:
            DemoEcosystem(const ApplicationProperties& props);
            
            protected:
            void Setup() override;
            void Update(float deltaTime) override;
            void Draw() override;
        
        private:
            void UpdateCreature(Creature& creature, float deltaTime);
    };
}