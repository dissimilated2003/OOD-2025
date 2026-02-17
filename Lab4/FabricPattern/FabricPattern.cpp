#include "Canvas.h"
#include "Point.h"
#include "Color.h"
#include "IShape.h"
#include "ShapeFactory.h"
#include "Designer.h"
#include "Painter.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <memory>

static int RunPictureRender()
{
    try
    {
        std::ifstream inputfile{ "picture.txt" };
        if (!inputfile.is_open())
        {
            throw std::runtime_error("Failed to open input file");
        }
        auto factory = std::make_unique<ShapeFactory>();
        Designer designer(std::move(factory));
        PictureDraft draft = designer.CreateDraft(inputfile);
        inputfile.close();

        sf::RenderWindow window(sf::VideoMode(800, 600), L"Осмысленное нагромождение фигур");
        Canvas canvas{ window };
        window.clear(sf::Color::White);
        Painter::DrawPicture(draft, canvas);
        
        while (window.isOpen())
        {
            sf::Event event{};
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) window.close();
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main()
{
    return RunPictureRender();
}

