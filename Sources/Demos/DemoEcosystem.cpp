#include "DemoEcosystem.h"

namespace nkentseu {
    DemoEcosystem::DemoEcosystem(const ApplicationProperties& props) : Application(props) {}
    
    void DemoEcosystem::Setup() {
        Background(graphics::Color::White);
        
        // Créer différentes créatures
        for (int i = 0; i < 50; i++) {
            Creature c;
            c.position = math::Vector2(
                math::Random::Range(0.0f, (float)GetWindowWidth()),
                math::Random::Range(0.0f, (float)GetWindowHeight())
            );
            
            c.type = math::Random::Range(0, 2);
            c.tx = math::Random::Range(0.0f, 1000.0f);
            c.ty = math::Random::Range(0.0f, 1000.0f);
            c.velocity = math::Vector2(0, 0);
            
            if (c.type == 0) { // Papillon - Mouvement erratique
                c.color = graphics::Color(255, 200, 0, 255);
                c.size = 3.0f;
            } else if (c.type == 1) { // Feuille - Mouvement doux avec bruit de Perlin
                c.color = graphics::Color(0, 180, 0, 255);
                c.size = 5.0f;
            } else { // Insecte - Petits mouvements rapides
                c.color = graphics::Color(100, 100, 100, 255);
                c.size = 2.0f;
            }
            creatures.push_back(c);
        }
    }
    
    void DemoEcosystem::Update(float deltaTime) {
        for (auto& creature : creatures) {
            UpdateCreature(creature, deltaTime);
        }
    }
    
    void DemoEcosystem::UpdateCreature(Creature& creature, float deltaTime) {
        // Mouvement selon le type
        if (creature.type == 0) {
            // Papillon: mouvement erratique avec tendance
            float r = math::Random::Value();
            if (r < 0.6f) creature.velocity.x += 0.1f;
            if (r < 0.3f) creature.velocity.y -= 0.1f;
            if (r > 0.8f) creature.velocity.x -= 0.1f;
            if (r > 0.9f) creature.velocity.y += 0.1f;
            
            // Appliquer une légère friction
            creature.velocity = creature.velocity * 0.95f;
        
        } else if (creature.type == 1) {
            // Feuille: mouvement doux avec bruit de Perlin
            float noiseX = math::Random::Noise(creature.tx);
            float noiseY = math::Random::Noise(creature.ty);
            
            creature.velocity.x += math::Random::Map(noiseX, 0.0f, 1.0f, - 0.05f, 0.05f);
            creature.velocity.y += math::Random::Map(noiseY, 0.0f, 1.0f, - 0.03f, 0.07f);
            
            creature.tx += 0.01f;
            creature.ty += 0.01f;
            
            // Appliquer une friction plus forte
            creature.velocity = creature.velocity * 0.9f;
        } else {
            // Insecte: petits mouvements rapides
            creature.velocity.x += math::Random::Range(-0.5f, 0.5f);
            creature.velocity.y += math::Random::Range(-0.5f, 0.5f);
            
            // Limiter la vitesse maximale
            if (creature.velocity.Magnitude() > 2.0f) {
                creature.velocity = creature.velocity.Normalized() * 2.0f;
            }
        }
        
        // Mettre à jour la position
        creature.position = creature.position + creature.velocity;
        
        // Garder dans les limites avec rebond
        if (creature.position.x < 0 || creature.position.x > GetWindowWidth()) {
            creature.velocity.x = -creature.velocity.x;
            creature.position.x = std::max(0.0f,
            std::min(creature.position.x, (float)GetWindowWidth()));
        }
        
        if (creature.position.y < 0 || creature.position.y > GetWindowHeight()) {
            creature.velocity.y = -creature.velocity.y;
            creature.position.y = std::max(0.0f,
            std::min(creature.position.y, (float)GetWindowHeight()));
        }
    }
    
    void DemoEcosystem::Draw() {
        // Effacer légèrement pour créer un effet de traînée
        Fill(graphics::Color(255, 255, 255, 10));
        NoStroke();
        Rect(math::Vector2(0, 0), math::Vector2(GetWindowWidth(), GetWindowHeight()));
        
        // Dessiner les créatures
        for (const auto& creature : creatures) {
            Fill(creature.color);
            NoStroke();
            Circle(creature.position, creature.size);
        }
    }
}

// Fonction de création
nkentseu::Application* CreateEcosystemDemo(const nkentseu::ApplicationProperties& props) {
    return new nkentseu::DemoEcosystem(props);
}