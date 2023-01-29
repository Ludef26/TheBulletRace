#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;

	secondsPassed = 0;
	minutesPassed = 0;

	currentHUD = HUDStatus::START;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	VehicleInfo car;

	
	// Car properties ----------------------------------------
	car.chassis_size.Set(3.5, 1.5, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 300.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 10.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.5f;
	float half_length = car.chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);

	vehicle->SetPos(0.0f, 10.0f, -30.0f);

	vehicle->collision_listeners.add(this);

	initialTransf = new float[16];
	vehicle->GetTransform(initialTransf);

	lapTimer.Start();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{


	turn = acceleration = brake = 0.0f;
	// ______________________________________________DEBUG KEYS_________________________________________
	//CHECK POINTS
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		currentHUD = HUDStatus::C1;
		App->scene_intro->checkpoint = 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		currentHUD = HUDStatus::C2;
		App->scene_intro->checkpoint = 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		currentHUD = HUDStatus::C3;
		App->scene_intro->checkpoint = 3;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		currentHUD = HUDStatus::C4;
		App->scene_intro->checkpoint = 4;
	}
	
	//-------------------------CAMBIAR MASA COCHE----------------------
	//-------Mas masa
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		App->player->vehicle->info.mass += 100.f;
	}
	//-------------------Menos masa
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		App->player->vehicle->info.mass -= 100.f;
	}

	//-----------------------------------------------


	//---------------------------CONDICIONES---------------------------
	//-------boton ganar
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		ganar = true;
		perder = false;

	}
	if (ganar && !perder)
	{
		App->camera->Position.x = 10;
		App->camera->Position.y = 50;
		App->camera->Position.z = -205;
		sprintf_s(hud, "GANASTE");
		DrawTextHUD(10, 50, -200, hud, 1);

		sprintf_s(hud, "R to Restart");
		DrawTextHUD(10, 49, -200, hud, 2);

		sprintf_s(hud, "By : Juan de Dios Garcia & Luis Fernandez");
		DrawTextHUD(10.7, 48, -200, hud, 3);
		
	}
	//-------boton perder
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		perder = true;
		ganar = false;
	}
	if (perder && !ganar)
	{
		App->camera->Position.x = 10;
		App->camera->Position.y = 50;
		App->camera->Position.z = -205;
		sprintf_s(hud, "PERDISTE");
		DrawTextHUD(10, 50, -200, hud, 2);

		sprintf_s(hud, "R to Restart");
		DrawTextHUD(10, 49, -200, hud, 2);
		
		sprintf_s(hud, "By : Juan de Dios Garcia & Luis Fernandez");
		DrawTextHUD(10.7, 48, -200, hud, 3);
	}
	//-----------------------------------------------
	
	//-----------------------------QUITAR FUERZAS COCHE----------------------

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{

		quitarFisicas = !quitarFisicas;
	}

	if (quitarFisicas) 
	{
		App->physics->world->setGravity(btVector3(0.0f, 0.0f, 0.0f));
	}
	//-----------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		mostrarDatos = !mostrarDatos;

	}


	//Volver a el ultimo checkpoint
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		ResetPosition();

	//Hacer Reset al juego
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		currentHUD = HUDStatus::START;
		ResetLevel();
	}

	//---------------------------------------------------
	//--------------------------------CONTROLES JUGADOR
	if (App->camera->freeCamera == false && ganar == false && perder == false)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			acceleration = -MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY())
				vehicle->Jump(50000.0f);

			jumpCooldown.Start();
		}*/

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT )
		{
			vehicle->vehicle->getRigidBody()->applyTorqueImpulse({ 0.0f, 10.0f, 0.0f });
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT )
		{
			vehicle->vehicle->getRigidBody()->applyTorqueImpulse({ 0.0f, -10.0f, 0.0f });
		}
	}

	

	/*
	* if (minutesPassed == 4)
	{
		currentHUD = HUDStatus::GAME_OVER;
		ResetLevel();
	}
	*/
	
	//---------------------------- FRICCION COCHE
	normal = App->player->vehicle->info.mass * 10.0f;

	if (App->scene_intro->hielo == false) {

	cueficienteFriccion = 0.2f;
	}
	if (App->scene_intro->hielo == true) {
		vehicle->info.frictionSlip = 0.1f;
	}

	fuerzaFriccion = normal * cueficienteFriccion;


	if (App->player->vehicle->GetKmh() > 0) {
		acceleration -= fuerzaFriccion;
	}


	if (App->player->vehicle->GetKmh() < 0) {
		acceleration += fuerzaFriccion;
	}

	//----------------------------------------------
	vehicle->ApplyEngineForce(acceleration);
	
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	CameraFollow();

	vehicle->Render();

	if (danioCoche <= 0) {
		perder = true;
	}

	//-----------------------------------------------HUD
	if (mostrarDatos == true) 
	{
		sprintf_s(hud, "FRICCION --- %.2f", App->player->cueficienteFriccion);

		DrawTextHUD(
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX() + 2.5f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() + 5.0f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(),
			hud, 1
		);


		sprintf_s(hud, "Velocidad --- %.0f", App->player->vehicle->GetKmh());

		DrawTextHUD(
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX() + 2.5f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() + 7.0f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(),
			hud, 1
		);



		sprintf_s(hud, "Masa Coche --- %.0f", App->player->vehicle->info.mass);

		DrawTextHUD(
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX() + 2.5f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() + 6.0f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(),
			hud, 2
		);


		


		switch (currentHUD)
		{
		case(HUDStatus::C1):
			sprintf_s(hud, "Checkpoint 1");
			break;
		case(HUDStatus::C2):
			sprintf_s(hud, "Checkpoint 2");
			break;
		case(HUDStatus::C3):
			sprintf_s(hud, "Checkpoint 3");
			break;
		case(HUDStatus::C4):
			sprintf_s(hud, "Checkpoint 4");
			break;
		}

		DrawTextHUD(
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX() + 2.5f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() + 6.5f,
			vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(),
			hud, 1
		);



	}
	sprintf_s(hud, "DAMAGE %d / 15",danioCoche);
	DrawTextHUD(vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX() + 2.5f,
		vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() + 2.5f,
		vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(),
		hud, 1);

	sprintf_s(hud, "CHECKPOINT 1");
	DrawTextHUD(0, 10, 0, hud, 2);

	sprintf_s(hud, "CHECKPOINT 2");
	DrawTextHUD(45, 60, 80, hud, 2);

	sprintf_s(hud, "CHECKPOINT 3");
	DrawTextHUD(280, 60, 80, hud, 2);

	sprintf_s(hud, "CHECKPOINT 4");
	DrawTextHUD(250, 25, -100, hud, 2);

	sprintf_s(hud, "META");
	DrawTextHUD(0, 40, -100, hud, 2);

	
	if (daniarCoche == true) {
		danioCoche--;
		daniarCoche = false;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::CameraFollow()
{
	if (App->camera->freeCamera == false && ganar == false && perder == false)
	{
		carPos = vehicle->vehicle->getChassisWorldTransform();
		initialCarPos = { carPos.getOrigin().getX(),carPos.getOrigin().getY(),carPos.getOrigin().getZ() };
		carDir = { carPos.getBasis().getColumn(2).getX(),carPos.getBasis().getColumn(2).getY(),carPos.getBasis().getColumn(2).getZ() };
		App->camera->Position = cameraPos;
		cameraPos = initialCarPos - 15 * carDir;
		App->camera->Position.y = initialCarPos.y + 5;
	}
	
}

void ModulePlayer::ResetPosition()
{

	daniarCoche = true;
	if (App->scene_intro->checkpoint == 0)
	{
		vehicle->SetTransform(initialTransf);
	}
	else if (App->scene_intro->checkpoint == 1)
	{
		mat4x4 t;
		for (int i = 0; i < 16; i++)
		{
			t.M[i] = initialTransf[i];
		}
		t.rotate(0, vec3{ 0,1,0 });
		vehicle->SetTransform(t.M);
		vehicle->SetPos(0, 10, 5);
	}
	else if (App->scene_intro->checkpoint == 2)
	{
		mat4x4 t;
		for (int i = 0; i < 16; i++)
		{
			t.M[i] = initialTransf[i];
		}
		t.rotate(90, vec3{ 0,1,0 });
		vehicle->SetTransform(t.M);
		vehicle->SetPos(45, 60, 80);
	}
	else if (App->scene_intro->checkpoint == 3)
	{
		mat4x4 t;
		for (int i = 0; i < 16; i++)
		{
			t.M[i] = initialTransf[i];
		}
		t.rotate(180, vec3{ 0,1,0 });
		vehicle->SetTransform(t.M);
		vehicle->SetPos(280, 60, 80);
	}
	else if (App->scene_intro->checkpoint == 4)
	{
		mat4x4 t;
		for (int i = 0; i < 16; i++)
		{
			t.M[i] = initialTransf[i];
		}
		t.rotate(270, vec3{ 0,1,0 });
		vehicle->SetTransform(t.M);
		vehicle->SetPos(250, 25, -100);
	}

	vehicle->vehicle->getRigidBody()->clearForces();
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0.0f,0.0f,0.0f });
	vehicle->vehicle->resetSuspension();
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->type == ElementType::DAMAGE)
	{
		daniarCoche = true;
		ResetPosition();

	}

}

void ModulePlayer::DrawTextHUD(float x, float y, float z, const char* text,int color)
{
	if (color == 1) 
	{
	glColor4b(0.0f, 0.0f, 100.0f, 0.0f);
	}
	else if (color == 2) {
	glColor4b(100.0f, 0.0f, 0.0f, 0.0f);
	}

	else if (color == 3) {
		glColor4b(0.0f, 100.0f, 0.0f, 0.0f);
	}

	glRasterPos3f(x, y, z);
	int textLength = strlen(text);

	for (int i = 0; i < textLength; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

void ModulePlayer::ResetLevel()
{

	ganar = false;
	perder = false;
	danioCoche = 16;


	App->scene_intro->checkpoint = 0;

	ResetPosition();
	//App->scene_intro->spawnedBalls1 = false;
	//App->scene_intro->spawnedBalls2 = false;

	App->scene_intro->primitives[16]->wire = false;
	//App->scene_intro->primitives[17]->wire = false;
	App->scene_intro->primitives[18]->wire = false;
	App->scene_intro->primitives[19]->wire = false;
}