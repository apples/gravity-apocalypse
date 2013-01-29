#ifndef INUGAMI_FONT_H
#define INUGAMI_FONT_H

#include "spritesheet.h"

#include <string>

namespace Inugami {

class Font : public Spritesheet
{
public:
    Font(const std::string &filename, unsigned int w, unsigned int h, float cx = 0.5f, float cy = 0.5f);
    virtual ~Font();

    void draw(const std::string& in);

private:
    const float w,h;
};

} // namespace Inugami

#endif // INUGAMI_FONT_H
