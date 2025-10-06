#include "Random.h"
#include "MathConstants.h"

namespace nkentseu {
    namespace math {
        std::mt19937 Random::sGenerator;
        bool Random::sSeeded = false;
        
        void Random::Seed(unsigned int seed) {
            sGenerator.seed(seed);
            sSeeded = true;
        }
        
        bool Random::Bool(float probability) {
            if (!sSeeded) Seed();
            std::bernoulli_distribution dist(probability);
            return dist(sGenerator);
        }
        
        float Random::Value() {
            return Range(0.0f, 1.0f);
        }
        
        float Random::Angle() {
            return Range(0.0f, TWO_PI);
        }
        
        float Random::AngleDegrees() {
            return Range(0.0f, 360.0f);
        }
        
        Vector2 Random::Vector(float minLength, float maxLength) {
            float angle = Angle();
            float length = Range(minLength, maxLength);
            return Vector2(std::cos(angle) * length, std::sin(angle) * length);
        }
        
        Vector2 Random::VectorInCircle(float radius) {
            return Vector(0.0f, radius) * std::sqrt(Value());
        }
        
        Vector2 Random::VectorOnCircle(float radius) {
            float angle = Angle();
            return Vector2(std::cos(angle) * radius, std::sin(angle) * radius);
        }
        
        graphics::Color Random::Color(bool withAlpha) {
            return graphics::Color(
                Range(0, 255),
                Range(0, 255),
                Range(0, 255),
                withAlpha ? Range(0, 255) : 255
            );
        }
        
        graphics::Color Random::Color(const graphics::Color& min, const graphics::Color& max) {
            return graphics::Color(
                Range(min.r, max.r),
                Range(min.g, max.g),
                Range(min.b, max.b),
                Range(min.a, max.a)
            );
        }

        // Implémentation simplifiée de bruit Perlin
        float Random::Noise(float x, float y, float z) {
            // Implémentation simplifiée de bruit value noise
            // Pour une implémentation plus complète, voir: https://github.com/sol-prog/Perlin_Noise
            
            // Utiliser le générateur pour créer un bruit cohérent
            static auto hash = [](int n) {
                n = (n << 13) ^ n;
                return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
            };
            
            int xi = static_cast<int>(std::floor(x)) & 255;
            int yi = static_cast<int>(std::floor(y)) & 255;
            int zi = static_cast<int>(std::floor(z)) & 255;
            
            float xf = x - std::floor(x);
            float yf = y - std::floor(y);
            float zf = z - std::floor(z);
            
            float u = Fade(xf);
            float v = Fade(yf);
            float w = Fade(zf);
            
            int a = hash(xi) + yi;
            int aa = hash(a) + zi;
            int ab = hash(a + 1) + zi;
            int b = hash(xi + 1) + yi;
            int ba = hash(b) + zi;
            int bb = hash(b + 1) + zi;
            
            float x1 = Lerp(Grad(hash(aa), xf, yf, zf),
                          Grad(hash(ba), xf - 1, yf, zf), u);
            float x2 = Lerp(Grad(hash(ab), xf, yf - 1, zf),
                          Grad(hash(bb), xf - 1, yf - 1, zf), u);
            float y1 = Lerp(x1, x2, v);
            
            x1 = Lerp(Grad(hash(aa + 1), xf, yf, zf - 1),
                     Grad(hash(ba + 1), xf - 1, yf, zf - 1), u);
            x2 = Lerp(Grad(hash(ab + 1), xf, yf - 1, zf - 1),
                     Grad(hash(bb + 1), xf - 1, yf - 1, zf - 1), u);
            float y2 = Lerp(x1, x2, v);
            
            return (Lerp(y1, y2, w) + 1.0f) / 2.0f;
        }
        
        // Fonctions helper pour le bruit Perlin
        float Random::Fade(float t) {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }
        
        float Random::Lerp(float a, float b, float t) {
            return a + t * (b - a);
        }
        
        float Random::Grad(int hash, float x, float y, float z) {
            int h = hash & 15;
            float u = h < 8 ? x : y;
            float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }
    }
}