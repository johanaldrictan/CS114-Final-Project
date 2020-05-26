//====================================================================================
// File: Math.h
// Description: Header file containing math macros and functions
//====================================================================================
#ifndef MATH_H
#define MATH_H

#include <SDL/SDL.h>
#include "MR_EngineTypeDefs.h"

#define ABS(Expression) (Expression > 0 ? Expression : -1*(Expression))
#define ROUND(Expression) (Expression - (int)Expression >= 0.5f ? (int)(Expression+1) : (int)(Expression))

struct vector2
{
	int32 X;
	int32 Y;
};

struct vector2f
{
	float X;
	float Y;
};

struct rectf
{
	float X;
	float Y;
	float W;
	float H;
};

//Returns the interpolated y value for the given X
//(NOTE): Uses "precise" float subtraction i.e Lerp(f1,f2,0) = f1, Lerp(f1,f2,1) = f2
inline
float Lerp(float Y0, float Y1, float X)
{
	return (1 - X)*Y0 + X * Y1;
}

//Clamp X between F1 and F2
inline
float Clamp(float X, float F1, float F2)
{
	if (X > F2)
	{
		return F2;
	}
	if (X < F1)
	{
		return F1;
	}
	return X;
}

//Returns Euclidean Distance of 2 vectors
inline
float Distance(vector2 V1, vector2 V2)
{
	return (float)(V1.X - V2.X)*(V1.X - V2.X) + (V1.Y - V2.Y)*(V1.Y - V2.Y);
}

inline
float Distance(vector2f V1, vector2 V2)
{
	return (V1.X - V2.X)*(V1.X - V2.X) + (V1.Y - V2.Y)*(V1.Y - V2.Y);
}

inline
float Distance(vector2 V1, vector2f V2)
{
	return (V1.X - V2.X)*(V1.X - V2.X) + (V1.Y - V2.Y)*(V1.Y - V2.Y);
}

inline
float Distance(vector2f V1, vector2f V2)
{
	return (V1.X - V2.X)*(V1.X - V2.X) + (V1.Y - V2.Y)*(V1.Y - V2.Y);
}

//Returns true if the two rectangles overlaps
inline
bool Overlap(SDL_Rect R1, SDL_Rect R2)
{
	return ((R1.x < R2.x + R2.w && R1.x + R1.w > R2.x) &&
		(R1.y < R2.y + R2.h && R1.y + R1.h > R2.y));
}
#endif