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

	//---------------------COLISION GANAR------------
	if (body1->type == ElementType::GANAR && body1->isSensor)
	{
		App->player->ganar = true;
		
		App->player->perder = false;
		
		LOG("GANAR");
	}

	//---------------------COLISION TRAMPOLIN------------
	if (body1->type == ElementType::TRAMPOLIN && body1->isSensor)
	{
		
		App->player->vehicle->Push(0, 300, 0);
		LOG("TRAMPOLIN");
	}

	//---------------------COLISION HIELO------------
	if (body1->type == ElementType::ICE && body1->isSensor)
	{
		hielo = true;
		App->player->cueficienteFriccion = 0.0001f;
		LOG("ESTA SOBRE HIELO");
		
	}

	if (body1->type == ElementType::NO_ICE && body1->isSensor)
	{
		hielo = false;
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
	//---------------------------------BOLOS
	Sphere* sphere1 = new Sphere(2);
	sphere1->SetPos(0, 6, 30);
	primitives.PushBack(sphere1);
	App->physics->AddBody(*sphere1, 200.0f);
	sphere1->color = { 0,1,0,1 };

	Cube* bolo = new Cube(2, 10, 2);
	bolo->SetPos(0, 15, 80);
	primitives.PushBack(bolo);
	App->physics->AddBody(*bolo, 10.0f);
	bolo->color = { 0,1,0,1 };

	Cube* bolo1 = new Cube(2, 10, 2);
	bolo1->SetPos(10, 15, 90);
	primitives.PushBack(bolo1);
	App->physics->AddBody(*bolo1, 10.0f);
	bolo1->color = { 0,1,0,1 };

	Cube* bolo2 = new Cube(2, 10, 2);
	bolo2->SetPos(-10, 15, 90);
	primitives.PushBack(bolo2);
	App->physics->AddBody(*bolo2, 10.0f);
	bolo2->color = { 0,1,0,1 };

	// ---------------------------SUELO VENENO
	Cube* venenoSuelo = new Cube(700, 1, 700);
	venenoSuelo->SetPos(0, 0, 0);
	primitives.PushBack(venenoSuelo);
	App->physics->AddBody(*venenoSuelo, 0.0f, ElementType::DAMAGE, this, true);
	venenoSuelo->color = { 0,1,0,1 };
	//-----------------------------------------------------
	
	//-----------------------------checkpoints
	//ZONA GRAVEDAD
	Cube* checkpoint = new Cube(10, 10, 10);
	checkpoint->SetPos(0, 10, 0);
	primitives.PushBack(checkpoint);
	App->physics->AddBody(*checkpoint, 0.0f, ElementType::CHECKPOINT, this, true);
	checkpoint->wire = true;

	//ZONA TRAMPOLIN
	Cube* checkpoint2 = new Cube(30, 10, 40);
	checkpoint2->SetPos(45, 60, 80);
	primitives.PushBack(checkpoint2);
	App->physics->AddBody(*checkpoint2, 0.0f, ElementType::CHECKPOINT2, this, true);
	checkpoint2->wire = true;

	//ZONA HIELO
	Cube* checkpoint3 = new Cube(30, 10, 40);
	checkpoint3->SetPos(280, 60, 80);
	primitives.PushBack(checkpoint3);
	App->physics->AddBody(*checkpoint3, 0.0f, ElementType::CHECKPOINT3, this, true);
	checkpoint3->wire = true;

	//ZONA BOLAS
	Cube* checkpoint4 = new Cube(30, 10, 40);
	checkpoint4->SetPos(250, 25, -100);
	primitives.PushBack(checkpoint4);
	App->physics->AddBody(*checkpoint4, 0.0f, ElementType::CHECKPOINT4, this, true);
	checkpoint4->wire = true;

	//GANAR
	Cube* Ganar = new Cube(10, 10, 10);
	Ganar->SetPos(0, 40, -100);
	primitives.PushBack(Ganar);
	App->physics->AddBody(*Ganar, 0.0f, ElementType::GANAR, this, true);
	Ganar->wire = true;
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



	//---------------------ZONA AGUA
	
	
	Cube* Gravedad_INV1 = new Cube(100, 30, 40);
	Gravedad_INV1->SetPos(30, 38, 80);
	Gravedad_INV1->wire = true;
	primitives.PushBack(Gravedad_INV1);
	App->physics->AddBody(*Gravedad_INV1, 0.0f, ElementType::GRAVEDAD_INV, this, true);
	Gravedad_INV1->color = { 0,0,1,97 };

	Cube* SueloGRAVEDADRAMPA = new Cube(70, 10, 40);
	SueloGRAVEDADRAMPA->SetPos(80, 25, 80);
	SueloGRAVEDADRAMPA->SetRotation(35, { 0,0,1 });
	primitives.PushBack(SueloGRAVEDADRAMPA);
	App->physics->AddBody(*SueloGRAVEDADRAMPA, 0.0f);


	Cube* SueloGRAVEDADRAMPA2 = new Cube(70, 10, 40);
	SueloGRAVEDADRAMPA2->SetPos(80, 25, 80);
	SueloGRAVEDADRAMPA2->SetRotation(-35, { 0,0,1 });
	primitives.PushBack(SueloGRAVEDADRAMPA2);
	App->physics->AddBody(*SueloGRAVEDADRAMPA2, 0.0f);


	
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
	App->physics->AddBody(*Gravedad_NORM1, 0.0f, ElementType::GRAVEDAD_NORM, this, true);
	Gravedad_NORM1->color = { 0,0,1,97 };


	//-----------------------------------------Zona bolas tiradoras

	//1 bolas Tiradoras
	Sphere* esferaBase1 = new Sphere(1);
	esferaBase1->SetPos(160, 50, 80);
	primitives.PushBack(esferaBase1);
	App->physics->AddBody(*esferaBase1, 0.0f);


	Sphere* esferaTiradora1 = new Sphere(5);
	esferaTiradora1->SetPos(160, 30, 80);
	primitives.PushBack(esferaTiradora1);
	App->physics->AddBody(*esferaTiradora1, 1000000.0f);
	esferaTiradora1->body->Push(0.0f, 0.0f, 25000000.0f);
	esferaTiradora1->color = { 1,1,0,1 };

	App->physics->AddConstraintP2P(*esferaBase1->body, *esferaTiradora1->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase1->transform.translation().x - esferaTiradora1->transform.translation().x, esferaBase1->transform.translation().y - esferaTiradora1->transform.translation().y, esferaBase1->transform.translation().z - esferaTiradora1->transform.translation().z });
	
	//---------------------------------------------TRAMAPOLIN
	Cube* Trampolin1 = new Cube(40, 10, 40);
	Trampolin1->SetPos(200, 6, 80);
	Trampolin1->wire = true;
	primitives.PushBack(Trampolin1);
	App->physics->AddBody(*Trampolin1, 0.0f, ElementType::TRAMPOLIN, this, true);
	Trampolin1->color = { 0,0,1,97 };

	//Suelo Tras Trampolin
	Cube* Suelo3 = new Cube(80, 10, 40);
	Suelo3->SetPos(300, 50, 80);
	primitives.PushBack(Suelo3);
	App->physics->AddBody(*Suelo3, 0.0f);

	//Pared1
	Cube* Pared1 = new Cube(10, 60, 40);
	Pared1->SetPos(300, 50, 80);
	primitives.PushBack(Pared1);
	App->physics->AddBody(*Pared1, 0.0f);

	//-------------------------------------------------
	
	//--------------------------------------------Zona HIELO
	Cube* triggerHielo1 = new Cube(100, 10, 0.1);
	triggerHielo1->wire = true;
	triggerHielo1->SetPos(300, 60, 60);
	App->physics->AddBody(*triggerHielo1, 0.0f, ElementType::ICE,this,true);

	Cube* triggerHielo2 = new Cube(130, 0.1, 140);
	triggerHielo2->wire = true;
	triggerHielo2->SetPos(300, 20, -10);
	App->physics->AddBody(*triggerHielo2, 0.0f, ElementType::NO_ICE, this, true);


	//Rampa de hielo
	Cube* SueloHielo2 = new Cube(90, 10, 150);
	SueloHielo2->SetPos(300,31, -10);
	SueloHielo2->SetRotation(-15, { 1,0,0 });
	primitives.PushBack(SueloHielo2);
	App->physics->AddBody(*SueloHielo2, 0.0f);
	SueloHielo2->color = { 0,1,1,1 };

	//Suelo Final
	Cube* SueloHielo3 = new Cube(50, 10, 40);
	SueloHielo3->SetPos(250, 13, -100);
	primitives.PushBack(SueloHielo3);
	App->physics->AddBody(*SueloHielo3, 0.0f);

	//Bolas Dañinas

	Sphere* asesinoHielo1 = new Sphere(11);
	asesinoHielo1->SetPos(300, 25, -35);
	primitives.PushBack(asesinoHielo1);
	App->physics->AddBody(*asesinoHielo1, 0.0f, ElementType::DAMAGE);
	asesinoHielo1->color = { 1,0,0,1 };

	Sphere* asesinoHielo2 = new Sphere(11);
	asesinoHielo2->SetPos(260, 40, 20);
	primitives.PushBack(asesinoHielo2);
	App->physics->AddBody(*asesinoHielo2, 0.0f, ElementType::DAMAGE);
	asesinoHielo2->color = { 1,0,0,1 };

	Sphere* asesinoHielo3 = new Sphere(11);
	asesinoHielo3->SetPos(270, 20, -50);
	primitives.PushBack(asesinoHielo3);
	App->physics->AddBody(*asesinoHielo3, 0.0f, ElementType::DAMAGE);
	asesinoHielo3->color = { 1,0,0,1 };



	// ------------------------------------------Zona Bolas Asesinas

	//Suelo Pasarela
	Cube* SueloBolasAsesinas = new Cube(250, 10, 10);
	SueloBolasAsesinas->SetPos(100, 23, -100);
	SueloBolasAsesinas->SetRotation(-5, { 0,0,1 });
	primitives.PushBack(SueloBolasAsesinas);
	App->physics->AddBody(*SueloBolasAsesinas, 0.0f);

	//Suelo Ganar
	Cube* SueloBolasAsesinas2 = new Cube(25, 10, 25);
	SueloBolasAsesinas2->SetPos(0, 30, -100);

	primitives.PushBack(SueloBolasAsesinas2);
	App->physics->AddBody(*SueloBolasAsesinas2, 0.0f);


	
	//1 bolas Asesinas
	Sphere* esferaBase2 = new Sphere(1);
	esferaBase2->SetPos(90, 25, -100);
	primitives.PushBack(esferaBase2);
	App->physics->AddBody(*esferaBase2, 0.0f);


	Sphere* esferaAsesina1 = new Sphere(1.5);
	esferaAsesina1->SetPos(90, 30, -100);
	primitives.PushBack(esferaAsesina1);
	App->physics->AddBody(*esferaAsesina1, 10.0f, ElementType::DAMAGE, App->player, true);
	esferaAsesina1->body->Push(0.0f, 0.0f, 100000.0f);
	esferaAsesina1->color = { 1,0,0,1 };

	App->physics->AddConstraintP2P(*esferaBase2->body, *esferaAsesina1->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase2->transform.translation().x - esferaAsesina1->transform.translation().x, esferaBase2->transform.translation().y - esferaAsesina1->transform.translation().y, esferaBase2->transform.translation().z - esferaAsesina1->transform.translation().z });
	
	//2 bolas Asesinas
	Sphere* esferaBase3 = new Sphere(1);
	esferaBase3->SetPos(60, 30, -100);
	primitives.PushBack(esferaBase3);
	App->physics->AddBody(*esferaBase3, 0.0f);


	Sphere* esferaAsesina2 = new Sphere(1.5);
	esferaAsesina2->SetPos(60, 35, -100);
	primitives.PushBack(esferaAsesina2);
	App->physics->AddBody(*esferaAsesina2, 10.0f, ElementType::DAMAGE, App->player, true);
	esferaAsesina2->body->Push(0.0f, 0.0f, -100000.0f);
	esferaAsesina2->color = { 1,0,0,1 };

	App->physics->AddConstraintP2P(*esferaBase3->body, *esferaAsesina2->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase3->transform.translation().x - esferaAsesina2->transform.translation().x, esferaBase3->transform.translation().y - esferaAsesina2->transform.translation().y, esferaBase3->transform.translation().z - esferaAsesina2->transform.translation().z });
	
	//3 bolas Asesinas
	Sphere* esferaBase4 = new Sphere(1);
	esferaBase4->SetPos(30, 33, -100);
	primitives.PushBack(esferaBase4);
	App->physics->AddBody(*esferaBase4, 0.0f);


	Sphere* esferaAsesina3 = new Sphere(1.5);
	esferaAsesina3->SetPos(30, 40, -100);
	primitives.PushBack(esferaAsesina3);
	App->physics->AddBody(*esferaAsesina3, 10.0f, ElementType::DAMAGE, App->player, true);
	esferaAsesina3->body->Push(0.0f, 0.0f, 100000.0f);
	esferaAsesina3->color = { 1,0,0,1 };

	App->physics->AddConstraintP2P(*esferaBase4->body, *esferaAsesina3->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase4->transform.translation().x - esferaAsesina3->transform.translation().x, esferaBase4->transform.translation().y - esferaAsesina3->transform.translation().y, esferaBase4->transform.translation().z - esferaAsesina3->transform.translation().z });

	//4 bolas Asesinas
	Sphere* esferaBase5 = new Sphere(1);
	esferaBase5->SetPos(120, 25, -100);
	primitives.PushBack(esferaBase5);
	App->physics->AddBody(*esferaBase5, 0.0f);


	Sphere* esferaAsesina4 = new Sphere(1.5);
	esferaAsesina4->SetPos(120, 30, -100);
	primitives.PushBack(esferaAsesina4);
	App->physics->AddBody(*esferaAsesina4, 10.0f, ElementType::DAMAGE, App->player, true);
	esferaAsesina4->body->Push(0.0f, 0.0f, 100000.0f);
	esferaAsesina4->color = { 1,0,0,1 };

	App->physics->AddConstraintP2P(*esferaBase5->body, *esferaAsesina4->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase5->transform.translation().x - esferaAsesina4->transform.translation().x, esferaBase5->transform.translation().y - esferaAsesina4->transform.translation().y, esferaBase5->transform.translation().z - esferaAsesina4->transform.translation().z });

	

	//2 bolas Tiradoras
	Sphere* esferaBase20 = new Sphere(1);
	esferaBase20->SetPos(200, 40, -100);
	primitives.PushBack(esferaBase20);
	App->physics->AddBody(*esferaBase20, 0.0f);


	Sphere* esferaTiradora2 = new Sphere(5);
	esferaTiradora2->SetPos(200, 25, -100);
	primitives.PushBack(esferaTiradora2);
	App->physics->AddBody(*esferaTiradora2, 1000000.0f);
	esferaTiradora2->body->Push(0.0f, 0.0f, 25000000.0f);
	esferaTiradora2->color = { 1,1,0,1 };

	App->physics->AddConstraintP2P(*esferaBase20->body, *esferaTiradora2->body, vec3{ 0.0f, 0.0f, 0.0f }, vec3{ esferaBase20->transform.translation().x - esferaTiradora2->transform.translation().x, esferaBase20->transform.translation().y - esferaTiradora2->transform.translation().y, esferaBase20->transform.translation().z - esferaTiradora2->transform.translation().z });

	
}

