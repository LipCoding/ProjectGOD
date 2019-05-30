#include "Ease.h"

PG_USING

float CEase_None::easeNone(float t, const float & s, const float & v, const float & d)
{
	//float postFix = t /= d;
	return v * t + s;
	/*if (t >= d)
	{
		t = d;
		return v;
	}

	return s;*/
}

////////////////////Back///////////////////////
float CEase_Back::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	float s = 1.70158f;
	float postFix = t /= d;
	return c * (postFix)*t*((s + 1)*t - s) + b;
}

float CEase_Back::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	float s = 1.70158f;
	return c * ((t = t / d - 1)*t*((s + 1)*t + s) + 1) + b;
}

float CEase_Back::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	float s = 1.70158f;

	if ((t /= d / 2) < 1)
		return c / 2 * (t*t*(((s *= (1.525f)) + 1)*t - s)) + b;

	float postFix = t -= 2;
	return c / 2 * ((postFix)*t*(((s *= (1.525f)) + 1)*t + s) + 2) + b;
}


////////////////////Bounce///////////////////////
float CEase_Bounce::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c - easeOut(d - t, 0, c, d) + b;
}

float CEase_Bounce::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d) < (1 / 2.75f))
	{
		return c * (7.5625f*t*t) + b;
	}
	else if (t < (2 / 2.75f))
	{
		float postFix = t -= (1.5f / 2.75f);
		return c * (7.5625f*(postFix)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75))
	{
		float postFix = t -= (2.25f / 2.75f);
		return c * (7.5625f*(postFix)*t + .9375f) + b;
	}
	else
	{
		float postFix = t -= (2.625f / 2.75f);
		return c * (7.5625f*(postFix)*t + .984375f) + b;
	}
}

float CEase_Bounce::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if (t < d / 2)
		return easeIn(t * 2, 0, c, d) * .5f + b;
	else
		return easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}


////////////////////Circ///////////////////////
float CEase_Circ::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;
	return -c * (sqrt(1 - (t /= d)*t) - 1) + b;
}

float CEase_Circ::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;
	return c * sqrt(1 - (t = t / d - 1)*t) + b;
}

float CEase_Circ::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	return c / 2 * (sqrt(1 - t * (t -= 2)) + 1) + b;
}


////////////////////Cubic///////////////////////
float CEase_Cubic::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * (t /= d)*t*t + b;
}

float CEase_Cubic::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * ((t = t / d - 1)*t*t + 1) + b;
}

float CEase_Cubic::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d / 2) < 1)
		return c / 2 * t*t*t + b;

	return c / 2 * ((t -= 2)*t*t + 2) + b;
}


////////////////////Elastic///////////////////////
float CEase_Elastic::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return b + c;

	float p = d * .3f;
	float a = c;
	float s = p / 4;
	float postFix = a * (float)pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
	return -(postFix * sinf((t*d - s)*(2 * PI) / p)) + b;
}

float CEase_Elastic::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if (t == 0)
		return b;

	if ((t /= d) == 1)
		return b + c;

	float p = d * .3f;
	float a = c;
	float s = p / 4;

	return (a*(float)pow(2, -10 * t) * sinf((t*d - s)*(2 * PI) / p) + c + b);
}

float CEase_Elastic::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if (t == 0)
		return b;
	if ((t /= d / 2) == 2)
		return b + c;

	float p = d * (.3f*1.5f);
	float a = c;
	float s = p / 4;

	if (t < 1)
	{
		float postFix = a * (float)pow(2, 10 * (t -= 1)); // postIncrement is evil
		return -.5f*(postFix* sinf((t*d - s)*(2 * PI) / p)) + b;
	}

	float postFix = a * (float)pow(2, -10 * (t -= 1)); // postIncrement is evil
	return postFix * sinf((t*d - s)*(2 * PI) / p)*.5f + c + b;
}


//////////////////////////expo/////////////////////////////
float CEase_Expo::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return (t == 0) ? b : c * (float)pow(2, 10 * (t / d - 1)) + b;
}

float CEase_Expo::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return (t == d) ? b + c : c * (const float&)(-pow(2, -10 * t / d) + 1) + b;
}

float CEase_Expo::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if (t == 0)
		return b;

	if (t == d)
		return b + c;

	if ((t /= d / 2) < 1)
		return c / 2 * (float)pow(2, 10 * (t - 1)) + b;

	return c / 2 * (float)(-pow(2, -10 * --t) + 2) + b;
}


///////////////////////linear/////////////////////////////////
float CEase_Linear::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * t / d + b;
}

float CEase_Linear::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * t / d + b;
}

float CEase_Linear::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * t / d + b;
}


////////////////////Quad///////////////////////
float CEase_Quad::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * (t /= d)*t + b;
}

float CEase_Quad::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return -c * (t /= d)*(t - 2) + b;
}

float CEase_Quad::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d / 2) < 1)
		return ((c / 2)*(t*t)) + b;

	return -c / 2 * (((t - 2)*(--t)) - 1) + b;
}


////////////////////Quart///////////////////////
float CEase_Quart::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * (t /= d)*t*t*t + b;
}

float CEase_Quart::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return -c * ((t = t / d - 1)*t*t*t - 1) + b;
}

float CEase_Quart::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d / 2) < 1)
		return c / 2 * t*t*t*t + b;

	return -c / 2 * ((t -= 2)*t*t*t - 2) + b;
}


////////////////////Quint///////////////////////
float CEase_Quint::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * (t /= d)*t*t*t*t + b;
}

float CEase_Quint::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * ((t = t / d - 1)*t*t*t*t + 1) + b;
}

float CEase_Quint::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	if ((t /= d / 2) < 1)
		return c / 2 * t*t*t*t*t + b;

	return c / 2 * ((t -= 2)*t*t*t*t + 2) + b;
}


////////////////////Sine///////////////////////
float CEase_Sine::easeIn(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return -c * cosf(t / d * (PI / 2)) + c + b;
}

float CEase_Sine::easeOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return c * sinf(t / d * (PI / 2)) + b;
}

float CEase_Sine::easeInOut(float t, const float & b, const float & c, const float & d)
{
	if (t >= d)
		t = d;

	return -c / 2 * (cosf(PI*t / d) - 1) + b;
}
