#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>

class Plot3D;


class Drawable
{
	friend class Plot3D;
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	virtual void draw(const glm::mat4 &view) = 0;

private:
	bool hide = false;
};

#endif // DRAWABLE_H
