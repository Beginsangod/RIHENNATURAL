#pragma once
#include "Vector2.h"

namespace nkentseu {
    namespace math {
        class Matrix3x3 {
            public:
                float m[3][3];
                
                Matrix3x3();
                // Matrix3x3(const Matrix3x3& other);
                
                // Matrice identité
                static Matrix3x3 Identity();
                
                // Matrices de transformation
                static Matrix3x3 Translation(float x, float y);
                static Matrix3x3 Rotation(float angle);
                static Matrix3x3 Scaling(float sx, float sy);
                
                // Opérations
                Matrix3x3 operator*(const Matrix3x3& other) const;
                Vector2 TransformPoint(const Vector2& point) const;
                Vector2 TransformVector(const Vector2& vector) const;
                
                // Méthodes utilitaires
                Matrix3x3 Inverse() const;
                Matrix3x3 Transpose() const;
                float Determinant() const;
        };
    }
}