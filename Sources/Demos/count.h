#pragma once
#include "Core/Application.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector2.h"

namespace nkentseu {

    class Count: public Application {
        private:
            math::Vector2 position;
            float gravity = 9.81;
            float r = 100;
            float friction = 20;

        public:
            Count(const ApplicationProperties& props);

        protected:
            void Setup() override;
            void Update(float deltaTime) override;
            void Draw() override;
    };
}