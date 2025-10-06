#pragma once
#include "Core/Application.h"
#include "Core/Math/Random.h"
#include "Core/Events/Mouse.h"
#include "Core/Math/Vector2.h"

namespace nkentseu {
    class DemoRandomWalk : public Application {
        private:
        math::Vector2 position;
        
        public:
        DemoRandomWalk(const ApplicationProperties& props);
        
        protected:
        void Setup() override;
        void Update(float deltaTime) override;
        void Draw() override;
    };
}