#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>


class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	virtual void draw(const glm::mat4 &view) = 0;
};

#endif // DRAWABLE_H
