#pragma once

namespace nkentseu {
    namespace math {

        /**
         * @brief Constantes mathématiques courantes
         */
        class Constants {
            public:
                static constexpr float PI = 3.14159265358979323846f;
                static constexpr float TWO_PI = 2.0f * PI;
                static constexpr float HALF_PI = PI / 2.0f;
                static constexpr float QUARTER_PI = PI / 4.0f;
                
                static constexpr float DEG_TO_RAD = PI / 180.0f;
                static constexpr float RAD_TO_DEG = 180.0f / PI;
                
                static constexpr float E = 2.71828182845904523536f;
                static constexpr float SQRT2 = 1.41421356237309504880f;
                static constexpr float SQRT3 = 1.73205080756887729352f;
                
                static constexpr float GOLDEN_RATIO = 1.61803398874989484820f;
        };

        // Constantes globales pour un accès facile
        constexpr float PI = Constants::PI;
        constexpr float TWO_PI = Constants::TWO_PI;
        constexpr float HALF_PI = Constants::HALF_PI;

    } // namespace math
} // namespace nkentseu