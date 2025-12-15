#pragma once
#include <functional> // pour std::hash
#include <cstddef>    // pour size_t

struct Coord {
    int x, y, z;

    // Constructeur constexpr pour calculs à la compilation si besoin
    constexpr Coord(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    // Opérateur d'égalité (requis pour unordered_map)
    bool operator==(const Coord& other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }

    // Opérateur d'inégalité
    bool operator!=(const Coord& other) const noexcept {
        return !(*this == other);
    }

    // Méthodes utilitaires (déplacées ici pour alléger Cite)
    Coord sud() const { return { x, y - 1, z }; }

    // Suggestion : Implémenter "cote" ici si la logique ne dépend pas de l'état de la Cité
    Coord cote(bool inversion) const;
};

// ==========================================
// SPÉCIALISATION DE STD::HASH
// ==========================================
// Cela permet d'écrire unordered_map<Coord, ...> sans 3ème argument
namespace std {
    template <>
    struct hash<Coord> {
        size_t operator()(const Coord& c) const noexcept {
            // Algorithme de hash inspiré de boost::hash_combine pour éviter les collisions
            size_t h1 = std::hash<int>{}(c.x);
            size_t h2 = std::hash<int>{}(c.y);
            size_t h3 = std::hash<int>{}(c.z);

            // Mélange des bits plus robuste que le simple XOR et décalage
            size_t seed = 0;
            auto hash_combine = [&seed](size_t v) {
                seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

            hash_combine(h1);
            hash_combine(h2);
            hash_combine(h3);
            return seed;
        }
    };
}