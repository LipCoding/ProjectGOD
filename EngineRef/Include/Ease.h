#pragma once

#include "PGMath.h"

#define PI 3.14159265f

PG_BEGIN


/*------ EASE parameter--------
	t:		timer		(Call by Value)
	s(b)(In):	start value
	v(c)(In):	variation(increase or decrease) value
	d(In):	duration
---------------------------------*/


class PG_DLL CEase_None
{
public:
	static float easeNone(float t, const float& s, const float& v, const float& d);
};

class PG_DLL CEase_Back {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);

};

class PG_DLL CEase_Bounce {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};


class PG_DLL CEase_Circ {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Cubic {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};


class PG_DLL CEase_Elastic {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Expo {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Linear {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Quad {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Quart {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

class PG_DLL CEase_Quint {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};


class PG_DLL CEase_Sine {

public:

	static float easeIn(float t, const float& b, const float& c, const float& d);
	static float easeOut(float t, const float& b, const float& c, const float& d);
	static float easeInOut(float t, const float& b, const float& c, const float& d);
};

PG_END