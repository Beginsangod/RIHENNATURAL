#include "Mouse.h"
#include <SDL3/SDL.h>

namespace nkentseu {
    namespace events {

        MouseState Mouse::sCurrentState;
        MouseState Mouse::sPreviousState;

        void Mouse::Update() {
            // Sauvegarder l'état précédent
            sPreviousState = sCurrentState;
            
            // Mettre à jour la position
            float x, y;
            SDL_GetMouseState(&x, &y);
            sCurrentState.PreviousPosition = sCurrentState.Position;
            sCurrentState.Position = math::Vector2(x, y);
            sCurrentState.Delta = sCurrentState.Position - sCurrentState.PreviousPosition;
            
            // Mettre à jour les boutons
            Uint32 buttonState = SDL_GetMouseState(nullptr, nullptr);
            sCurrentState.LeftButton = (buttonState & SDL_BUTTON_LMASK) != 0;
            sCurrentState.MiddleButton = (buttonState & SDL_BUTTON_MMASK) != 0;
            sCurrentState.RightButton = (buttonState & SDL_BUTTON_RMASK) != 0;
            sCurrentState.X1Button = (buttonState & SDL_BUTTON_X1MASK) != 0;
            sCurrentState.X2Button = (buttonState & SDL_BUTTON_X2MASK) != 0;
        }

        bool Mouse::IsButtonDown(MouseButton button) {
            switch (button) {
                case MouseButton::Left: return sCurrentState.LeftButton;
                case MouseButton::Middle: return sCurrentState.MiddleButton;
                case MouseButton::Right: return sCurrentState.RightButton;
                case MouseButton::X1: return sCurrentState.X1Button;
                case MouseButton::X2: return sCurrentState.X2Button;
                default: return false;
            }
        }

        bool Mouse::IsButtonPressed(MouseButton button) {
            bool current = false;
            bool previous = false;
            
            switch (button) {
                case MouseButton::Left:
                    current = sCurrentState.LeftButton;
                    previous = sPreviousState.LeftButton;
                    break;
                case MouseButton::Middle:
                    current = sCurrentState.MiddleButton;
                    previous = sPreviousState.MiddleButton;
                    break;
                case MouseButton::Right:
                    current = sCurrentState.RightButton;
                    previous = sPreviousState.RightButton;
                    break;
                case MouseButton::X1:
                    current = sCurrentState.X1Button;
                    previous = sPreviousState.X1Button;
                    break;
                case MouseButton::X2:
                    current = sCurrentState.X2Button;
                    previous = sPreviousState.X2Button;
                    break;
            }
            
            return current && !previous;
        }

        bool Mouse::IsButtonReleased(MouseButton button) {
            bool current = false;
            bool previous = false;
            
            switch (button) {
                case MouseButton::Left:
                    current = sCurrentState.LeftButton;
                    previous = sPreviousState.LeftButton;
                    break;
                case MouseButton::Middle:
                    current = sCurrentState.MiddleButton;
                    previous = sPreviousState.MiddleButton;
                    break;
                case MouseButton::Right:
                    current = sCurrentState.RightButton;
                    previous = sPreviousState.RightButton;
                    break;
                case MouseButton::X1:
                    current = sCurrentState.X1Button;
                    previous = sPreviousState.X1Button;
                    break;
                case MouseButton::X2:
                    current = sCurrentState.X2Button;
                    previous = sPreviousState.X2Button;
                    break;
            }
            
            return !current && previous;
        }

    } // namespace events
} // namespace nkentseu