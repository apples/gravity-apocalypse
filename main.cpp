
#include "inugami/inugami.h"

#include <fstream>

#include "log.h"
#include "resource.h"
#include "field.h"

using Inugami::Core;
using Inugami::Interface;
using Inugami::Transform;
using Inugami::Vec3;

using Inugami::stringify;

void init();
void draw();
void tick();
void idle();

Core* core;
Interface* iface;

std::ofstream logFile("system.log");

int main()
{
    try
    {
        init();
        core->go();
    }
    catch (std::exception &e)
    {
        std::ofstream("error.txt") << e.what();
    }

    delete core;

    return 0;
}

Field* field;

void init()
{
    logger = new Log(logFile);
    SLog slog(logger, "INIT");

    logger->log<5>("Creating Resource...");
    resource = new Resource();
    logger->log<5>("Done.");

    core = resource->core;
    iface = core->getInterface();

    logger->log<5>("Registering callbacks...");
    core->addCallback(draw, 60.0);
    core->addCallback(tick, 60.0);
    core->addCallback(idle, -1.0);
    logger->log<5>("Done.");

    logger->log<5>("Creating field...");
    field = new Field();
    logger->log<5>("Done.");
}

void draw()
{
    core->beginFrame();

    core->applyCam(Camera().ortho(-128.f, 128.f, -96.f, 96.f, -1.f, 1.f));

    Transform mat;
    field->draw(mat);

    {
        static auto drawDivider = [](Transform mat)
        {
            static unsigned int anim = 0;
            ++anim;
            mat.translate(Vec3(-120.f, 88.f, 0.f));
            for (int i=0; i<16; ++i)
            {
                mat.push();
                mat.translate(Vec3{i*16.f, 0.f, 0.f});
                core->modelMatrix(mat);
                resource->getSheet("interface").draw(0,(anim/2+i)%16);
                mat.pop();
            }
        };
        mat.push();
        drawDivider(mat);
        mat.scale(Vec3{-1.f, -1.f, 1.f});
        drawDivider(mat);
        mat.pop();
    }

    {
        static auto drawNum = [](int num, Transform mat)
        {
            auto snum = stringify(num);
            mat.translate(Vec3{-((snum.length()-1)*4.0), 0.f, 0.f});
            for (char c : snum)
            {
                core->modelMatrix(mat);
                resource->getSheet("font").draw(c/16, c%16);
                mat.translate(Vec3{8.0, 0.f, 0.f});
            }
        };
        mat.push();
        mat.translate(Vec3{-64.f, 88.f, 0.f});
        drawNum(field->scores[0], mat);
        mat.pop();
        mat.push();
        mat.translate(Vec3{64.f, 88.f, 0.f});
        drawNum(field->scores[1], mat);
        mat.pop();
    }

    core->endFrame();
}

void tick()
{
    iface->poll();

    field->tick();
}

void idle()
{
    if (iface->keyDown(GLFW_KEY_ESCAPE) || core->closeRequested())
    {
        core->running = false;
    }
}
