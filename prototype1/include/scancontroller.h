#ifndef SCANCONTROLLER_H
#define SCANCONTROLLER_H

#include "controller.h"


class ScanController : public Controller
{
public:
	ScanController();
	virtual ~ScanController();

	virtual void main() override;
};

#endif // SCANCONTROLLER_H
