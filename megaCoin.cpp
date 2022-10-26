#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "megaCoin.h"


const char* obj_coin_big [] = { "obj_coin_big", NULL };

class daBigCoin_c : public dEn_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	int timer;

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	//My sprite uses nybble 5 like the shy guy, please correct it if it's wrong, properties described below.
	
	mHeapAllocator_c allocator;
	m3d::mdl_c Coin10;
	m3d::mdl_c Coin30;
	m3d::mdl_c Coin50;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daBigCoin_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Caught);
};

CREATE_STATE(daBigCoin_c, Wait);
CREATE_STATE(daBigCoin_c, Caught);

void daBigCoin_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	Coin10.setDrawMatrix(matrix);
	Coin10.setScale(&scale);
	Coin10.calcWorld(false);
	//10 Coin Model
	
	Coin30.setDrawMatrix(matrix);
	Coin30.setScale(&scale);
	Coin30.calcWorld(false);
	//30 Coin Model
	
	Coin50.setDrawMatrix(matrix);
	Coin50.setScale(&scale);
	Coin50.calcWorld(false);
	//50 Coin Model
}

dActor_c* daBigCoin_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBigCoin_c));
	return new(buffer) daBigCoin_c;
}

const SpriteData MegaCoinSpriteData = 
{ ProfileId::89, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile MegaCoinProfile(&daBigCoin_c::build, SpriteId::488, &MegaCoinSpriteData, ProfileId::89, ProfileId::89, "MegaCoin", obj_coin_big);


int daBigCoin_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("obj_coin_big", "g3d/obj_coin_big.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Coin10");
	Coin10.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&Coin10, 0);
	
	//For 10 Coin Model
	
	this->resFile.data = getResource("obj_coin_big", "g3d/obj_coin_big.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Coin30");
	Coin30.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&Coin30, 0);
	
	//For 30 Coin Model
	
	this->resFile.data = getResource("obj_coin_big", "g3d/obj_coin_big.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Coin50");
	Coin50.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&Coin50, 0);
	
	//For 50 Coin Model

	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;


	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 0.0; 
	HitMeBaby.yDistToEdge = 0.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFEEBF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	doStateChange(&StateID_Wait);



	this->onExecute();

	return true;
}

int daBigCoin_c::onExecute() {
	acState.execute();

	int PieceType = settings >> 28 & 0xF;
	
	updateModelMatrices();
	if (PieceType == 1) {
		Coin30._vf1C();
	}
	if (PieceType == 2) {
		Coin50._vf1C();
	}
	if ((PieceType != 1) && (PieceType != 2)) {
		Coin10._vf1C();
	}

	return true;
}

int daBigCoin_c::onDelete() {
	return true;
}

int daBigCoin_c::onDraw() {
	Coin10.scheduleForDrawing();
	Coin30.scheduleForDrawing();
	Coin50.scheduleForDrawing();

	return true;
}

void daBigCoin_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	
	this->timer = 0;

	if (timer == 1 ) {
		PlaySound(this, SE_SYS_RANK_STAR_COIN_TO_SCORE);
		Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.2f, 2.2f, 2.2f};
		SpawnEffect("Wm_ob_starcoinget_hit", 0, &efPos, &nullRot, &efScale);
	}
	
	this->rot.y += 0x800;
	
	if ((timer > 30) && (timer < 60)) {
		speed.y = -0.1;
	} else {
		speed.y = 0.1;
	}
	
	if (done) {
		this->Delete(1);
	}
	
	this->timer += 1;
}

bool daBigCoin_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->doStateChange(&StateID_Caught);
}

bool daBigCoin_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

bool daBigCoin_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat5_Mario(apThis, apOther);
}

void daBigCoin_c::beginState_Wait() {
	
	this->timer = 0;
}
void daBigCoin_c::executeState_Wait() {
	
	this->rot.y += 0x200;
	
	if (timer == 60) {
		Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.2f, 2.2f, 2.2f};
		SpawnEffect("Wm_ob_starcoinget_ring", 0, &efPos, &nullRot, &efScale);
	}

	this->timer += 1;
	
	if (done) {
		doStateChange(&StateID_Wait);
	}
	
}
void daBigCoin_c::endState_Wait() {}

void daBigCoin_c::beginState_Caught() {}
void daBigCoin_c::executeState_Caught() {
		
	this->timer = 0;

	if (timer == 1 ) {
		PlaySound(this, SE_SYS_RANK_STAR_COIN_TO_SCORE);
		Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.2f, 2.2f, 2.2f};
		SpawnEffect("Wm_ob_starcoinget_hit", 0, &efPos, &nullRot, &efScale);
	}
	
	this->rot.y += 0x800;
	
	if ((timer > 30) && (timer < 60)) {
		speed.y = -0.1;
	} else {
		speed.y = 0.1;
	}
	
	if (done) {
		this->Delete(1);
	}
	
	this->timer += 1;
	
}
void daBigCoin_c::endState_Caught() {}