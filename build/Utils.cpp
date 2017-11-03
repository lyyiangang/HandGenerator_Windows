#include "Utils.h"
#include "GL\glew.h"
#include <vector>



Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::DrawImage(const unsigned char* prgb, const int dim[])
{
    if (prgb ==nullptr)
        return;
    float p[2] = { 0, 1.0f }, s[2] = { 1.0f, -1.0f };
    int tid = 0;
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim[0],dim[1], 0, GL_RGB, GL_UNSIGNED_BYTE, prgb);

    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    std::vector<std::pair<float,float>> corners= { { 0.0f, 0.0f },{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };
    for (auto& c : corners)
    {
        glTexCoord2f(c.first, c.second);
        float tmpPos[3] = { p[0] + c.first * s[0], p[1] + c.second * s[1],0.0f };
        glVertex3fv(tmpPos);
    }
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

float Utils::Clamp(float val, float low, float high)
{
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

