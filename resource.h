#ifndef RESOURCE_H
#define RESOURCE_H

#include "inugami/camera.h"
#include "inugami/core.h"
#include "inugami/interface.h"
#include "inugami/shader.h"
#include "inugami/spritesheet.h"

#include <map>
#include <string>

using Inugami::Camera;
using Inugami::Core;
using Inugami::Interface;
using Inugami::Shader;
using Inugami::Spritesheet;

class Resource final
{
public:
    Resource();
    ~Resource();

    Spritesheet& getSheet(const std::string& in);
    Interface::Proxy& getKeybind(const std::string& str);

    int scores[2];
    Core* core;

private:

    std::map<std::string, Spritesheet*> sheets;
    std::map<std::string, Interface::Proxy> keybinds;

    Shader* shader;
};

extern Resource* resource;

#endif // RESOURCE_H
