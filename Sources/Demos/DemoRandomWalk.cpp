#include "DemoRandomWalk.h"

namespace nkentseu {
    DemoRandomWalk::DemoRandomWalk(const ApplicationProperties& props)
    : Application(props) {}
    
    void DemoRandomWalk::Setup() {
        position = math::Vector2(GetWindowWidth() / 2.0f, GetWindowHeight() / 2.0f);
        Background(graphics::Color::Black);
    }
    
    void DemoRandomWalk::Update(float deltaTime) {
        // La logique de mise à jour sera dans Draw pour cet exemple simple
    }
    
    /* 8 DIRECTIONS randomwalk
    void DemoRandomWalk::Draw() {
        // Dessiner le marcheur
        Fill(graphics::Color::White);
        NoStroke();
        Circle(position, 4.0f);
        
        // Prendre un pas aléatoire
        int choice = math::Random::Range(0, 7);
        
        if (choice == 0) {
            position.x++;
        } else if (choice == 1) {
            position.x--;
        } else if (choice == 2) {
            position.y++;
        } else if (choice == 3) {
            position.y--;
        } else if (choice == 4) {
            position.x++;
            position.y++;
        } else if (choice == 5) {
            position.x++;
            position.y--;
        } else if (choice == 6) {
            position.x--;
            position.y++;
        }else {
            position.x--;
            position.y--;
        }
        
        
        // Garder dans les limites de l'écran
        position.x = std::max(0.0f, std::min(position.x, (float)GetWindowWidth()));
        position.y = std::max(0.0f, std::min(position.y, (float)GetWindowHeight()));
    }*/

    /*Marcheur avec tendance
    void DemoRandomWalk::Draw() { 
        // Dessiner le marcheur
        Fill(graphics::Color::Blue);
        NoStroke();
        Circle(position, 4.0f);
        
        // Distribution non-uniforme: 40% droite, 20% autres directions
        float r = math::Random::Value();

        float mousex = nkentseu::events::Mouse::GetPosition().x;
        float mousey = nkentseu::events::Mouse::GetPosition().y;

        if (r < 0.4f) { // 40% de chance d'aller vers la souris
            if (position.x > mousex) {
                position.x--;
            }else {
                position.x++;
            }
            if (position.y > mousey) {
                position.y--;
            }else {
                position.y++;
            }
        } else if (r < 0.6f) {
            position.x--; // 20% gauche
        } else if (r < 0.8f) {
            position.y++; // 20% bas
        } else {
            position.y--; // 20% haut
        }
        
        // Garder dans les limites
        position.x = std::max(0.0f, std::min(position.x, (float)GetWindowWidth()));
        position.y = std::max(0.0f, std::min(position.y, (float)GetWindowHeight()));
    }*/

    void DemoRandomWalk::Draw() {
        // Dessiner le marcheur
        Fill(graphics::Color::Blue);
        NoStroke();
        Circle(position, 4.0f);
        
        // Mouvement avec distribution gaussienne
        float stepX = math::Random::Gaussian(0.0f, 2.0f);
        float stepY = math::Random::Gaussian(0.0f, 2.0f);
        
        position.x += stepX;
        position.y += stepY;
        
        // Garder dans les limites
        position.x = std::max(0.0f, std::min(position.x, (float)GetWindowWidth()));
        position.y = std::max(0.0f, std::min(position.y, (float)GetWindowHeight()));
    }
}

// Fonction de création
nkentseu::Application* CreateRandomWalkDemo(const nkentseu::ApplicationProperties& props) {
    return new nkentseu::DemoRandomWalk(props);
}