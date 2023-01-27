#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCircuito.h"
#include "Color.h"

#include "Globals.h"
#include "glmath.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleCircuito::ModuleCircuito(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleCircuito::~ModuleCircuito()
{}

bool ModuleCircuito::Start()
{
	bool ret = false;

	LOG("Loading Circuit");

	checkpoint2.SetPos(0, 5, 0);
	checkpoint2.size.x = 10;
	checkpoint2.size.y = 10;
	checkpoint2.size.z = 1;
	checkpoint2.axis = false;
	checkpoint2.wire = true;
	checkpoint2.color.Set(225, 225, 0);
	physSensor = App->physics->AddBody(checkpoint2, 0);
	physSensor->SetAsSensor(true);
	physSensor->SetId(2);

	//INITIAL TERRAIN
	initialTerrain.SetPos(0, 0.1f, 0);
	initialTerrain.size = { 40, 0.2f, 50 };
	initialTerrain.color = Brown;
	physWall = App->physics->AddBody(initialTerrain, WALL_MASS);

	limitWall1 = CreateRampOrWall(vec3(20, 2.4f, 0), vec3(0.2f, 5, 50), WALL_MASS * 2, Gray, 0, vec3(0, 0, 0), true);
	limitWall2 = CreateRampOrWall(vec3(0, 2.4f, -25), vec3(40, 5, 0.2f), WALL_MASS * 2, Gray, 0, vec3(0, 0, 0), true);

	//FIRST RAMP
	firstRamp = CreateRampOrWall(vec3(0, 6.f, 35.5f), vec3(40, 0.2f, 25), 0, Red, -30, vec3(1, 0, 0), true);

	//FIRST FLOOR
	first_terrain.SetPos(0, 6, 53.5f);
	first_terrain.size = { 40, 12.f, 15 };
	first_terrain.color = Brown;
	physFirstTerrain = App->physics->AddBody(first_terrain, WALL_MASS);

	limitWall3 = CreateRampOrWall(vec3(20, 14.4f, 53.5f), vec3(0.2f, 5, 15), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall4 = CreateRampOrWall(vec3(-20, 14.4f, 53.5f), vec3(0.2f, 5, 15), 0, Gray, 0, vec3(0, 0, 0), true);

	//SECOND RAMP
	secondRamp = CreateRampOrWall(vec3(0, 22, 77.5f), vec3(40, 0.2f, 40), 0, Red, -30, vec3(1, 0, 0), true);

	//SECOND FLOOR
	secondTerrain.SetPos(0, 16, 109.8f);
	secondTerrain.size = { 40, 32, 30 };
	secondTerrain.color = Brown;
	physSecondTerrain = App->physics->AddBody(secondTerrain, WALL_MASS);

	limitWall5 = CreateRampOrWall(vec3(20, 34.4f, 109.8f), vec3(0.2f, 5, 30), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall6 = CreateRampOrWall(vec3(-20, 34.4f, 109.8f), vec3(0.2f, 5, 30), 0, Gray, 0, vec3(0, 0, 0), true);

	//THIRD RAMP (DESCENDING ONE)
	thirdRamp = CreateRampOrWall(vec3(0, 16, 140.8f), vec3(40, 0.2f, 45.25f), 0, White, 45, vec3(1, 0, 0), true);

	//THIRD TERRAIN
	thirdTerrain.SetPos(-80, 0.1f, 171.8f);
	thirdTerrain.size = { 200, 0.2f, 30 };
	thirdTerrain.color = Red;
	physThirdTerrain = App->physics->AddBody(thirdTerrain, WALL_MASS);

	limitWall7 = CreateRampOrWall(vec3(-80, 2.4f, 186.8f), vec3(200, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall8 = CreateRampOrWall(vec3(20, 2.4f, 171.8f), vec3(0.2f, 5, 30), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall9 = CreateRampOrWall(vec3(-85, 2.4f, 156.8f), vec3(130, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);

	//FOURTH TERRAIN
	fourthTerrain.SetPos(-165, 0.1f, 106.15f);
	fourthTerrain.size = { 30, 0.2f, 101.3f };
	fourthTerrain.color = Green;
	physFourthTerrain = App->physics->AddBody(fourthTerrain, WALL_MASS);

	limitWall10 = CreateRampOrWall(vec3(-180, 2.4f, 121.15f), vec3(0.2f, 5, 131.3f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall11 = CreateRampOrWall(vec3(-150, 2.4f, 125.675f), vec3(0.2f, 5, 62.25f), 0, Gray, 0, vec3(0, 0, 0), true);

	//FIFTH TERRAIN
	fifthTerrain.SetPos(-50, 0.1f, 77.65f);
	fifthTerrain.size = { 200, 0.2f, 33.8f };
	fifthTerrain.color = Blue;
	physFifthTerrain = App->physics->AddBody(fifthTerrain, WALL_MASS);

	limitWall12 = CreateRampOrWall(vec3(-50, 2.4f, 94.25f), vec3(200, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall13 = CreateRampOrWall(vec3(-82.5f, 2.4f, 60.65f), vec3(200, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);

	//SIXTH TERRAIN
	sixthTerrain.SetPos(35, 0.1f, 41.875f);
	sixthTerrain.size = { 30, 0.2f, 37.75f };
	sixthTerrain.color = White;
	physSixthTerrain = App->physics->AddBody(sixthTerrain, WALL_MASS);

	limitWall14 = CreateRampOrWall(vec3(50.5, 2.4f, 59.25f), vec3(0.2f, 5, 71.3f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall15 = CreateRampOrWall(vec3(35, 2.4f, 23), vec3(30, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);

	//SEVENTH TERRAIN
	seventhTerrain.SetPos(-10, 0.1f, 36);
	seventhTerrain.size = { 60, 0.2f, 20.5f };
	seventhTerrain.color = Green;
	physSeventhTerrain = App->physics->AddBody(seventhTerrain, WALL_MASS);

	limitWall16 = CreateRampOrWall(vec3(-30, 2.4f, 25.75f), vec3(20, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall17 = CreateRampOrWall(vec3(-30, 2.4f, 46.25f), vec3(20, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);

	//FOURTH RAMP
	fourthRamp = CreateRampOrWall(vec3(-47, 3.169f, 36), vec3(15, 0.2f, 20.5f), 0, Red, -25, vec3(0, 0, 1), true);

	//FIRST ELEVATED 0º RAMP
	fifthRamp = CreateRampOrWall(vec3(-100, 16, 36), vec3(50, 0.2f, 20.5f), 0, Blue, 0, vec3(0, 0, 0), true);

	limitWall18 = CreateRampOrWall(vec3(-100, 18.4f, 25.75f), vec3(50, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall19 = CreateRampOrWall(vec3(-115, 18.4f, 46.25f), vec3(80, 5, 0.2f), 0, Gray, 0, vec3(0, 0, 0), true);

	//SECOND ELEVATED 0º RAMP
	sixthRamp = CreateRampOrWall(vec3(-140, 16, 16), vec3(30, 0.2f, 60), 0, White, 0, vec3(0, 0, 0), true);

	limitWall20 = CreateRampOrWall(vec3(-155, 16.4f, 16.125f), vec3(0.2f, 5, 60), 0, Gray, 0, vec3(0, 0, 0), true);
	limitWall21 = CreateRampOrWall(vec3(-125, 16.4f, 21.125), vec3(0.2f, 5, 9.5f), 0, Gray, 0, vec3(0, 0, 0), true);

	//SEVENTH RAMP
	seventhRamp = CreateRampOrWall(vec3(-75.65f, 8, 0), vec3(100, 0.2f, 30), 0, Green, -9.20f, vec3(0, 0, 1), true);

	//EIGHT TERRAIN
	eightTerrain.SetPos(-22.825f, 0.1f, 0);
	eightTerrain.size = { 6.80f, 0.2f, 30 };
	eightTerrain.color = Blue;
	physEightTerrain = App->physics->AddBody(eightTerrain, WALL_MASS);

	App->audio->PlayMusic("assets/audio/music/thunderstep-music-one-man-army-epic-electronic-rock-hybrid.wav");

	return true;
}

update_status ModuleCircuito::Update(float dt)
{

	initialTerrain.Render();
	firstRamp.Render();
	first_terrain.Render();
	secondRamp.Render();
	secondTerrain.Render();
	thirdRamp.Render();
	thirdTerrain.Render();
	fourthTerrain.Render();
	fifthTerrain.Render();
	sixthTerrain.Render();
	seventhTerrain.Render();
	fourthRamp.Render();
	fifthRamp.Render();
	sixthRamp.Render();
	seventhRamp.Render();
	eightTerrain.Render();

	limitWall1.Render();
	limitWall2.Render();
	limitWall3.Render();
	limitWall4.Render();
	limitWall5.Render();
	limitWall6.Render();
	limitWall7.Render();
	limitWall8.Render();
	limitWall9.Render();
	limitWall10.Render();
	limitWall11.Render();
	limitWall12.Render();
	limitWall13.Render();
	limitWall14.Render();
	limitWall15.Render();
	limitWall16.Render();
	limitWall17.Render();
	limitWall18.Render();
	limitWall19.Render();
	limitWall20.Render();
	limitWall21.Render();
	//limitWall22.Render();
	//limitWall23.Render();

	return UPDATE_CONTINUE;
}

bool ModuleCircuito::CleanUp()
{
	LOG("Unloading Circuit");

	return true;
}

Cube ModuleCircuito::CreateRampOrWall(vec3 position, vec3 size, float mass, Color color, float angle, vec3 angle_rot, bool is_collider)
{
	Cube cube(size.x, size.y, size.z);
	cube.SetPos(position.x, position.y, position.z);
	cube.color = color;

	if (angle != 0.0f)
	{
		cube.SetRotation(angle, angle_rot);
	}

	if (is_collider)
	{
		App->physics->AddBody(cube, mass);
	}

	return cube;
}


