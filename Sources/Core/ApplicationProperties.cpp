#include "ApplicationProperties.h"
#include <filesystem>

namespace nkentseu {

    ApplicationProperties ApplicationProperties::FromCommandLine(const std::vector<std::string>& args) {
        ApplicationProperties props;
        props.commandLineArgs = args;
        
        if (args.empty()) {
            return props;
        }
        
        // Parser les arguments de ligne de commande
        for (size_t i = 1; i < args.size(); ++i) {
            const std::string& arg = args[i];
            
            if (arg == "--width" && i + 1 < args.size()) {
                try {
                    props.windowWidth = std::stoi(args[++i]);
                } catch (...) {
                    Log.Warning("Invalid width value: {0}", args[i]);
                }
            }
            else if (arg == "--height" && i + 1 < args.size()) {
                try {
                    props.windowHeight = std::stoi(args[++i]);
                } catch (...) {
                    Log.Warning("Invalid height value: {0}", args[i]);
                }
            }
            else if (arg == "--title" && i + 1 < args.size()) {
                props.windowTitle = args[++i];
            }
            else if (arg == "--fullscreen") {
                props.fullscreen = true;
            }
            else if (arg == "--resizable") {
                props.resizable = true;
            }
            else if (arg == "--borderless") {
                props.borderless = true;
            }
            else if (arg == "--vsync") {
                props.vsync = true;
            }
            else if (arg == "--no-vsync") {
                props.vsync = false;
            }
            else if (arg == "--fps" && i + 1 < args.size()) {
                try {
                    props.targetFPS = std::stoi(args[++i]);
                } catch (...) {
                    Log.Warning("Invalid FPS value: {0}", args[i]);
                }
            }
            else if (arg == "--name" && i + 1 < args.size()) {
                props.applicationName = args[++i];
            }
            else if (arg == "--org" && i + 1 < args.size()) {
                props.organizationName = args[++i];
            }
            else if (arg == "--icon" && i + 1 < args.size()) {
                props.iconPath = args[++i];
            }
            else if (arg == "--font" && i + 1 < args.size()) {
                props.fontPath = args[++i];
            }
            else if (arg == "--config" && i + 1 < args.size()) {
                // On ignore ici, car c'est géré dans FromCommandLineAndConfig
                i++;
            }
            else if (arg == "--pos-x" && i + 1 < args.size()) {
                try {
                    props.windowPosition.x = std::stof(args[++i]);
                } catch (...) {
                    Log.Warning("Invalid position X value: {0}", args[i]);
                }
            }
            else if (arg == "--pos-y" && i + 1 < args.size()) {
                try {
                    props.windowPosition.y = std::stof(args[++i]);
                } catch (...) {
                    Log.Warning("Invalid position Y value: {0}", args[i]);
                }
            }
            else if (arg == "--high-dpi") {
                props.highDPI = true;
            }
            else if (arg == "--demo") {
                props.defaultDemo = args[++i];
            }
            else {
                Log.Warning("Unknown command line argument: {0}", arg);
            }
        }
        
        return props;
    }

    ApplicationProperties ApplicationProperties::FromConfigFile(const std::string& configPath) {
        ApplicationProperties props;
        
        if (configPath.empty()) {
            Log.Warning("Empty config file path provided");
            return props;
        }
        
        std::ifstream configFile(configPath);
        if (!configFile.is_open()) {
            Log.Warning("Could not open config file: {0}", configPath);
            return props;
        }
        
        Log.Info("Loading configuration from: {0}", configPath);
        
        std::string line;
        int lineNumber = 0;
        (void)lineNumber; // Pour éviter le warning
        
        while (std::getline(configFile, line)) {
            lineNumber++;
            
            // Ignorer les lignes vides et les commentaires
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            ParseConfigLine(props, line);
        }
        
        configFile.close();
        Log.Info("Configuration loaded successfully from {0}", configPath);
        
        return props;
    }

