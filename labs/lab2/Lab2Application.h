#ifndef PROG2002_LAB2APPLICATION_H
#define PROG2002_LAB2APPLICATION_H

#include <string>
#include "GLFWApplication.h"

class Lab2Application : public GLFWApplication{
    public:
        Lab2Application(const std::string &name, const std::string &version);
        ~Lab2Application();

        unsigned Init() override;
        unsigned Run() const override;
        unsigned stop() override;
};


#endif //PROG2002_LAB2APPLICATION_H
