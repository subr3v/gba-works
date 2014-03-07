#include <stdint.h>

#define fixedMul(a,b) ( (a * b) >> DECIMAL_BIT )
#define fixedDiv(a,b) ( (int32_t)((((int64_t)a) << DECIMAL_BIT) / b) )

struct FixedPoint
{
	int32_t value;

	FixedPoint() { }
	FixedPoint(int32_t i) : value(i << DECIMAL_BIT) {}
	FixedPoint(float f) : value(toFixed(f)) {}

	FixedPoint& operator += (FixedPoint r) 
	{ 
		value += r.value;
		return *this;
	}

	FixedPoint& operator -= (FixedPoint r) 
	{ 
		value -= r.value;
		return *this; 
	}

	FixedPoint& operator *= (FixedPoint r) 
	{ 
		value = fixedMul(value, r.value);
		return *this;
	}

	FixedPoint& operator /= (FixedPoint r) 
	{ 
		value = fixedDiv(value, r.value); 
		return *this; 
	}

	FixedPoint& operator *= (int32_t r) 
	{ 
		value *= r;
		return *this; 
	}

	FixedPoint& operator /= (int32_t r) 
	{ 
		value /= r;
		return *this; 
	}

	FixedPoint operator - ()  const
	{ 
		FixedPoint p;
		p.value = -value;
		return p;
	}

	FixedPoint operator + (int32_t r)  const
	{ 
		FixedPoint p;
		p.value = value + (r << DECIMAL_BIT);
		return p;
	}

	FixedPoint operator - (int32_t r)  const
	{
		FixedPoint p;
		p.value = value - (r << DECIMAL_BIT);
		return p;
	}

	FixedPoint operator * (int32_t r)  const
	{ 
		FixedPoint p;	
		p.value = fixedMul(value,(r << DECIMAL_BIT));
		return p;
	}

	FixedPoint operator / (int32_t r) const
	{
		FixedPoint p;
		p.value = fixedDiv(value,(r << DECIMAL_BIT));
		return p;
	}

	FixedPoint operator + (FixedPoint r)  const
	{ 
		FixedPoint p;
		p.value = value + r.value;
		return p;
	}

	FixedPoint operator - (FixedPoint r)  const
	{
		FixedPoint p;
		p.value = value - r.value;
		return p;
	}

	FixedPoint operator * (FixedPoint r)  const
	{ 
		FixedPoint p;	
		p.value = fixedMul(value,r.value);
		return p;
	}

	FixedPoint operator / (FixedPoint r) const
	{
		FixedPoint p;
		p.value = fixedDiv(value,r.value);
		return p;
	}

	bool operator == (FixedPoint r) const 
	{
		return value == r.value; 
	}

	bool operator != (FixedPoint r) const 
	{ 
		return !(*this == r);
	}

	bool operator <  (FixedPoint r) const 
	{ 
		return value < r.value;
	}

	bool operator >  (FixedPoint r) const 
	{ 
		return value > r.value; 
	}

	bool operator <= (FixedPoint r) const 
	{ 
		return value <= r.value; 
	}

	bool operator >= (FixedPoint r) const 
	{ 
		return value >= r.value; 
	}

	int32_t ToInt() { return value >> DECIMAL_BIT; }

	static  FixedPoint Floor(const FixedPoint& r) 
	{
		FixedPoint ret;
		ret.value = (r.value >> DECIMAL_BIT) << DECIMAL_BIT;
		return ret;
	}

	static  int32_t ToInt(const FixedPoint& r) 
	{
		return r.value >> DECIMAL_BIT;
	}

	static const int32_t DECIMAL_BIT = 8;
private:

	static int32_t toFixed(float f)
	{
		return (int32_t)(f * (1 << DECIMAL_BIT));
	}
};