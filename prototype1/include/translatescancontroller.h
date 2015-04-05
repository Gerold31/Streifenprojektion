#ifndef TRANSLATESCANCONTROLLER_H
#define TRANSLATESCANCONTROLLER_H

#include "controller.h"


class TranslateScanController : public Controller
{
public:
	TranslateScanController(double dx, double dy, double dz, int n);
	virtual ~TranslateScanController();

	virtual void main() override;

private:
	double dx;
	double dy;
	double dz;
	int n;
};

#endif // SCANCONTROLLER_H
