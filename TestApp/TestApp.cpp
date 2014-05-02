// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

class Polygon
{
protected:
	int width, height;
public:
	void set_values(int a, int b)
	{ width = a; height = b; }
};



class Rectangle : public Polygon
{
protected:
	int a;
	static abc ab;
public:
	int area()
	{ return width*height; }
};

class Triangle : public Polygon
{
public:
	int area()
	{ return width*height / 2; }
};

template<int T>
ABC<T>::~ABC()
{}

template<int T>
abc ABC<T>::s = { 10, T, T, 10 };

int _tmain(int argc, _TCHAR* argv[])
{
	ABC<5> ab;

	Rectangle rect;
	Triangle trgl;
	Polygon * ppoly1 = &rect;
	Polygon * ppoly2 = &trgl;
	ppoly1->set_values(4, 5);
	ppoly2->set_values(4, 5);

}
