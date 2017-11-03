#pragma once
class Utils
{
public:
    Utils();
    ~Utils();

    static void DrawImage(const unsigned char* prgb, const int dim[2]);

    static float Clamp(float val, float low, float high);
};

