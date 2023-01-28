#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModulePhysics3D.h"
#include "ModuleCamera3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateCircuit();
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (uint n = 0; n < primitives.Count(); n++)
	{
		if (primitives[n]->body != nullptr)
		{
			primitives[n]->Update();
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	//---------------------COLISION TRAMPOLIN------------
	if (body1->type == ElementType::TRAMPOLIN && body1->isSensor)
	{
		
		App->player->vehicle->Push(0, 300, 0);
		LOG("TRAMPOLIN");
	}

	//---------------------COLISION HIELO------------
	if (body1->type == ElementType::ICE && body1->isSensor)
	{
		hielo = !hielo;
		App->player->cueficienteFriccion = 0.01f;
		LOG("ESTA SOBRE HIELO");
		
	}

	//---------------------COLISION GRAVEDAD------------
	if (body1->type == ElementType::GRAVEDAD_INV && body1->isSensor)
	{
		App->camera->Position.z = 200;
		App->physics->world->setGravity(btVector3(0.0f, 10.0f, 0.0f));
		LOG("ESTA SOBRE AGUA");
	}
	else if (body1->type == ElementType::GRAVEDAD_NORM && body1->isSensor)
	{
		App->physics->world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	}

	if (body1->type == ElementType::TUBE_SENSOR && body1->isSensor)
	{
		/*
		* 	primitives[16]->wire = true;
		primitives[17]->wire = true;
		primitives[18]->wire = true;
		primitives[19]->wire = true;
		*/
	
	}
	else if (body1->type == ElementType::SPAWN_SENSOR_RAMP && body1->isSensor )
	{
		/*
		* Sphere* sphere1 = new Sphere(2);
		sphere1->SetPos(230, 25, 90);
		primitives.PushBack(sphere1);
		App->physics->AddBody(*sphere1, 10.0f, ElementType::DAMAGE);
		sphere1->color = { 1,0,0,1 };

		Sphere* sphere2 = new Sphere(2);
		sphere2->SetPos(220, 25, 90);
		primitives.PushBack(sphere2);
		App->physics->AddBody(*sphere2, 10.0f, ElementType::DAMAGE);
		sphere2->color = { 1,0,0,1 };

		Sphere* sphere3 = new Sphere(2);
		sphere3->SetPos(210, 25, 90);
		primitives.PushBack(sphere3);
		App->physics->AddBody(*sphere3, 10.0f, ElementType::DAMAGE);
		sphere3->color = { 1,0,0,1 };

		Sphere* sphere4 = new Sphere(2);
		sphere4->SetPos(200, 25, 90);
		primitives.PushBack(sphere4);
		App->physics->AddBody(*sphere4, 10.0f, ElementType::DAMAGE);
		sphere4->color = { 1,0,0,1 };

		Sphere* sphere5 = new Sphere(2);
		sphere5->SetPos(190, 25, 90);
		primitives.PushBack(sphere5);
		App->physics->AddBody(*sphere5, 10.0f, ElementType::DAMAGE);
		sphere5->color = { 1,0,0,1 };

		*/
		
		
	}
	if (body1->type == ElementType::SPAWN_SENSOR_RAIL && body1->isSensor )
	{
		/*
		* Sphere* sphere6 = new Sphere(2);
		sphere6->SetPos(210, 24, -55);
		primitives.PushBack(sphere6);
		App->physics->AddBody(*sphere6, 10.0f, ElementType::DAMAGE);
		sphere6->color = { 1,0,0,1 };
		*/
		

		
	}
	if (body1->type == ElementType::FINISH && body1->isSensor && checkpoint == 4)
	{
		/*
		* App->player->minutesPassed = 0;
		App->player->secondsPassed = 0;
		App->player->lapTimer.Start();
		checkpoint = 0;

		App->player->currentHUD = HUDStatus::VICTORY;

		

		primitives[16]->wire = false;
		primitives[17]->wire = false;
		primitives[18]->wire = false;
		primitives[19]->wire = false;
		*/
		
	}
	if (body1->type == ElementType::CHECKPOINT && body1->isSensor && checkpoint == 0)
	{
		checkpoint = 1;
		App->player->currentHUD = HUDStatus::C1;
	}
	if (body1->type == ElementType::CHECKPOINT2 && body1->isSensor && checkpoint == 1)
	{
		checkpoint = 2;
		App->player->currentHUD = HUDStatus::C2;
	}
	if (body1->type == ElementType::CHECKPOINT3 && body1->isSensor && checkpoint == 2)
	{
		checkpoint = 3;
		App->player->currentHUD = HUDStatus::C3;
	}
	if (body1->type == ElementType::CHECKPOINT4 && body1->isSensor && checkpoint == 3)
	{
		checkpoint = 4;
		App->player->currentHUD = HUDStatus::C4;
	}
}


	//------------------------------------ Building scenario
void ModuleSceneIntro::CreateCircuit()
{

	Sphere* sphere1 = new Sphere(2);
	sphere1->SetPos(0, 6, 30);
	primitives.PushBack(sphere1);
	App->physics->AddBody(*sphere1, 2000.0f);
	sphere1->color = { 1,0,0,1 };

	// ---------------------------SUELO VENENO
	Cube* venenoSuelo = new Cube(700, 1, 700);
	venenoSuelo->SetPos(0, 0, 0);
	primitives.PushBack(venenoSuelo);
	App->physics->AddBody(*venenoSuelo, 0.0f, ElementType::DAMAGE, this, true);
	venenoSuelo->color = { 0,1,0,1 };
	//-----------------------------------------------------
	
	//-----------------------------checkpoints

	Cube* checkpoint = new Cube(10, 10, 10);
	checkpoint->SetPos(0, 10, 0);
	primitives.PushBack(checkpoint);
	App->physics->AddBody(*checkpoint, 0.0f, ElementType::CHECKPOINT, this, true);
	checkpoint->wire = true;


	Cube* checkpoint2 = new Cube(30, 10, 40);
	checkpoint2->SetPos(45, 60, 80);
	primitives.PushBack(checkpoint2);
	App->physics->AddBody(*checkpoint2, 0.0f, ElementType::CHECKPOINT2, this, true);
	checkpoint2->wire = true;


	Cube* checkpoint3 = new Cube(30, 10, 40);
	checkpoint3->SetPos(280, 60, 80);
	primitives.PushBack(checkpoint3);
	App->physics->AddBody(*checkpoint3, 0.0f, ElementType::CHECKPOINT3, this, true);
	checkpoint3->wire = true;

	//------------------------- CIELO
	// 1
	Cube* cielo1 = new Cube(500, 1, 500);
	cielo1->SetPos(100, 80, 0);
	primitives.PushBack(cielo1);
	App->physics->AddBody(*cielo1, 0.0f, ElementType::DAMAGE);
	cielo1->color = { 0.4,0.9,1,1 };


	// 2
	Cube* cielo2 = new Cube(500, 100, 1);
	cielo2->SetPos(100, 50, 200);
	primitives.PushBack(cielo2);
	App->physics->AddBody(*cielo2, 0.0f, ElementType::DAMAGE);
	cielo2->color = { 0.4,0.9,1,1 };

	
	// 3
	Cube* cielo3 = new Cube(1, 100, 500);
	cielo3->SetPos(-50, 50, 0);
	primitives.PushBack(cielo3);
	App->physics->AddBody(*cielo3, 0.0f, ElementType::DAMAGE);
	cielo3->color = { 0.4,0.9,1,1 };

	
	// 4
	Cube* cielo4 = new Cube(1, 100, 500);
	cielo4->SetPos(300, 50, 0);
	primitives.PushBack(cielo4);
	App->physics->AddBody(*cielo4, 0.0f, ElementType::DAMAGE);
	cielo4->color = { 0.4,0.9,1,1 };
	
	
	// 5
	Cube* cielo5 = new Cube(500, 100, 1);
	cielo5->SetPos(100, 50, -150);
	primitives.PushBack(cielo5);
	App->physics->AddBody(*cielo5, 0.0f, ElementType::DAMAGE);
	cielo5->color = { 0.4,0.9,1,1 };


	//-----------------------------------------------------
	
	//------------------------------------SUELO
	//1
	Cube* Suelo1 = new Cube(40, 10, 100);
	Suelo1->SetPos(0, 5, 10.5);
	primitives.PushBack(Suelo1);
	App->physics->AddBody(*Suelo1, 0.0f);

	//2
	Cube* Suelo2 = new Cube(200, 10, 40);
	Suelo2->SetPos(80, 5, 80);
	primitives.PushBack(Suelo2);
	App->physics->AddBody(*Suelo2, 0.0f);

	//3
	Cube* Suelo3 = new Cube(80, 10, 40);
	Suelo3->SetPos(300, 50, 80);
	primitives.PushBack(Suelo3);
	App->physics->AddBody(*Suelo3, 0.0f);


	//---------------------------------------------TRAMAPOLIN
	Cube* Trampolin1 = new Cube(40, 10, 40);
	Trampolin1->SetPos(200, 6, 80);
	Trampolin1->wire = true;
	primitives.PushBack(Trampolin1);
	App->physics->AddBody(*Trampolin1, 0.0f, ElementType::TRAMPOLIN, this, true);
	Trampolin1->color = { 0,0,1,97 };

	//Pared1
	Cube* Pared1 = new Cube(10, 40, 40);
	Pared1->SetPos(300, 50, 80);
	primitives.PushBack(Pared1);
	App->physics->AddBody(*Pared1, 0.0f);

	//-------------------------------------------------

	//---------------------AGUA
	
	//2
	Cube* Gravedad_INV1 = new Cube(100, 25, 40);
	Gravedad_INV1->SetPos(30, 40, 80);
	Gravedad_INV1->wire = true;
	primitives.PushBack(Gravedad_INV1);
	App->physics->AddBody(*Gravedad_INV1, 0.0f, ElementType::GRAVEDAD_INV, this, true);
	Gravedad_INV1->color = { 0,0,1,97 };

	Cube* SueloGRAVEDADRAMPA = new Cube(70, 10, 40);
	SueloGRAVEDADRAMPA->SetPos(80, 30, 80);
	SueloGRAVEDADRAMPA->SetRotation(50, { 0,0,1 });
	primitives.PushBack(SueloGRAVEDADRAMPA);
	App->physics->AddBody(*SueloGRAVEDADRAMPA, 0.0f);


	Cube* SueloGRAVEDADRAMPA2 = new Cube(70, 10, 40);
	SueloGRAVEDADRAMPA2->SetPos(80, 30, 80);
	SueloGRAVEDADRAMPA2->SetRotation(-50, { 0,0,1 });
	primitives.PushBack(SueloGRAVEDADRAMPA2);
	App->physics->AddBody(*SueloGRAVEDADRAMPA2, 0.0f);


	//2
	Cube* SueloGRAVEDAD = new Cube(100, 10, 40);
	SueloGRAVEDAD->SetPos(80, 50, 80);
	primitives.PushBack(SueloGRAVEDAD);
	App->physics->AddBody(*SueloGRAVEDAD, 0.0f);


	Cube* SueloGRAVEDADRAMPA3 = new Cube(50, 10, 40);
	SueloGRAVEDADRAMPA3->SetPos(10, 50, 80);
	SueloGRAVEDADRAMPA3->SetRotation(50, { 0,0,1 });
	primitives.PushBack(SueloGRAVEDADRAMPA3);
	App->physics->AddBody(*SueloGRAVEDADRAMPA3, 0.0f);
	
	Cube* Gravedad_NORM1 = new Cube(1000, 2, 1000);
	Gravedad_NORM1->SetPos(30, 60, 80);
	Gravedad_NORM1->wire = true;
	//primitives.PushBack(hielo1);
	App->physics->AddBody(*Gravedad_NORM1, 0.0f, ElementType::GRAVEDAD_NORM, this, true);
	Gravedad_NORM1->color = { 0,0,1,97 };
	
	
	
	//---------------------HIELO
	Cube* Hielo1 = new Cube(1, 2, 40);
	Hielo1->SetPos(0, 12, 80);
	Hielo1->wire = true;
	primitives.PushBack(Hielo1);
	App->physics->AddBody(*Hielo1, 0.0f, ElementType::ICE, this, true);
	Hielo1->color = { 0,0,1,97 };



	// Balls for constraints links
	// 46
	Sphere* sphere5 = new Sphere(1);
	sphere5->SetPos(150, 47, -100);
	primitives.PushBack(sphere5);
	App->physics->AddBody(*sphere5, 0.0f);

	// 47
	Sphere* sphere6 = new Sphere(5);
	sphere6->SetPos(150, 17, -100);
	primitives.PushBack(sphere6);
	App->physics->AddBody(*sphere6, 1000000.0f);
	sphere6->body->Push(25000000.0f, 0.0f, 0.0f);
	sphere6->color = { 1,1,0,1 };

	// 48
	Sphere* sphere7 = new Sphere(11);
	sphere7->SetPos(110, 11, -100);
	primitives.PushBack(sphere7);
	App->physics->AddBody(*sphere7, 0.0f, ElementType::DAMAGE);
	sphere7->color = { 1,0,0,1 };

	// 49
	Sphere* sphere8 = new Sphere(1);
	sphere8->SetPos(90, 20, -100);
	primitives.PushBack(sphere8);
	App->physics->AddBody(*sphere8, 0.0f);

	// 50
	Sphere* sphere9 = new Sphere(1.5);
	sphere9->SetPos(90, 15, -100);
	primitives.PushBack(sphere9);
	App->physics->AddBody(*sphere9, 10.0f, ElementType::DAMAGE, App->player, true);
	sphere9->body->Push(0.0f, 0.0f, 100000.0f);
	sphere9->color = { 1,0,0,1 };

	// 51
	Sphere* sphere10 = new Sphere(1);
	sphere10->SetPos(70, 20, -100);
	primitives.PushBack(sphere10);
	App->physics->AddBody(*sphere10, 0.0f);

	// 52
	Sphere* sphere11 = new Sphere(1.5);
	sphere11->SetPos(70, 15, -100);
	primitives.PushBack(sphere11);
	App->physics->AddBody(*sphere11, 10.0f, ElementType::DAMAGE, App->player, true);
	sphere11->body->Push(0.0f, 0.0f, -100000.0f);
	sphere11->color = { 1,0,0,1 };

	// 53
	Sphere* sphere12 = new Sphere(1);
	sphere12->SetPos(50, 20, -100);
	primitives.PushBack(sphere12);
	App->physics->AddBody(*sphere12, 0.0f);

	// 54
	Sphere* sphere13 = new Sphere(1.5);
	sphere13->SetPos(50, 15, -100);
	primitives.PushBack(sphere13);
	App->physics->AddBody(*sphere13, 10.0f, ElementType::DAMAGE, App->player, true);
	sphere13->body->Push(0.0f, 0.0f, 100000.0f);
	sphere13->color = { 1,0,0,1 };

	// Adding constraints
	App->physics->AddConstraintP2P(*sphere5->body, *sphere6->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ sphere5->transform.translation().x - sphere6->transform.translation().x, sphere5->transform.translation().y - sphere6->transform.translation().y, sphere5->transform.translation().z - sphere6->transform.translation().z });
	App->physics->AddConstraintP2P(*sphere8->body, *sphere9->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ sphere8->transform.translation().x - sphere9->transform.translation().x, sphere8->transform.translation().y - sphere9->transform.translation().y, sphere8->transform.translation().z - sphere9->transform.translation().z });
	App->physics->AddConstraintP2P(*sphere10->body, *sphere11->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ sphere10->transform.translation().x - sphere11->transform.translation().x, sphere10->transform.translation().y - sphere11->transform.translation().y, sphere10->transform.translation().z - sphere11->transform.translation().z });
	App->physics->AddConstraintP2P(*sphere12->body, *sphere13->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ sphere12->transform.translation().x - sphere13->transform.translation().x, sphere12->transform.translation().y - sphere13->transform.translation().y, sphere12->transform.translation().z - sphere13->transform.translation().z });
	
}

