#pragma once
#include <cmath>

namespace nkentseu {
    namespace math {

        /**
         * @brief Classe de vecteur 2D pour la gestion des positions, dimensions, etc.
         */
        class Vector2 {
            public:
                // Constructeurs
                Vector2() : x(0.0f), y(0.0f) {}
                Vector2(float value) : x(value), y(value) {}
                Vector2(float x, float y) : x(x), y(y) {}

                // Opérateurs arithmétiques
                Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
                Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
                Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
                Vector2 operator/(const Vector2& other) const { return Vector2(x / other.x, y / other.y); }

                Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
                Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

                // Opérateurs d'affectation
                Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
                Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
                Vector2& operator*=(const Vector2& other) { x *= other.x; y *= other.y; return *this; }
                Vector2& operator/=(const Vector2& other) { x /= other.x; y /= other.y; return *this; }

                // Opérateurs de comparaison
                bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
                bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }

                // Méthodes utilitaires
                float Magnitude() const { return std::sqrt(x * x + y * y); }
                Vector2 Normalized() const { float mag = Magnitude(); return mag > 0.0f ? *this / mag : Zero; }
                float Distance(const Vector2& other) const { return (*this - other).Magnitude(); }
                float Dot(const Vector2& other) const { return x * other.x + y * other.y; }

                // Accès flexible aux données
                union {
                    struct { float x, y; };
                    struct { float width, height; };
                    struct { float u, v; };
                    float data[2];
                };

                // Constantes statiques
                static const Vector2 Zero;
                static const Vector2 One;
                static const Vector2 Up;
                static const Vector2 Down;
                static const Vector2 Left;
                static const Vector2 Right;
        };

    } // namespace math
} // namespace nkentseu