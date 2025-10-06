#pragma once
#include "Core/Application.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Random.h"
#include "Graphics/Color.h"
#include <vector>
#include <random>
#include <string>

namespace nkentseu {

    class DemoParticles : public Application {
        private:
            std::vector<math::Vector2> walker;

        public:
            DemoParticles(const ApplicationProperties& props);

        protected:
            void Setup() override;
            void Update(float deltaTime) override;
            void Draw() override;
    };

} // namespace nkentseu