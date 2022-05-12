#include <iostream>

#include "../tests/doctest.h"
#include "UIHandler.h"

/*
0 -> run unit tests
1 -> normal mode
*/
#define PROGRAM_MODE 1

int main()
{
#if (PROGRAM_MODE==0)
	doctest::Context().run();
#else
	UIHandler uih;
	uih.run();
#endif // (PROGRAM_MODE==1)
}