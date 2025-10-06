#pragma once

#include <cstdint>  // Ajout de l'en-tête pour uint8_t et uint32_t

namespace nkentseu {
    namespace graphics {

        /**
         * @brief Classe de gestion des couleurs RGBA
         */
        class Color {
            public:
                union {
                    struct {
                        uint8_t r, g, b, a;
                    };
                    uint32_t value;
                };

                // Constructeurs
                Color() : r(0), g(0), b(0), a(255) {}
                Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
                Color(uint32_t hexValue) : value(hexValue) {}

                // Couleurs prédéfinies
                static const Color Black;
                static const Color White;
                static const Color Red;
                static const Color Green;
                static const Color Blue;
                static const Color Yellow;
                static const Color Magenta;
                static const Color Cyan;
                static const Color Transparent;

                // Opérateurs
                bool operator==(const Color& other) const { return value == other.value; }
                bool operator!=(const Color& other) const { return value != other.value; }
        };

    } // namespace graphics
} // namespace nkentseu