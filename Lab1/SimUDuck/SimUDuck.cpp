#include <iostream>

#include "Duck.h"
#include "MallardDuck.h"
#include "RedHeadDuck.h"
#include "RubberDuck.h"
#include "ModelDuck.h"
#include "DecoyDuck.h"
#include "DuckFunctions.h"

void RunDuck()
{
	MallardDuck mallardDuck{};
	PlayWithDuck(mallardDuck);

	RedHeadDuck redHeadDuck{};
	PlayWithDuck(redHeadDuck);

	RubberDuck rubberDuck{};
	PlayWithDuck(rubberDuck);

	ModelDuck modelDuck{};
	PlayWithDuck(modelDuck);

	DecoyDuck decoyDuck{};
	PlayWithDuck(decoyDuck);

	MallardDuck newDuck{};
	for (int k = 5; k--;) newDuck.Fly();
}

int main()
{
	RunDuck();
	
	return EXIT_SUCCESS;
}
