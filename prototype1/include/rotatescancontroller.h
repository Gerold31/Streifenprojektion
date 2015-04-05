#ifndef ROTATESCANCONTROLLER_H
#define ROTATESCANCONTROLLER_H

#include "controller.h"


class RotateScanController : public Controller
{
public:
	RotateScanController();
	virtual ~RotateScanController();

	virtual void main() override;
};

#endif // SCANCONTROLLER_H
