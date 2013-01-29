
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
    SLog slog(logger, "DRAW");

    logger->log<10>("Frame start.");
    core->beginFrame();

    logger->log<10>("Setting camera...");
    core->applyCam(Camera().ortho(-128.f, 128.f, -96.f, 96.f, -1.f, 1.f));

    logger->log<10>("Drawing paddles...");
    Transform mat;
    field->draw(mat);

    core->endFrame();
    logger->log<10>("Frame end.");
}

void tick()
{
    SLog slog(logger, "TICK");

    logger->log<10>("Polling interface...");
    iface->poll();

    logger->log<10>("Ticking paddles...");
    field->tick();

    core->setWindowTitle((stringify(field->scores[0])+":"+stringify(field->scores[1])).c_str(), true);
}

void idle()
{
    if (iface->keyDown(GLFW_KEY_ESC) || core->getWindowParam(GLFW_CLOSE_REQUESTED))
    {
        core->running = false;
    }
}
