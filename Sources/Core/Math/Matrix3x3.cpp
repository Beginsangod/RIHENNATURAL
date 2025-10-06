#include "Matrix3x3.h"
#include <cmath>
#include <cstring>

namespace nkentseu {
    namespace math {
        Matrix3x3::Matrix3x3() {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        // Matrix3x3::Matrix3x3(const Matrix3x3& other) {
        //     std::memcpy(m, other.m, sizeof(m));
        // }
        
        Matrix3x3 Matrix3x3::Identity() {
            return Matrix3x3();
        }
        
        Matrix3x3 Matrix3x3::Translation(float x, float y) {
            Matrix3x3 mat = Identity();
            mat.m[0][2] = x;
            mat.m[1][2] = y;
            return mat;
        }
        
        Matrix3x3 Matrix3x3::Rotation(float angle) {
            Matrix3x3 mat = Identity();
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            
            mat.m[0][0] = cosA;
            mat.m[0][1] = -sinA;
            mat.m[1][0] = sinA;
            mat.m[1][1] = cosA;
            
            return mat;
        }
        
        Matrix3x3 Matrix3x3::Scaling(float sx, float sy) {
            Matrix3x3 mat = Identity();
            mat.m[0][0] = sx;
            mat.m[1][1] = sy;
            return mat;
        }
        
        Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other) const {
            Matrix3x3 result;
            
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    result.m[i][j] = 0.0f;
                    for (int k = 0; k < 3; k++) {
                        result.m[i][j] += m[i][k] * other.m[k][j];
                    }
                }
            }
            
            return result;
        }
        
        Vector2 Matrix3x3::TransformPoint(const Vector2& point) const {
            float x = m[0][0] * point.x + m[0][1] * point.y + m[0][2];
            float y = m[1][0] * point.x + m[1][1] * point.y + m[1][2];
            float w = m[2][0] * point.x + m[2][1] * point.y + m[2][2];
            
            if (w != 0.0f && w != 1.0f) {
                x /= w;
                y /= w;
            }
            
            return Vector2(x, y);
        }
        
        Vector2 Matrix3x3::TransformVector(const Vector2& vector) const {
            float x = m[0][0] * vector.x + m[0][1] * vector.y;
            float y = m[1][0] * vector.x + m[1][1] * vector.y;
            
            return Vector2(x, y);
        }
    }
}