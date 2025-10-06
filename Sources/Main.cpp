#include "Main.h"
#include "Demos/DemoParticles.h"
#include "Demos/DemoRandomWalk.h"
#include "Demos/DemoPerlinWalk.h"
#include "Demos/DemoEcosystem.h"
#include "Demos/count.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

// Déclaration des fonctions de démo
nkentseu::Application* CreateParticlesDemo(const nkentseu::ApplicationProperties& props);
nkentseu::Application* CreateRandomWalkDemo(const nkentseu::ApplicationProperties& props);
nkentseu::Application* CreatePerlinWalkDemo(const nkentseu::ApplicationProperties& props);
nkentseu::Application* CreateEcosystemDemo(const nkentseu::ApplicationProperties& props);

// Structure pour gérer les démos
struct DemoConfig {
    std::string name;
    std::string description;
    bool enabled;
    std::function<nkentseu::Application*(const
    nkentseu::ApplicationProperties&)> creator;
};

// Registre des démos disponibles
std::vector<DemoConfig> availableDemos = {
    {"particles", "Fire Particles Simulation", true, CreateParticlesDemo},
    {"randomwalk", "Random Walker Demo", true, CreateRandomWalkDemo},
    {"perlinwalk", "Perlin Walker Demo", true, CreatePerlinWalkDemo},
    {"ecosystem", "Ecosystem Demo", true, CreateEcosystemDemo},
};

// Fonction pour lire une valeur spécifique du fichier de config
std::string GetConfigValue(const std::string& configPath, const std::string& key) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        return "";
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Ignorer les commentaires et lignes vides
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty() || line[0] == '#') continue;
        
        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) continue;
        
        std::string currentKey = line.substr(0, equalsPos);
        std::string value = line.substr(equalsPos + 1);
        
        // Convertir la clé en minuscules pour case-insensitive
        std::transform(currentKey.begin(), currentKey.end(), currentKey.begin(), ::tolower);
        
        if (currentKey == key) {
            file.close();
            return value;
        }
    }
    file.close();
    return "";
}

// Charger la configuration des démos
void LoadDemoConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        nkentseu::Log.Warning("Cannot open demo config file: {0}", configPath);
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Ignorer les commentaires et lignes vides
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty() || line[0] == '#') continue;
        
        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) continue;
        
        std::string key = line.substr(0, equalsPos);
        std::string value = line.substr(equalsPos + 1);
        
        // Chercher la démo correspondante
        for (auto& demo : availableDemos) {
            if (key == demo.name + ".enabled") {
                demo.enabled = (value == "true" || value == "1");
                break;
            }
        }
    }
    file.close();
}

// Trouver une démo par nom
DemoConfig* FindDemo(const std::string& name) {
    for (auto& demo : availableDemos) {
        if (demo.name == name && demo.enabled) {
            return &demo;
        }
    }
    return nullptr;
}

// Fonction pour créer la démo particles
nkentseu::Application* CreateParticlesDemo(const nkentseu::ApplicationProperties& props) {
    return new nkentseu::DemoParticles(props);
}

// Fonction principale modifiée pour supporter multiple démos
nkentseu::Application* Main(nkentseu::Args args) {
    /*return new nkentseu::Count(nkentseu::Application::CreatePropertiesFromConfig("app.config"));*/
    // Charger la configuration des démos
    LoadDemoConfig("demos.config");
    
    // Lire la démo par défaut depuis app.config
    std::string defaultDemo = GetConfigValue("app.config", "defaultdemo");
    if (defaultDemo.empty()) {
        defaultDemo = "randomwalk"; // Valeur par défaut si non trouvée
    }
    
    std::string demoName = defaultDemo;
    
    // Chercher l'argument --demo (qui écrase la config)
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--demo" && i + 1 < args.size()) {
            demoName = args[i + 1];
            break;
        }
    }
    
    // Trouver la démo demandée
    DemoConfig* demoConfig = FindDemo(demoName);
    if (!demoConfig) {
        nkentseu::Log.Error("Demo not found or disabled: {0}", demoName);
        nkentseu::Log.Info("Available demos:");
        for (const auto& demo : availableDemos) {
            if (demo.enabled) {
                nkentseu::Log.Info("  {0}: {1}", demo.name, demo.description);
            }
        }
        return nullptr;
    }
    
    nkentseu::Log.Info("Loading demo: {0} - {1}", demoConfig->name, demoConfig->description);
    
    // Créer les propriétés de l'application
    auto props = nkentseu::Application::CreatePropertiesFromConfig("app.config");
    return demoConfig->creator(props);
    
}