#pragma once
#include "Core/Application.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector2.h"
namespace nkentseu {
    class DemoPerlinWalk : public Application {
        private:
        math::Vector2 position;
        float tx, ty;
        
        public:
        DemoPerlinWalk(const ApplicationProperties& props);
        
        protected:
        void Setup() override;
        void Update(float deltaTime) override;
        void Draw() override;
    };
}