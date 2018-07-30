
#include "Rescale.h"

Rescale::Rescale( float srcMin, float srcMax, float dstMin, float dstMax ) :
	srcMin( srcMin ), srcMax( srcMax ), dstMin( dstMin ), dstMax( dstMax )
{
}

float Rescale::Scale( float x ) const
{
	//assert( x >= srcMin && x <= srcMax );

	const float numerator = (dstMax - dstMin)*(x - srcMin);
	const float denominator = (srcMax - srcMin);

	return (numerator / denominator) + dstMin;
}

float Rescale::Unscale( float x ) const
{
	//assert( x >= dstMin && x <= dstMax );

	const float numerator = (srcMax - srcMin)*(x - dstMin);
	const float denominator = (dstMax - dstMin);

	return (numerator / denominator) + srcMin;
}