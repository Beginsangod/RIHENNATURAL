#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <stack> // Ajouter cette inclusion
#include "Math/Vector2.h"
#include "Math/Matrix3x3.h"
#include "Core/Events/Mouse.h"
#include "Core/Events/Keyboard.h"
#include "Graphics/Color.h"
#include "Log.h"
#include "ApplicationProperties.h"

namespace nkentseu {

    // Alias pour les arguments
    using Args = std::vector<std::string>;

    // Déclaration forward
    class Application;

    /**
     * @brief Fonction principale définie par l'utilisateur
     * @param args Arguments de la ligne de commande
     * @return Pointeur vers l'application créée
     */
    Application* Main(Args args);

    /**
     * @brief Classe principale de l'application RihenNatural
     */
    class Application {
        private:
            SDL_Window* mWindow = nullptr;
            SDL_Renderer* mRenderer = nullptr;
            bool mIsRunning = false;
            bool mIsInitialized = false;
            
            graphics::Color mBgFillColor = graphics::Color::Black;
            graphics::Color mFillColor = graphics::Color::White;
            graphics::Color mStrokeColor = graphics::Color::Black;
            bool mUseFill = true;
            bool mUseStroke = true;
            float mStrokeWeight = 1.0f;
            
            ApplicationProperties mProperties;

            // Matrice de transformation courante
            math::Matrix3x3 mTransformMatrix;
            std::stack<math::Matrix3x3> mTransformStack;

            // Méthodes internes
            void ProcessEvents();
            void UpdateInput();

        protected:
            // Méthodes protégées pour l'utilisateur
            void SetWindowSize(int width, int height);
            void SetWindowTitle(const char* title);

        public:
            Application(const ApplicationProperties& props = ApplicationProperties());
            virtual ~Application();

            // Méthodes d'initialisation système
            bool Initialize();
            void Close();

            // Méthodes principales
            void Run();
            void Exit();

            // Méthodes virtuelles à override par l'utilisateur
            virtual void Setup() {}              // Initialisation utilisateur
            virtual void Update(float deltaTime) {(void)deltaTime;} // Logique de frame
            virtual void Draw() {}               // Rendu

            // Méthodes graphiques
            void Background(const graphics::Color& color, bool useInternalClear = false);
            void Fill(const graphics::Color& color);
            void Stroke(const graphics::Color& color);
            void NoFill();
            void NoStroke();
            void StrokeWeight(float weight);
            float GetStrokeWeight() const { return mStrokeWeight; }
            void ThickLine(const math::Vector2& start, const math::Vector2& end, float thickness);
            void DrawCircleInternal(const math::Vector2& center, float radius);
            bool IsPointInQuadrilateral(const math::Vector2& point, const math::Vector2 quad[4]);
            
            void Circle(const math::Vector2& center, float radius);
            void Ellipse(const math::Vector2& center, float radiusX, float radiusY);
            void Rect(const math::Vector2& position, const math::Vector2& size);
            void Line(const math::Vector2& start, const math::Vector2& end);
            void Polygon(const std::vector<math::Vector2>& vertices, bool filled = true);
            void Triangle(const math::Vector2& p1, const math::Vector2& p2, const math::Vector2& p3);
            void Quad(const math::Vector2& p1, const math::Vector2& p2, const math::Vector2& p3, const math::Vector2& p4);

            // Dans la classe Application, ajoutez ces méthodes publiques :

            /**
             * @brief Dessine un arc de cercle
             * @param center Centre de l'arc
             * @param radius Rayon de l'arc
             * @param startAngle Angle de départ en radians
             * @param endAngle Angle de fin en radians
             * @param mode Mode de dessin (0: ouvert, 1: fermé, 2: en forme de tarte)
             */
            void Arc(const math::Vector2& center, float radius, 
                    float startAngle, float endAngle, int mode = 0);

            /**
             * @brief Dessine un arc de cercle avec épaisseur
             * @param center Centre de l'arc
             * @param radius Rayon de l'arc
             * @param startAngle Angle de départ en radians
             * @param endAngle Angle de fin en radians
             * @param thickness Épaisseur de l'arc
             */
            void ThickArc(const math::Vector2& center, float radius,
                        float startAngle, float endAngle, float thickness);

            /**
             * @brief Dessine un arc elliptique
             * @param center Centre de l'arc
             * @param radiusX Rayon horizontal
             * @param radiusY Rayon vertical
             * @param startAngle Angle de départ en radians
             * @param endAngle Angle de fin en radians
             * @param rotation Rotation de l'ellipse en radians
             */
            void EllipticalArc(const math::Vector2& center, float radiusX, float radiusY,
                            float startAngle, float endAngle, float rotation = 0.0f);

            /**
             * @brief Dessine un cercle avec un trou (anneau)
             * @param center Centre du cercle
             * @param innerRadius Rayon intérieur
             * @param outerRadius Rayon extérieur
             */
            void Ring(const math::Vector2& center, float innerRadius, float outerRadius);

            /**
             * @brief Dessine un secteur circulaire (forme de tarte)
             * @param center Centre du secteur
             * @param radius Rayon du secteur
             * @param startAngle Angle de départ en radians
             * @param endAngle Angle de fin en radians
             */
            void Pie(const math::Vector2& center, float radius,
                    float startAngle, float endAngle);

            // Dans la classe Application, ajoutez cette méthode publique :
            // void Text(const std::string& text, const math::Vector2& position, 
            //         const graphics::Color& color = graphics::Color::White, 
            //         int fontSize = 16);

            // Getters
            math::Vector2 GetMousePosition() const;
            bool IsKeyPressed(events::KeyCode key) const;
            bool IsMouseButtonPressed(events::MouseButton button) const;
            int GetWindowWidth() const { return mProperties.windowWidth; }
            int GetWindowHeight() const { return mProperties.windowHeight; }
            
            // Fonction utilitaire
            bool IsPointInPolygon(const math::Vector2& point, const std::vector<math::Vector2>& vertices);

            // Méthodes de transformation
            void PushMatrix();
            void PopMatrix();
            void ResetMatrix();
            
            void Translate(float x, float y);
            void Translate(const math::Vector2& translation);
            
            void Rotate(float angle); // angle en radians
            void RotateDegrees(float degrees);
            
            void Scale(float sx, float sy);
            void Scale(float scale);
            void Scale(const math::Vector2& scale);
            
            // Getters
            math::Matrix3x3 GetCurrentMatrix() const;
            math::Vector2 TransformPoint(const math::Vector2& point) const;

            ApplicationProperties GetProperty() const {
                return mProperties;
            }
            
            // Méthodes utilitaires pour le développeur
            static ApplicationProperties CreatePropertiesFromArgs(const Args& args) {
                return ApplicationProperties::FromCommandLineAndConfig(args);
            }
            
            static ApplicationProperties CreatePropertiesFromConfig(const std::string& configPath) {
                return ApplicationProperties::FromConfigFile(configPath);
            }
            
            static ApplicationProperties CreateDefaultProperties() {
                return ApplicationProperties();
            }
            
            friend Application* ::nkentseu::Main(Args args);
    };

} // namespace nkentseu