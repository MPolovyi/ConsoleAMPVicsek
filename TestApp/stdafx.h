// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
struct abc
{
	int a; int b;
	abc() { a = 0; b = 0; };
	abc(int _a, int _b) : a(_a), b(_b) {}
};

template<int T>
class ABC
{
public:
	static abc s[];
	ABC() 
	{
		auto b = s[2];
	};
	~ABC();

private:

};

typedef class _tinymt_state_32
{
public:
	unsigned        mat1;
	unsigned        mat2;
	unsigned        tmat;
} tinymt_state_32;

struct _tinymt_dc_32
{
	unsigned 		characteristics[4];
	unsigned		type;
	unsigned		id;
	tinymt_state_32 state;
	unsigned		weight;
	unsigned		delta;
};

typedef class _tinymt32_status_t
{
public:
	unsigned status[4];
	tinymt_state_32 state;
} tinymt32_status_t;

// This table is output of the dynamic creator for mersenne twister.
// This table is for 32b RNG only.
_tinymt_dc_32 tinymt_dc_data_32[] =
{
	//# charactristic, type, id, mat1, mat2, tmat, weight, delta
	0xd8524022, 0xed8dff4a, 0x8dcc50c7, 0x98faba43, 32U, 0U, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff, 63U, 0U,
	0x8ee476cb, 0x10b7c7e2, 0x0dd10725, 0x924e9877, 32U, 0U, 0x877810ef, 0xfc38ff0f, 0xc7fb7fff, 63U, 0U
};

class tinymt_precalc
{
	tinymt_precalc() {};
	~tinymt_precalc() {};
	static _tinymt_dc_32 tinymt_dc_data_32[];
};

_tinymt_dc_32 tinymt_precalc::tinymt_dc_data_32[] =
{
	//# charactristic, type, id, mat1, mat2, tmat, weight, delta
	0xd8524022, 0xed8dff4a, 0x8dcc50c7, 0x98faba43, 32U, 0U, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff, 63U, 0U,
	0x8ee476cb, 0x10b7c7e2, 0x0dd10725, 0x924e9877, 32U, 0U, 0x877810ef, 0xfc38ff0f, 0xc7fb7fff, 63U, 0U
};