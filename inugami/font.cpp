#include "font.h"

namespace Inugami {

Font::Font(const std::string &filename, unsigned int w, unsigned int h, float cx, float cy) :
    Spritesheet(filename, w, h, cx, cy),
    w(w), h(-float(h))
{}

Font::~Font()
{}

void Font::draw(const std::string& in)
{
    glPushMatrix();

    for (char i : in)
    {
        if (i == '\n')
        {
            glPopMatrix();
            glTranslatef(0.0f, h, 0.0f);
            glPushMatrix();
            continue;
        }

        int r = i/16;
        int c = i%16;
        Spritesheet::draw(r, c);
        glTranslatef(w, 0.0f, 0.0f);
    }

    glPopMatrix();
}

} // namespace Inugami
