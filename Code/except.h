#pragma once
#include <exception>
#include <string>

// Classe de base pour toutes les erreurs du jeu
class AkropolisException : public std::exception {
protected:
    std::string info;

public:
    AkropolisException(const std::string& i) : info(i) {}
    virtual ~AkropolisException() = default;

    const char* what() const noexcept override {
        return info.c_str();
    }
};