    ApplicationProperties ApplicationProperties::FromCommandLineAndConfig(const std::vector<std::string>& args) {
        ApplicationProperties configProps;
        std::string configPath;
        
        // Chercher le chemin du fichier de config dans les arguments
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "--config" && i + 1 < args.size()) {
                configPath = args[i + 1];
                break;
            }
        }
        
        // Charger la configuration depuis le fichier si spécifié
        if (!configPath.empty()) {
            configProps = FromConfigFile(configPath);
        }
        
        // Appliquer les arguments de ligne de commande (qui écrasent la config)
        ApplicationProperties cmdProps = FromCommandLine(args);
        
        // Combiner les propriétés (les arguments CLI ont priorité)
        ApplicationProperties finalProps = configProps;
        finalProps.commandLineArgs = args;
        
        // Override avec les valeurs de la ligne de commande
        if (cmdProps.windowWidth != 800) finalProps.windowWidth = cmdProps.windowWidth;
        if (cmdProps.windowHeight != 600) finalProps.windowHeight = cmdProps.windowHeight;
        if (cmdProps.windowTitle != "RihenNatural Application") finalProps.windowTitle = cmdProps.windowTitle;
        if (cmdProps.windowPosition.x != WINDOWPOS_CENTERED) finalProps.windowPosition.x = cmdProps.windowPosition.x;
        if (cmdProps.windowPosition.y != WINDOWPOS_CENTERED) finalProps.windowPosition.y = cmdProps.windowPosition.y;
        if (cmdProps.resizable) finalProps.resizable = true;
        if (cmdProps.fullscreen) finalProps.fullscreen = true;
        if (cmdProps.borderless) finalProps.borderless = true;
        if (cmdProps.highDPI) finalProps.highDPI = true;
        if (cmdProps.applicationName != "RihenNaturalApp") finalProps.applicationName = cmdProps.applicationName;
        if (cmdProps.organizationName != "RihenNatural") finalProps.organizationName = cmdProps.organizationName;
        if (!cmdProps.vsync) finalProps.vsync = false;
        if (cmdProps.targetFPS != 60) finalProps.targetFPS = cmdProps.targetFPS;
        if (!cmdProps.iconPath.empty()) finalProps.iconPath = cmdProps.iconPath;
        if (!cmdProps.fontPath.empty()) finalProps.fontPath = cmdProps.fontPath;
        if (cmdProps.defaultDemo != "ecosystem") finalProps.defaultDemo = cmdProps.defaultDemo;
        
        return finalProps;
    }

    void ApplicationProperties::ParseConfigLine(ApplicationProperties& props, const std::string& line) {
        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) {
            Log.Warning("Invalid config line format: {0}", line);
            return;
        }
        
        std::string key = line.substr(0, equalsPos);
        std::string value = line.substr(equalsPos + 1);
        
        // Convertir la clé en minuscules pour case-insensitive
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        
        try {
            if (key == "windowtitle") {
                props.windowTitle = value;
            }
            else if (key == "windowwidth") {
                props.windowWidth = std::stoi(value);
            }
            else if (key == "windowheight") {
                props.windowHeight = std::stoi(value);
            }
            else if (key == "windowposx") {
                props.windowPosition.x = std::stof(value);
            }
            else if (key == "windowposy") {
                props.windowPosition.y = std::stof(value);
            }
            else if (key == "resizable") {
                props.resizable = (value == "true" || value == "1");
            }
            else if (key == "fullscreen") {
                props.fullscreen = (value == "true" || value == "1");
            }
            else if (key == "borderless") {
                props.borderless = (value == "true" || value == "1");
            }
            else if (key == "highdpi") {
                props.highDPI = (value == "true" || value == "1");
            }
            else if (key == "applicationname") {
                props.applicationName = value;
            }
            else if (key == "organizationname") {
                props.organizationName = value;
            }
            else if (key == "vsync") {
                props.vsync = (value == "true" || value == "1");
            }
            else if (key == "targetfps") {
                props.targetFPS = std::stoi(value);
            }
            else if (key == "iconpath") {
                props.iconPath = value;
            }
            else if (key == "fontpath") {
                props.fontPath = value;
            }
            else if (key == "defaultdemo") {
                // Cette clé est utilisée dans Main.cpp, pas dans ApplicationProperties
                // On l'ignore ici car elle n'est pas pertinente pour les propriétés de l'application
                props.defaultDemo = value;
            }
            else {
                Log.Warning("Unknown config key: {0}", key);
            }
        }
        catch (const std::exception& e) {
            Log.Warning("Error parsing config value for {0}: {1}", key, e.what());
        }
    }

} // namespace nkentseu