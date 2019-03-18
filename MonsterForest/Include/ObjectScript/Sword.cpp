#include "Sword.h"



CSword::CSword()
{
}

CSword::CSword(const CSword & gun)
{
}


CSword::~CSword()
{
}

bool CSword::Init()
{
	return false;
}

void CSword::Input(float fTime)
{
}

int CSword::Update(float fTime)
{


	return 0;
}

int CSword::LateUpdate(float fTime)
{
	return 0;
}

CSword * CSword::Clone()
{
	return new CSword(*this);
}
