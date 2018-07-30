#pragma once
/*
class RenderContext
{
	ModelMatrix model;
	//
};

class SolarSystem
{
public:
	void Render( );

private:
	void RenderRecursive( std::shared_ptr<SolarSystemObject> object );
};

class SolarSystemObject
{
protected:
	GLdouble diameter;
	glm::vec3 translation;
	GLdouble rotationSelf;
	GLdouble rotationOrigin;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<VertexArrayObject> mesh;

	std::vector<std::shared_ptr<SolarSystemObject>> objects;

public:
	void SubObjectAdd( std::shared_ptr<SolarSystemObject> object );
	std::shared_ptr<SolarSystemObject> SubObjectGet( GLUint index );
	GLuint SubObjectCountGet( void );

public:
	virtual void Render( RenderContext context ) = 0;
};

class Sun : public SolarSystemObject
{
};

class Planet : public SolarSystemObject
{
};*/