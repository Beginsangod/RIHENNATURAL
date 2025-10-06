#include "Keyboard.h"

namespace nkentseu {
    namespace events {

        std::unordered_map<KeyCode, bool> Keyboard::sCurrentState;
        std::unordered_map<KeyCode, bool> Keyboard::sPreviousState;

        void Keyboard::Update() {
            // Sauvegarder l'état précédent
            sPreviousState = sCurrentState;
            
            // Mettre à jour l'état actuel
            const bool* keyboardState = SDL_GetKeyboardState(nullptr);
            // const unsigned char* keyboardState = SDL_GetKeyboardState(nullptr);
            if (keyboardState) {
                // Mettre à jour toutes les touches en utilisant les scancodes
                sCurrentState[KeyCode::A] = keyboardState[SDL_SCANCODE_A];
                sCurrentState[KeyCode::B] = keyboardState[SDL_SCANCODE_B];
                sCurrentState[KeyCode::C] = keyboardState[SDL_SCANCODE_C];
                sCurrentState[KeyCode::D] = keyboardState[SDL_SCANCODE_D];
                sCurrentState[KeyCode::E] = keyboardState[SDL_SCANCODE_E];
                sCurrentState[KeyCode::F] = keyboardState[SDL_SCANCODE_F];
                sCurrentState[KeyCode::G] = keyboardState[SDL_SCANCODE_G];
                sCurrentState[KeyCode::H] = keyboardState[SDL_SCANCODE_H];
                sCurrentState[KeyCode::I] = keyboardState[SDL_SCANCODE_I];
                sCurrentState[KeyCode::J] = keyboardState[SDL_SCANCODE_J];
                sCurrentState[KeyCode::K] = keyboardState[SDL_SCANCODE_K];
                sCurrentState[KeyCode::L] = keyboardState[SDL_SCANCODE_L];
                sCurrentState[KeyCode::M] = keyboardState[SDL_SCANCODE_M];
                sCurrentState[KeyCode::N] = keyboardState[SDL_SCANCODE_N];
                sCurrentState[KeyCode::O] = keyboardState[SDL_SCANCODE_O];
                sCurrentState[KeyCode::P] = keyboardState[SDL_SCANCODE_P];
                sCurrentState[KeyCode::Q] = keyboardState[SDL_SCANCODE_Q];
                sCurrentState[KeyCode::R] = keyboardState[SDL_SCANCODE_R];
                sCurrentState[KeyCode::S] = keyboardState[SDL_SCANCODE_S];
                sCurrentState[KeyCode::T] = keyboardState[SDL_SCANCODE_T];
                sCurrentState[KeyCode::U] = keyboardState[SDL_SCANCODE_U];
                sCurrentState[KeyCode::V] = keyboardState[SDL_SCANCODE_V];
                sCurrentState[KeyCode::W] = keyboardState[SDL_SCANCODE_W];
                sCurrentState[KeyCode::X] = keyboardState[SDL_SCANCODE_X];
                sCurrentState[KeyCode::Y] = keyboardState[SDL_SCANCODE_Y];
                sCurrentState[KeyCode::Z] = keyboardState[SDL_SCANCODE_Z];
                
                sCurrentState[KeyCode::Num0] = keyboardState[SDL_SCANCODE_0];
                sCurrentState[KeyCode::Num1] = keyboardState[SDL_SCANCODE_1];
                sCurrentState[KeyCode::Num2] = keyboardState[SDL_SCANCODE_2];
                sCurrentState[KeyCode::Num3] = keyboardState[SDL_SCANCODE_3];
                sCurrentState[KeyCode::Num4] = keyboardState[SDL_SCANCODE_4];
                sCurrentState[KeyCode::Num5] = keyboardState[SDL_SCANCODE_5];
                sCurrentState[KeyCode::Num6] = keyboardState[SDL_SCANCODE_6];
                sCurrentState[KeyCode::Num7] = keyboardState[SDL_SCANCODE_7];
                sCurrentState[KeyCode::Num8] = keyboardState[SDL_SCANCODE_8];
                sCurrentState[KeyCode::Num9] = keyboardState[SDL_SCANCODE_9];
                
                sCurrentState[KeyCode::F1] = keyboardState[SDL_SCANCODE_F1];
                sCurrentState[KeyCode::F2] = keyboardState[SDL_SCANCODE_F2];
                sCurrentState[KeyCode::F3] = keyboardState[SDL_SCANCODE_F3];
                sCurrentState[KeyCode::F4] = keyboardState[SDL_SCANCODE_F4];
                sCurrentState[KeyCode::F5] = keyboardState[SDL_SCANCODE_F5];
                sCurrentState[KeyCode::F6] = keyboardState[SDL_SCANCODE_F6];
                sCurrentState[KeyCode::F7] = keyboardState[SDL_SCANCODE_F7];
                sCurrentState[KeyCode::F8] = keyboardState[SDL_SCANCODE_F8];
                sCurrentState[KeyCode::F9] = keyboardState[SDL_SCANCODE_F9];
                sCurrentState[KeyCode::F10] = keyboardState[SDL_SCANCODE_F10];
                sCurrentState[KeyCode::F11] = keyboardState[SDL_SCANCODE_F11];
                sCurrentState[KeyCode::F12] = keyboardState[SDL_SCANCODE_F12];
                
                sCurrentState[KeyCode::Escape] = keyboardState[SDL_SCANCODE_ESCAPE];
                sCurrentState[KeyCode::Tab] = keyboardState[SDL_SCANCODE_TAB];
                sCurrentState[KeyCode::CapsLock] = keyboardState[SDL_SCANCODE_CAPSLOCK];
                sCurrentState[KeyCode::Shift] = keyboardState[SDL_SCANCODE_LSHIFT];
                sCurrentState[KeyCode::Ctrl] = keyboardState[SDL_SCANCODE_LCTRL];
                sCurrentState[KeyCode::Alt] = keyboardState[SDL_SCANCODE_LALT];
                sCurrentState[KeyCode::Space] = keyboardState[SDL_SCANCODE_SPACE];
                sCurrentState[KeyCode::Enter] = keyboardState[SDL_SCANCODE_RETURN];
                sCurrentState[KeyCode::Backspace] = keyboardState[SDL_SCANCODE_BACKSPACE];
                sCurrentState[KeyCode::Delete] = keyboardState[SDL_SCANCODE_DELETE];
                
                sCurrentState[KeyCode::Up] = keyboardState[SDL_SCANCODE_UP];
                sCurrentState[KeyCode::Down] = keyboardState[SDL_SCANCODE_DOWN];
                sCurrentState[KeyCode::Left] = keyboardState[SDL_SCANCODE_LEFT];
                sCurrentState[KeyCode::Right] = keyboardState[SDL_SCANCODE_RIGHT];
                
                sCurrentState[KeyCode::LeftShift] = keyboardState[SDL_SCANCODE_LSHIFT];
                sCurrentState[KeyCode::RightShift] = keyboardState[SDL_SCANCODE_RSHIFT];
                sCurrentState[KeyCode::LeftCtrl] = keyboardState[SDL_SCANCODE_LCTRL];
                sCurrentState[KeyCode::RightCtrl] = keyboardState[SDL_SCANCODE_RCTRL];
                sCurrentState[KeyCode::LeftAlt] = keyboardState[SDL_SCANCODE_LALT];
                sCurrentState[KeyCode::RightAlt] = keyboardState[SDL_SCANCODE_RALT];
            }
        }

        bool Keyboard::IsKeyDown(KeyCode key) {
            auto it = sCurrentState.find(key);
            return it != sCurrentState.end() && it->second;
        }

        bool Keyboard::IsKeyPressed(KeyCode key) {
            auto currentIt = sCurrentState.find(key);
            auto previousIt = sPreviousState.find(key);
            
            bool current = (currentIt != sCurrentState.end()) && currentIt->second;
            bool previous = (previousIt != sPreviousState.end()) && previousIt->second;
            
            return current && !previous;
        }

        bool Keyboard::IsKeyReleased(KeyCode key) {
            auto currentIt = sCurrentState.find(key);
            auto previousIt = sPreviousState.find(key);
            
            bool current = (currentIt != sCurrentState.end()) && currentIt->second;
            bool previous = (previousIt != sPreviousState.end()) && previousIt->second;
            
            return !current && previous;
        }

        void Keyboard::Reset() {
            sCurrentState.clear();
            sPreviousState.clear();
        }

    } // namespace events
} // namespace nkentseu