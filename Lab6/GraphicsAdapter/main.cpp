// 6 лр - адаптер
// сел за лабу 18.11.2025 в 13:40
// закончил первое задание 19.11.2025 - 0:30
// сел за второе задание 19.11.2025 - 13:10
// закончил второе задание 19.11.2025 - 15:50 (перерыв на час по деканатам был)
// сел за третье задание 19.11.2025 - 16:10
// закончил третье задание 19.11.2025 - 17:45 (перерыв на 20 мин помыть посуду)

#include <string>

#include "App.h"

void RunPaint()
{
	std::cout << "Should we use new API (y)?";
	std::string userInput;
	if (std::getline(std::cin, userInput) && (userInput == "Y" || userInput == "y"))
	{
		app::PaintPictureOnModernGraphicsRenderer();
	}
	else
	{
		app::PaintPictureOnCanvas();
	}
}

int main()
{
	RunPaint();
}