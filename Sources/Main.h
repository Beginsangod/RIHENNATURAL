#include "Core/Application.h"
#include <SDL3/SDL_main.h>
#include "Core/Log.h"

// Déclaration de la fonction utilisateur
nkentseu::Application* Main(nkentseu::Args args);

int main(int argc, char* argv[]) {
    // Initialiser le système de logging
    nkentseu::Logger::Initialize();
    
    // Convertir les arguments
    nkentseu::Args args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    
    nkentseu::Log.Info("Starting RihenNatural application");
    nkentseu::Log.Info("Arguments count: {0}", args.size());
    
    // Appeler la fonction principale de l'utilisateur
    nkentseu::Application* app = Main(args);
    
    if (!app) {
        nkentseu::Log.Error("Main() function returned null pointer");
        nkentseu::Log.Shutdown();
        return 1;
    }
    
    // Initialiser l'application
    if (!app->Initialize()) {
        nkentseu::Log.Error("Failed to initialize application");
        delete app;
        nkentseu::Log.Shutdown();
        return 1;
    }
    
    // Exécuter la boucle principale
    app->Run();
    
    // Nettoyer
    delete app;
    nkentseu::Logger::Shutdown();
    
    return 0;
}

// Implémentation par défaut
// nkentseu::Application* Main(nkentseu::Args args) {
//     nkentseu::Log.Warning("Using default Main() implementation");
    
//     // L'utilisateur peut choisir comment créer les propriétés
//     auto props = nkentseu::Application::CreatePropertiesFromArgs(args);
//     return new nkentseu::Application(props);
// }



// nkentseu::Application* Main(nkentseu::Args /*args*/) {
//     // Option 1: Utiliser uniquement les arguments CLI
//     // auto props = nkentseu::Application::CreatePropertiesFromArgs(args);
    
//     // Option 2: Charger depuis un fichier de config spécifique
//     auto props = nkentseu::Application::CreatePropertiesFromConfig("app.config");
    
//     // Option 3: Combiner les deux
//     // auto props = nkentseu::Application::CreatePropertiesFromArgs(args);
    
//     // Option 4: Valeurs par défaut + personnalisation manuelle
//     // auto props = nkentseu::Application::CreateDefaultProperties();
//     // props.windowTitle = "My Custom App";
//     // props.windowWidth = 1024;
//     // props.applicationName = "MyApp";
    
//     return new nkentseu::DemoParticles(props);
// }