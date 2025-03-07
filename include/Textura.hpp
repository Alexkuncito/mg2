#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#pragma once

#include <glad.h>
#include <string>
#include <iostream>

class Textura {
public:
    unsigned int ID;

    Textura(const std::string& path);
    void bind() const;
    void unbind() const;
    ~Textura();
};

#endif
