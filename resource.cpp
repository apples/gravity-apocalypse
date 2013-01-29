#include "resource.h"

#include "log.h"

#include <stdexcept>

Resource* resource;

Resource::Resource() :
    scores{0,0},
    core(nullptr),
    shader(nullptr)
{
    SLog slog(logger, "RESOURCE");

    Core::RenderParams rp;
    rp.width = 1024;
    rp.height = 768;
    rp.fullscreen = false;
    rp.vsync = false;
    rp.fsaaSamples = 0;

    logger->log<5>("Creating Core...");
    core = new Core(rp);
    core->setWindowTitle("Gravity Apocalypse", true);
    logger->log<5>("Done.");

    logger->log<5>("Creating shader...");
    Shader::Program pro;
    pro[Shader::Type::VERT] =
        "#version 400\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in vec3 VertexNormal;\n"
        "layout (location = 2) in vec2 VertexTexCoord;\n"
        "out vec3 Position;\n"
        "out vec3 Normal;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "    TexCoord = VertexTexCoord;\n"
        "    Normal = normalize(VertexNormal);\n"
        "    Position = VertexPosition/5;\n"
        "    gl_Position = vec4(VertexPosition/5,1.0);\n"
        "}\n"
    ;
    pro[Shader::Type::FRAG] =
        "#version 400\n"
        "in vec3 Position;\n"
        "in vec3 Normal;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D Tex1;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    vec4 texColor = texture(Tex1, interpolateAtCentroid(TexCoord));\n"
        "    FragColor = texColor;\n"
        "}\n"
    ;
    shader = new Shader(pro);
    shader->bind();
    logger->log<5>("Done.");

    logger->log<5>("Creating keybinds...");
    Interface* iface = core->getInterface();
    keybinds["player.1.up"] =   iface->getProxy('W'_ivk);
    keybinds["player.1.down"] = iface->getProxy('S'_ivk);
    keybinds["player.2.up"] =   iface->getProxy('U'_ivkArrow);
    keybinds["player.2.down"] = iface->getProxy('D'_ivkArrow);
    logger->log<5>("Done.");

    logger->log<5>("Loading spritesheets...");
    sheets["paddles"] = new Spritesheet("data/paddles.png", 8, 32);
    sheets["ball"] = new Spritesheet("data/ball.png", 8, 8);
    sheets["forces"] = new Spritesheet("data/forces.png", 32, 32);
    logger->log<5>("Done.");
}

Resource::~Resource()
{
    for (auto& p : sheets) delete p.second;
    delete shader;
    delete core;
}

Spritesheet& Resource::getSheet(const std::string& in)
{
    Spritesheet *& rval = sheets[in];
    if (!rval) throw std::logic_error("Resource: Spritesheet not found!");
    return *rval;
}

Interface::Proxy& Resource::getKeybind(const std::string& str)
{
    return keybinds[str];
}
