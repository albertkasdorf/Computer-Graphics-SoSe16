#pragma once

class ElapsedTime
{
public:
	ElapsedTime( void );
	~ElapsedTime( void );

private:
	const double max;

	double timeSinceStart;
	double elapsedTime;

public:
	void Update( void );

	float Get( void ) const;

	float TimeSinceStart( void ) const;
};