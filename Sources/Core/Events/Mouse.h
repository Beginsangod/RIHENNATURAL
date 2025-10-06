#pragma once
#include "../Math/Vector2.h"

namespace nkentseu {
    namespace events {

        /**
         * @brief Énumération des boutons de souris
         */
        enum class MouseButton {
            Left = 1,
            Middle = 2,
            Right = 3,
            X1 = 4,
            X2 = 5
        };

        /**
         * @brief État de la souris
         */
        struct MouseState {
            math::Vector2 Position;
            math::Vector2 PreviousPosition;
            math::Vector2 Delta;
            bool LeftButton : 1;
            bool MiddleButton : 1;
            bool RightButton : 1;
            bool X1Button : 1;
            bool X2Button : 1;
        };

        /**
         * @brief Classe de gestion des événements souris
         */
        class Mouse {
            private:
                static MouseState sCurrentState;
                static MouseState sPreviousState;

            public:
                static void Update();
                
                static const math::Vector2& GetPosition() { return sCurrentState.Position; }
                static const math::Vector2& GetDelta() { return sCurrentState.Delta; }
                
                static bool IsButtonDown(MouseButton button);
                static bool IsButtonPressed(MouseButton button);
                static bool IsButtonReleased(MouseButton button);
        };

    } // namespace events
} // namespace nkentseu