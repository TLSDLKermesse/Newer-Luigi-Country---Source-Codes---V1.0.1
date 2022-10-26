#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>

#define cPlayerOccupying (*(dStageActor_c**)(((u32)(clown)) + 0x738 ))
//Can this order be used again? (Comes From bossPlayerClown)

const char* I_moon [] = { "I_moon", NULL };

class daLiveMoon_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	int timer;

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daLiveMoon_c);
	DECLARE_STATE(Wait);
};

CREATE_STATE(daLiveMoon_c, Wait);

void daLiveMoon_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daLiveMoon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daLiveMoon_c));
	return new(buffer) daLiveMoon_c;
}

void daLiveMoon_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->pos.x = 330.0;
	this->pos.y = 240.0;
	Vec efPos = {pos.x + 8.0f, pos.y + 8.0f, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 2.0f, 2.0f};
	SpawnEffect("Wm_ob_keyget02_hit", 0, &efPos, &nullRot, &efScale);
}

void daLiveMoon_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u8 player = cPlayerOccupying->which_player;
	//Vec tempPos; (Intended use unknown)
	
	Vec efPos = {pos.x + 8.0f, pos.y + 8.0f, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 2.0f, 2.0f};
	SpawnEffect("Wm_ob_keyget02_hit", 0, &efPos, &nullRot, &efScale);

	int TouchedPlayer = player; //Is it gonna give us player's ID?
	for (int ii = 0; ii < 3; ii++) {//Syntax?
		player_lives[player] += 1;
		MapSoundPlayer(SoundRelatedClass, SE_SYS_ONE_UP, 1);
		Stop(10);
	}
	
	Stop(20);
	
	if (TouchedPlayer == 1) {//I didn't care if counter starts at 0 or 1.
		MapSoundPlayer(SoundRelatedClass, SE_VOC_LU_CLEAR_BOSS, 1);
	} else if (TouchedPlayer == 2) {
		MapSoundPlayer(SoundRelatedClass, SE_VOC_KO_CLEAR_BOSS, 1);
	} else if (TouchedPlayer == 3)
		MapSoundPlayer(SoundRelatedClass, SE_VOC_K_CLEAR_BOSS, 1);
	} else {
		MapSoundPlayer(SoundRelatedClass, SE_VOC_MA_CLEAR_BOSS, 1);
	}
	
	this->Delete(0);
}

void daLiveMoon_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->(spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) && playerCollision(ActivePhysics *apThis, ActivePhysics *apOther));
	//I'm not sure.
}

bool daLiveMoon_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
}

bool daLiveMoon_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);
}

bool daLiveMoon_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
}

bool daLiveMoon_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
}

bool daLiveMoon_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
}

const SpriteData 3UpMoonSpriteData = 
{ ProfileId::, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile 3UpMoonProfile(&daLiveMoon_c::build, SpriteId::506, &3UpMoonSpriteData, ProfileId::, ProfileId::, "3UpMoon", I_moon);


int daLiveMoon_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("I_moon", "g3d/I_moon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("I_moon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&bodyModel, 0);

	allocator.unlink(); 

	this->direction = 0; //On the right (no much utilities XD)
	
	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;


	ActivePhysics::Info moonPhysics; 
	moonPhysics.xDistToCenter = 0.0; 
	moonPhysics.yDistToCenter = 8.0; 
	moonPhysics.xDistToEdge = 12.0; 
	moonPhysics.yDistToEdge = 8.0; 
	moonPhysics.category1 = 0x3; 
	moonPhysics.category2 = 0x0; 
	moonPhysics.bitfield1 = 0x4F; 
	moonPhysics.bitfield2 = 0xFFC020C9; 
	//Ground Pound Yoshi Physics can't be found in NewSpriteCreator :(
	moonPhysics.unkShort1C = 0; 
	moonPhysics.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &moonPhysics); 
	this->aPhysics.addToList();



	doStateChange(&StateID_Wait);



	this->onExecute();

	return true;
}

int daLiveMoon_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int daLiveMoon_c::onDelete() {
	return true;
}

int daLiveMoon_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daLiveMoon_c::beginState_Wait() {
	MapSoundPlayer(SoundRelatedClass, SE_OBJ_BROOM_KEY_APP, 1);
}
void daLiveMoon_c::executeState_Wait() {
	
	for (int timer = 0; timer < 60; timer++) {
		this->pos.y += 0.05;
		//Why not two nummbers after the point? Does it work in this case?
		if (( timer <= 30) && (direction == 0)) {
			this->direction = 0;
			this->rot.y += 0x0050;
		} else {
			this->direction = 0;
			this->rot.y -= 0x0050;
		}
	}
	
	Vec efPos = {pos.x + 8.0f, pos.y + 8.0f, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	SpawnEffect("Wm_ob_keywait", 0, &efPos, &nullRot, &efScale);
	
	for (int timer = 0; timer < 60; timer++) {
		this->pos.y -= 0.05;
	}
	
	Vec efPos = {pos.x + 8.0f, pos.y + 8.0f, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	SpawnEffect("Wm_ob_keywait_c", 0, &efPos, &nullRot, &efScale);
	
}
void daLiveMoon_c::endState_Wait() {}
