#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include "controller.h"


class CommandController : public Controller
{
public:
	CommandController();
	virtual ~CommandController();

	virtual void main() override;
};

#endif // COMMANDCONTROLLER_H
