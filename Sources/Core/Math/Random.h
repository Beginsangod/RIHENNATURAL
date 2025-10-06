#pragma once
#include <random>
#include <type_traits>
#include <stdexcept> // Pour std::out_of_range
#include <algorithm> // Pour std::shuffle
#include "Vector2.h"
#include "Graphics/Color.h"

namespace nkentseu {
    namespace math {
        class Random {
            private:
                static std::mt19937 sGenerator;
                static bool sSeeded;
                
            public:
                // Initialisation
                static void Seed(unsigned int seed = std::random_device{}());
                
                // Méthode Range unique avec if constexpr
                template<typename T>
                static T Range(T min, T max) {
                    if (!sSeeded) Seed();
                    
                    if constexpr (std::is_integral_v<T>) {
                        std::uniform_int_distribution<T> dist(min, max);
                        return dist(sGenerator);
                    } else {
                        std::uniform_real_distribution<T> dist(min, max);
                        return dist(sGenerator);
                    }
                }
                
                // Booléen
                static bool Bool(float probability = 0.5f);
                
                // Valeur entre 0.0 et 1.0
                static float Value();
                
                // Angle aléatoire en radians
                static float Angle();
                
                // Angle aléatoire en degrés
                static float AngleDegrees();
                
                // Vecteur aléatoire
                static Vector2 Vector(float minLength = 0.0f, float maxLength = 1.0f);
                static Vector2 VectorInCircle(float radius = 1.0f);
                static Vector2 VectorOnCircle(float radius = 1.0f);
                
                // Couleur aléatoire
                static graphics::Color Color(bool withAlpha = false);
                static graphics::Color Color(const graphics::Color& min, const graphics::Color& max);
                
                // Sélection aléatoire dans un conteneur
                template<typename Container>
                static auto Choice(const Container& container) -> typename Container::value_type {
                    if (container.empty()) {
                        throw std::out_of_range("Random::Choice: container is empty");
                    }
                    auto it = container.begin();
                    std::advance(it, Range<size_t>(0, container.size() - 1));
                    return *it;
                }
                
                // Mélanger un conteneur
                template<typename Container>
                static void Shuffle(Container& container) {
                    if (!sSeeded) Seed();
                    std::shuffle(container.begin(), container.end(), sGenerator);
                }
                
                // Distribution normale (gaussienne)
                template<typename T>
                static T Gaussian(T mean = 0.0, T stddev = 1.0) {
                    if (!sSeeded) Seed();
                    std::normal_distribution<T> dist(mean, stddev);
                    return dist(sGenerator);
                }
                
                // Fonction de bruit Perlin (implémentation simplifiée)
                static float Noise(float x, float y = 0.0f, float z = 0.0f);
                
                // Fonction de mapping
                template<typename T>
                static T Map(T value, T inMin, T inMax, T outMin, T outMax) {
                    return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
                }
        
                // Fonctions helper pour le bruit Perlin
                static float Fade(float t);
                static float Lerp(float a, float b, float t);
                static float Grad(int hash, float x, float y, float z);
        };
    }
}