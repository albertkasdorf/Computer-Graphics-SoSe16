#pragma once

// How to scale down a range of numbers with a known min and max value
// http://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
class Rescale
{
public:
	Rescale( float srcMin, float srcMax, float dstMin, float dstMax );

private:
	float srcMin, srcMax;
	float dstMin, dstMax;

public:
	float Scale( float x ) const;
	float Unscale( float x ) const;
};