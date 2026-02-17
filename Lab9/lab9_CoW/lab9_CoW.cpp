#include "Image.h"
#include "ImageUtils.h"
#include "Drawer.h"

#include <iostream>

static void RunSolve()
{
    // чёрный холст
    auto img = ImportImage("sample_black.ppm");

    //DrawLine(img, { 0, 0 }, { img.GetImageSize().width, img.GetImageSize().height }, 0xFF0000);
    //DrawLine(img, { img.GetImageSize().width, 0 }, { 0, img.GetImageSize().height }, 0xFF0000);
    FillCircle(img, { 7, 5 }, 4, 0xFF0000);
    DrawCircle(img, { 7, 5 }, 4, 0xFFFFFF);

    SaveImage(img, "dst.ppm");
    std::cout << "Image saved\n";
}

int main()
{
    RunSolve();
}