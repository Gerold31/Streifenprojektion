#include "configuration.h"
#include "controller.h"


int main_app(int argc, char *argv[])
{
	Configuration::init(argc, argv);
	Configuration::controller->main();
	return EXIT_SUCCESS;
}

#ifndef MAINLESS
int main(int argc, char *argv[])
{
	main_app(argc, argv);
}
#endif // MAINLESS
