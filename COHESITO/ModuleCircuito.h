#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define WALL_MASS 100000

class ModuleCircuito : public Module
{
public:
	ModuleCircuito(Application* app, bool start_enabled = true);
	virtual ~ModuleCircuito();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Cube initialTerrain;
	PhysBody3D* physWall = nullptr;

	Cube firstRamp;
	Cube secondRamp;
	Cube thirdRamp;
	Cube fourthRamp;
	Cube fifthRamp;
	Cube sixthRamp;
	Cube seventhRamp;

	Cube first_terrain;
	PhysBody3D* physFirstTerrain = nullptr;

	Cube secondTerrain;
	PhysBody3D* physSecondTerrain = nullptr;

	Cube thirdTerrain;
	PhysBody3D* physThirdTerrain = nullptr;

	Cube fourthTerrain;
	PhysBody3D* physFourthTerrain = nullptr;

	Cube fifthTerrain;
	PhysBody3D* physFifthTerrain = nullptr;

	Cube sixthTerrain;
	PhysBody3D* physSixthTerrain = nullptr;

	Cube seventhTerrain;
	PhysBody3D* physSeventhTerrain = nullptr;

	Cube eightTerrain;
	PhysBody3D* physEightTerrain = nullptr;

	Cube FinishSensor;
	PhysBody3D* sensor = nullptr;

	//limit walls
	Cube limitWall1;
	Cube limitWall2;
	Cube limitWall3;
	Cube limitWall4;
	Cube limitWall5;
	Cube limitWall6;
	Cube limitWall7;
	Cube limitWall8;
	Cube limitWall9;
	Cube limitWall10;
	Cube limitWall11;
	Cube limitWall12;
	Cube limitWall13;
	Cube limitWall14;
	Cube limitWall15;
	Cube limitWall16;
	Cube limitWall17;
	Cube limitWall18;
	Cube limitWall19;
	Cube limitWall20;
	Cube limitWall21;
	Cube limitWall22;
	Cube limitWall23;

public:
	Cube CreateRampOrWall(vec3, vec3, float, Color, float, vec3, bool);
};