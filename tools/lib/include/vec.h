#ifndef VEC
#define VEC

#include <cmath>
#include <cstdint>


template<int dim, typename type_t>
class Vec final
{
public:
	Vec();

	type_t &operator[](const int &i);
	const type_t &operator[](const int &i) const;

	double norm() const;
	double norm2() const;

private:
	type_t v[dim];
};


template<int dim, typename t1_t, typename t2_t>
bool operator< (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2);

template<int dim, typename t1_t, typename t2_t>
bool operator> (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2);

template<int dim, typename t1_t, typename t2_t>
bool operator<= (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2);

template<int dim, typename t1_t, typename t2_t>
bool operator>= (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2);


typedef Vec<2, double> Vec2d;
typedef Vec<3, double> Vec3d;
typedef Vec<3, double> Vec4d;

typedef Vec<2, int> Vec2i;
typedef Vec<3, int> Vec3i;
typedef Vec<4, int> Vec4i;

typedef Vec<2, unsigned int> Vec2u;
typedef Vec<3, unsigned int> Vec3u;
typedef Vec<4, unsigned int> Vec4u;


template<int dim, typename type_t>
inline Vec<dim, type_t>::Vec()
{
	for (int i = 0; i < dim; ++i) {
		v[i] = (type_t) 0;
	}
}

template<int dim, typename type_t>
inline type_t &Vec<dim, type_t>::operator[](const int &i)
{
	return v[i];
}

template<int dim, typename type_t>
inline const type_t &Vec<dim, type_t>::operator[](const int &i) const
{
	return v[i];
}

template<int dim, typename type_t>
inline double Vec<dim, type_t>::norm() const
{
	return std::sqrt(norm2());
}

template<int dim, typename type_t>
inline double Vec<dim, type_t>::norm2() const
{
	double sum = 0;
	for (int i = 0; i < dim; ++i) {
		sum += v[i] * v[i];
	}
}


template<int dim, typename t1_t, typename t2_t>
inline bool operator< (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2)
{
	for (int i = 0; i < dim; ++i) {
		if (v1[i] >= v2[i])
			return false;
	}
	return true;
}

template<int dim, typename t1_t, typename t2_t>
inline bool operator> (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2)
{
	for (int i = 0; i < dim; ++i) {
		if (v1[i] <= v2[i])
			return false;
	}
	return true;
}

template<int dim, typename t1_t, typename t2_t>
inline bool operator<= (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2)
{
	for (int i = 0; i < dim; ++i) {
		if (v1[i] > v2[i])
			return false;
	}
	return true;
}

template<int dim, typename t1_t, typename t2_t>
inline bool operator>= (const Vec<dim, t1_t>& v1, const Vec<dim, t2_t>& v2)
{
	for (int i = 0; i < dim; ++i) {
		if (v1[i] < v2[i])
			return false;
	}
	return true;
}

#endif // VEC
