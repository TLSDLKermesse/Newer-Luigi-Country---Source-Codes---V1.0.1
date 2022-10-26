#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "boss.h"


const char* boom_boom [] = { "boom_boom", NULL };

class daBoom_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	char damage;
	char isDown;
	float XSpeed;
	int timer;
	bool stillFalling;
	bool topHurts;

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	
	void stunPlayers();
	void unstunPlayers();

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	bool isNearWall();
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daBoom_c);
	DECLARE_STATE(Intro);
	DECLARE_STATE(Move);
	DECLARE_STATE(Randomattack);
	DECLARE_STATE(Jump);
	DECLARE_STATE(Spin);
	DECLARE_STATE(Stomped);
	DECLARE_STATE(Damage);
	DECLARE_STATE(Starsforce);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daBoom_c, Intro);
CREATE_STATE(daBoom_c, Move);
CREATE_STATE(daBoom_c, Randomattack);
CREATE_STATE(daBoom_c, Jump);
CREATE_STATE(daBoom_c, Spin);
CREATE_STATE(daBoom_c, Stomped);
CREATE_STATE(daBoom_c, Damage);
CREATE_STATE(daBoom_c, Starsforce);
CREATE_STATE(daBoom_c, Outro);

void daBoom_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daBoom_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daBoom_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBoom_c));
	return new(buffer) daBoom_c;
}

const SpriteData BoomBoomSpriteData = 
{ ProfileId::92, 0, 0, 0, 0, 0x200, 0x240, 0, 0, 0, 0, 0 };

Profile BoomBoomProfile(&daBoom_c::build, SpriteId::495, &BoomBoomSpriteData, ProfileId::92, ProfileId::92, "BoomBoom", boom_boom);

void daBoom_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (this->isDown == 0) {
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);
	} else { // In damage last state
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
	}

	if(hitType == 1) {	// regular jump
		apOther->someFlagByte |= 2;
		if (this->isDown == 0) {
			this->playEnemyDownSound1();
			if (damage >= 18) {
				doStateChange(Damage); }
			else {
				doStateChange(Stomped); }
			damage++;
		} else {
			this->playEnemyDownSound1();
			doStateChange(Damage);
		} else if(hitType == 3) {
			apOther->someFlagByte |= 2;
			if (this->isDown == 0) {
				this->playEnemyDownSound1();
			} else {
				this->playEnemyDownSound1();
			}
		} else if(hitType == 0) {
			this->dEn_c::playerCollision(apThis, apOther);
			this->_vf220(apOther->owner);
		}
	}
}

void daBoom_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}

void daBoom_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	doStateChange(&StateID_Starsforce);
}

void daBoom_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage += 3;
	
	if (damage >= 18) {
		doStateChange(&StateID_Damage);
	} else {
		doStateChange(&StateID_Stomped);
	}
	
	return true;
}

void daBoom_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage += 3;
	this->collisionCatD_Drill(apThis, apOther);
	return true;
}

void daBoom_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat7_GroundPound(apThis, apOther);
}

void daBoom_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	throwObjectCounter += 1;
	damage += 1;
	if (throwObjectCounter < 3) {
		//MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_DAMAGE_DEF, 1);
		MapSoundPlayer(SoundRelatedClass, SE_EMY_KURIBO_L_DAMAGE_01, 1);
	}
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mo_damage", 0, &efPos, &nullRot, &efScale);
	//Hehehe it's blooding lol!
	
	if (throwObjectCounter >= 3) {
		if (damage >= 18) {
			doStateChange(&StateID_Damage);
		} else {
			doStateChange(&StateID_Stomped);
		}
	}
	
	return true;
}

void daBoom_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat9_RollingObject(apThis, apOther);
}

void daBoom_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat9_RollingObject(apThis, apOther);
}

void daBoom_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}

void daBoom_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat9_RollingObject(apThis, apOther);
}


int daBoom_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("boom_boom", "g3d/boom_boom.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("bunbun");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Entrance");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	stillFalling = 0;
	int jumpCounter = 0;
	int SoundID = 0;
	int throwObjectCounter = 0;
	damage = 0;
	
	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 0.200; 
	HitMeBaby.yDistToEdge = 0.240; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	topHurts = false;

	doStateChange(&StateID_Intro);



	this->onExecute();

	return true;
}

int daBoom_c::onExecute() {
	acState.execute();

	topHurts = false;
	
	updateModelMatrices();
	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daBoom_c::onDelete() {
	return true;
}

int daBoom_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daBoom_c::beginState_Intro() {
	this->timer = 0;

	// Stop the BGM Music
	StopBGMMusic();

	// Set the necessary Flags and make Mario enter Demo Mode
	dStage32C_c::instance->freezeMarioBossFlag = 1;
	WLClass::instance->_4 = 4;
	WLClass::instance->_8 = 0;

	MakeMarioEnterDemoMode();
	
	rot.y = 0x2800; // Y is horizontal axis
	speed.x = 0.0;
	speed.y = 0.0;
	
	pos.x = 130.0;
	pos.y = -50.0;
	extern "C" bool SpawnEffect(const char *BoomVoice [4], int, Vec*, S16Vec*, Vec*);
	//Am I using the right instruction to spawn a sound from sfx folder?
}
void daBoom_c::executeState_Intro() {
	if (timer == 15) {bindAnimChr_and_setUpdateRate("Entrance", 1, 0.0, 1.0);}
	
	if (chrAnimation.getCurrentFrame() = 90.0 || chrAnimation.getCurrentFrame() = 120.0 || chrAnimation.getCurrentFrame() = 150.0) {
		MapSoundPlayer(SoundRelatedClass, SE_EMY_MEGA_BROS_EARTHQUAKE, 1);
		Vec efPos = {pos.x + 7.0f, pos.y + 14.0f, pos.z - 5500.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		SpawnEffect("Wm_en_landsmoke_s", 0, &efPos, &nullRot, &efScale);
	}
	
	if (chrAnimation.getCurrentFrame() = 89.0) {
		extern "C" bool SpawnEffect(const char *BoomVoice [5], int, Vec*, S16Vec*, Vec*);
	}
	
	this->timer += 1;

	if (chrAnimation.getCurrentFrame() = 30.0 || chrAnimation.getCurrentFrame() = 60.0 || chrAnimation.getCurrentFrame() = 90.0) {
		Vec efPos = {pos.x + 7.0f, pos.y + 14.0f, pos.z - 5500.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		SpawnEffect("Wm_ob_fallsmoke_big", 0, &efPos, &nullRot, &efScale);
	}
	
	if (chrAnimation.getCurrentFrame() = 150.0) {
		Vec efPos = {pos.x + 7.0f, pos.y + 14.0f, pos.z - 5500.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		SpawnEffect("Wm_ob_fallsmoke_big", 0, &efPos, &nullRot, &efScale);
	}
}
void daBoom_c::endState_Intro() {
	Stop(120);
	rot.y = 0xD800; // Y is horizontal axis
	direction = 1; // Heading left.
	XSpeed = 1.0;
	speed.y = 0.0;
}

void daBoom_c::beginState_Move() {
	extern "C" bool SpawnEffect(const char *BoomVoice [1], int, Vec*, S16Vec*, Vec*);
	this->timer = 0;
	unstunPlayers();
	bindAnimChr_and_setUpdateRate("Move", 1, 0.0, 1.0);
}
void daBoom_c::executeState_Move() {
	for (int i = 0; i < 4 ; i++) {
		for (timer = 0; timer < 40; timer ++) {
			if (timer == 20) {
			Vec efPos = {pos.x, pos.y, pos.z};
			S16Vec nullRot = {0,0,0};
			Vec efScale = {2.0f, 0.8f, 1.0f};
			SpawnEffect("Wm_mr_brosquake", 0, &efPos, &nullRot, &efScale);
			}
			
			if ((timer == 10) || (timer == 30)) {
				Vec efPos = {pos.x, pos.y, pos.z};
				S16Vec nullRot = {0,0,0};
				Vec efScale = {2.0f, 0.8f, 1.0f};
				SpawnEffect("Wm_mr_misshit", 0, &efPos, &nullRot, &efScale);
			}
			
			if (direction == 0) {
				speed.x = 1.1 * XSpeed;
			} else {
				speed.x = -1.1 * XSpeed;
			}
	
			if (distance) {
				if (collMgr.isOnTopOfTile()) {
					stillFalling = false;
					pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
					doStateChange(&StateID_RealTurn);
				} else if (!stillFalling) {
					stillFalling = true;
					pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
					doStateChange(&StateID_RealTurn);	
				}
			}
		}
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		SpawnEffect("Wm_mr_brosquake", 0, &efPos, &nullRot, &efScale);
	}
	
	doStateChange(Randomattack);
}
void daBoom_c::endState_Move() {}

void daBoom_c::beginState_Randomattack() {}
void daBoom_c::executeState_Randomattack() {
	srand (time(NULL));
	int AttackNum = rand() % 2 + 1;
	if (AttackNum == 1) {
		srand (time(NULL));
		SoundID = rand() % 7 + 6; 
		doStateChange(Jump);
	}
	if (AttackNum == 2) {
		doStateChange(Spin);
	}
}
void daBoom_c::endState_Randomattack() {}

void daBoom_c::beginState_Jump() {
	extern "C" bool SpawnEffect(const char *BoomVoice [SoundID], int, Vec*, S16Vec*, Vec*);
	float moonjump = 5.0;
	speed.x = 0.0;
	speed.y = 0.0;
	bindAnimChr_and_setUpdateRate("Jump", 1, 0.0, 1.0);
	Stop(12);
	this->timer = 0;
}
void daBoom_c::executeState_Jump() {
	for (int ii = 0; ii < 20 ; ii++ ) {
		moonjump = moonjump / 1.0875;
		speed.y = moonjump;
		speed.x = (direction) ? 0.4f : -0.4f;
	}
	
	moonjump = moonjump - moonjump * 2;
	
	while (!stillFalling) {
		for (int ii = 0; ii < 20 ; ii++ ) {
			moonjump = moonjump * 1.0875;
			speed.y = moonjump;
			speed.x = (direction) ? 0.4f : -0.4f;
		}
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		SpawnEffect("Wm_mr_spindown", 0, &efPos, &nullRot, &efScale);
		speed.y = moonjump;
		speed.x = (direction) ? 0.4f : -0.4f;
	}

	stunPlayers();
	bindAnimChr_and_setUpdateRate("Jump_L", 1, 0.0, 1.0);
	MapSoundPlayer(SoundRelatedClass, SE_EMY_MEGA_BROS_EARTHQUAKE, 1);
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_ob_powdown", 0, &efPos, &nullRot, &efScale);
	Vec efScale = {2.0f, 0.8f, 1.0f};
	SpawnEffect("Wm_en_dossunfall01", 0, &efPos, &nullRot, &efScale);

}
void daBoom_c::endState_Jump() {
	speed.y = 0.0;
	speed.x = 0.0;
	jumpCounter += 1;
	srand (time(NULL));
	AttackNum = rand() % 2 + 1;
	if (AttackNum == 1) {
		jumpCounter = 0;
		doStateChange(&StateID_Move);
	} else if (jumpCounter > 2) {
		doStateChange(&StateID_Jump);
	}
}

void daBoom_c::beginState_Spin() {
	speed.x = 0.0;
	speed.y = 0.0;
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	SpawnEffect("Wm_mr_spindepart_a", 0, &efPos, &nullRot, &efScale);
	bindAnimChr_and_setUpdateRate("Spin_S", 1, 0.0, 1.0);
	Stop(40);
	this->timer = 0;
	MapSoundPlayer(SoundRelatedClass, SE_PLY_SPIN_ATTACK, 0.5);
}
void daBoom_c::executeState_Spin() {
	bindAnimChr_and_setUpdateRate("Spin", 1, 0.0, 1.0);
	moonjump = 3.0;
	for (timer = 0; timer < 40 ; timer++) {
		speed.x = (direction) ? 1.0f : -1.0f;
		moonjump = moonjump / 1.075;
		speed.y = moonjump;
		if ((timer == 5) || (timer == 10) || (timer == 15) || (timer == 20) || (timer == 25) || (timer == 30) || (timer == 35)) {
			Vec efPos = {pos.x, pos.y, pos.z};
			S16Vec nullRot = {0,0,0};
			Vec efScale = {2.0f, 0.8f, 1.0f};
			SpawnEffect("Wm_mr_spinjump", 0, &efPos, &nullRot, &efScale);
		}
	}

	moonjump = moonjump - moonjump * 2;

	while (!stillFalling) {
		for (timer = 0; timer < 40 ; timer++ ) {
			moonjump = moonjump * 1.075;
			speed.y = moonjump;
			speed.x = (direction) ? 1.0f : -1.0f;
		}
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {2.0f, 0.8f, 1.0f};
		effect.spawn("Wm_mr_spinjump", 0, &efPos, &nullRot, &efScale);
		speed.y = moonjump;
		speed.x = (direction) ? 1.0f : -1.0f;
	}
}
void daBoom_c::endState_Spin() {
	stunPlayers();
	srand (time(NULL));
	SoundID = rand() % 12 + 11;
	extern "C" bool SpawnEffect(const char *BoomVoice [SoundID], int, Vec*, S16Vec*, Vec*);
	speed.y = 0.0;
	speed.x = 0.0;
	bindAnimChr_and_setUpdateRate("Jump_L", 1, 0.0, 1.0);
	MapSoundPlayer(SoundRelatedClass, SE_EMY_MEGA_BROS_EARTHQUAKE, 1);
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	SpawnEffect("Wm_en_dossunfall02", 0, &efPos, &nullRot, &efScale);
	Stop(10);
	
	jumpCounter += 1;
	srand (time(NULL));
	AttackNum = rand() % 2 + 1;
	if (AttackNum == 1) {
		jumpCounter = 0;
		doStateChange(&StateID_Move);
	} else if (jumpCounter > 2) {
		doStateChange(&StateID_Spin);
	}
}

void daBoom_c::beginState_Stomped() {
	extern "C" bool SpawnEffect(const char *BoomVoice [3], int, Vec*, S16Vec*, Vec*);
	bindAnimChr_and_setUpdateRate("Damage", 1, 0.0, 1.0);
	speed.x = 0.0;
	speed.y = 0.0;
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_STOMPED, 1);
	this->timer = 0;
}
void daBoom_c::executeState_Stomped() {
	this->rot.y = 0x8000;
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	effect.spawn("Wm_mo_damage", 0, &efPos, &nullRot, &efScale);
	Stop(7);
	effect.spawn("Wm_mo_damage_a", 0, &efPos, &nullRot, &efScale);
	Stop(8);
	srand (time(NULL));
	SoundID = rand() % 17 + 15;
	extern "C" bool SpawnEffect(const char *BoomVoice [SoundID], int, Vec*, S16Vec*, Vec*);
	effect.spawn("Wm_mo_damage_b", 0, &efPos, &nullRot, &efScale);
	Stop(7);
	effect.spawn("Wm_mo_damage_c", 0, &efPos, &nullRot, &efScale);
	Stop(23);
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_FAT_LAND_QUAKE, 1);
	SpawnEffect("Wm_en_landsmoke_s", 0, &efPos, &nullRot, &efScale);
	Stop(15);
	SpawnEffect("Wm_en_landsmoke_s", 0, &efPos, &nullRot, &efScale);
	Stop(150);
	moonjump = 4.0;
	
	for (timer = 0; timer < 10; timer++) {
		moonjump = moonjump / 1.15;
		speed.y = moonjump;
		speed.x = 0.0;
	}
	
	moonjump = moonjump - moonjump * 2;
	
	while (!stillFalling) {
		for (timer = 0; timer < 40 ; timer++ ) {
			moonjump = moonjump * 1.15;
			speed.y = moonjump;
		}
	}
	
	SpawnEffect("Wm_en_landsmoke_s", 0, &efPos, &nullRot, &efScale);
	Stop (15);
	extern "C" bool SpawnEffect(const char *BoomVoice [1], int, Vec*, S16Vec*, Vec*);
	XSpeed += 1;
	doStateChange(&StateID_Move)
}
void daBoom_c::endState_Stomped() {
	this->rot.y = (this->direction) ? 0xE000 : 0x2000;
}

void daBoom_c::beginState_Damage() {
	this->rot.y = 0x8000;
	//Don't forget that Y is horizontal axis in case of models :)
	extern "C" bool SpawnEffect(const char *BoomVoice [2], int, Vec*, S16Vec*, Vec*);
	bindAnimChr_and_setUpdateRate("DamageLast", 1, 0.0, 1.0);
	speed.x = 0.0;
	speed.y = 0.0;
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_STOMPED, 1);
	OutroSetup(this);
	aPhysics.removeFromList();

	this->speed.x = 0;
	this->speed.y = 0.0;

	this->isDown = 1;
	this->timer = 0;
	
	Chuckles.removeFromList();
	Knuckles.removeFromList();
	
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	effect.spawn("Wm_mo_damage", 0, &efPos, &nullRot, &efScale);
	Stop(7);
	effect.spawn("Wm_mo_damage_a", 0, &efPos, &nullRot, &efScale);
	Stop(8);
	effect.spawn("Wm_mo_damage_b", 0, &efPos, &nullRot, &efScale);
	Stop(7);
	effect.spawn("Wm_mo_damage_c", 0, &efPos, &nullRot, &efScale);
	Stop(23);
}
void daBoom_c::executeState_Damage() {
	for (timer = 0; timer < 30; timer++) {
		this->rot.x += 0x200;
	}
	
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_DAMAGE_LAST, 1);
	
	for (timer = 0; timer < 40; timer++) {
		this->pos.y -= 3.0;
	}

	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {7.0f, 7.0f, 7.0f};
	effect.spawn("Wm_ob_stream", 0, &efPos, &nullRot, &efScale);
	stunPlayers();
	//Maybe it's not the right effect to choose here.
	Stop(30);
	unstunPlayers();
	Stop(30);
}
void daBoom_c::endState_Damage() {doStateChange(&StateID_Outro);}

void daBoom_c::beginState_Starsforce() {
	this->rot.y = 0x8000;
	bindAnimChr_and_setUpdateRate("DamageLast", 1, 0.0, 1.0);
	speed.x = 0.0;
	speed.y = 0.0;
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_STOMPED_LAST1, 1);
	OutroSetup(this);
	aPhysics.removeFromList();

	this->speed.x = 0;
	this->speed.y = 0.0;
	
	this->isDown = 1;
	this->timer = 0;
	
	Chuckles.removeFromList();
	Knuckles.removeFromList();
}
void daBoom_c::executeState_Starsforce() {
	moonjump = 3.75;
	for (i = 0; i < 80; i++) {
		moonjump = moonjump / 1.05;
		this->speed.y = moonjump;
		this->rot.x += 0x900;
	}
	Stop(20);
	this->pos.x = -300.0;
	this->pos.y = -300.0;
	MapSoundPlayer(SoundRelatedClass, SE_DEMO_OP_FIREWORKS, 1);
	Vec efPos = {pos.x + 300.0, pos.y + 300.0, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 2.0f, 2.0f};
	effect.spawn("Wm_2d_timeupsmoke", 0, &efPos, &nullRot, &efScale);
}
void daBoom_c::endState_Starsforce() {
	Stop(120);
	doStateChange(&StateID_Outro);
}

void daBoom_c::beginState_Outro() {
	this->timer = 0;
}
void daBoom_c::executeState_Outro() {
	if (this->dying == 1) {

		if (this->timer > 180) { ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE); }
		if (this->timer == 60) { PlayerVictoryCries(this); }

		this->timer += 1;
		return;
	}

	if ((chrAnimation.getCurrentFrame() == 41.0) || (chrAnimation.getCurrentFrame() == 62.0)) {
		Vec efPos = {pos.x, pos.y - 8.0f, pos.z + 500.0f};
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {7.0f, 7.0f, 7.0f};
		SpawnEffect("Wm_en_landsmoke_s", 0, &efPos, &nullRot, &oneVec);
	}

	if (chrAnimation.isAnimationDone()) {
		S16Vec nullRot = {0,0,0};

		Vec efPos = {pos.x + 8.0f, pos.y - 8.0f, pos.z + 500.0f};
		Vec efScale = {6.75f, 6.75f, 6.75f};
		SpawnEffect("Wm_ob_cmnshotstar", 0, &efPos, &nullRot, &efScale);

		Vec hitPos = {pos.x + 8.0f, pos.y, pos.z + 500.0f};
		Vec oneVec = {7.0f, 7.0f, 7.0f};
		SpawnEffect("Wm_mr_wirehit_hit", 0, &hitPos, &nullRot, &oneVec);

		MapSoundPlayer(SoundRelatedClass, STRM_BGM_SHIRO_BOSS_CLEAR, 1);
		BossGoalForAllPlayers();

		this->dying = 1;
		this->timer = 0;
	}

	this->timer += 1;
}
void daBoom_c::endState_Outro() {}

void daBoom_c::stunPlayers() {
	for (int i = 0; i < 4; i++) {
		playerStunned[i] = false;

		dStageActor_c *player = GetSpecificPlayerActor(i);
		if (player) {
			if (player->collMgr.isOnTopOfTile() && player->currentZoneID == currentZoneID) {
				stunPlayer(player, 1);
				playerStunned[i] = true;
			}
		}
	}
}

void daBoom_c::unstunPlayers() {
	for (int i = 0; i < 4; i++) {
		dStageActor_c *player = GetSpecificPlayerActor(i);
		if (player && playerStunned[i]) {
			unstunPlayer(player);
		}
	}
}

bool daBoom_c::isNearWall() {
	// back up our current settings
	VEC3 savePos = pos;
	VEC3 saveSpeed = speed;
	int saveDirection = direction;

	float checkLeft = (direction == 0) ? 8.0f : 104.0f;
	float checkRight = (direction == 0) ? 104.0f : 8.0f;

	bool result = false;

	speed.x = -0.1f;
	speed.y = 0.0f;
	pos.x = savePos.x - checkLeft;
	direction = 1; // left
	if (collMgr.calculateAdjacentCollision())
		result = true;

	speed.x = 0.1f;
	speed.y = 0.0f;
	pos.x = savePos.x + checkRight;
	direction = 0; // right
	if (collMgr.calculateAdjacentCollision())
		result = true;

	pos = savePos;
	speed = saveSpeed;
	direction = saveDirection;

	return result;
}

const char* BoomVoice[17] = {
	"sfx/#6_BUNBUN_ANGRY"
	"sfx/#6_BUNBUN_DAMAGE_LAST"
	"sfx/#6_BUNBUN_DAMAGE1"
	"sfx/#6_BUNBUN_GUESS_MARIO"
	"sfx/#6_BUNBUN_INTRO"
	"sfx/#6_BUNBUN_JUMP1"
	"sfx/#6_BUNBUN_JUMP2"
	"sfx/#6_BUNBUN_RETRY1"
	"sfx/#6_BUNBUN_RETRY2"
	"sfx/#6_BUNBUN_SPIN"
	"sfx/#6_BUNBUN_SPIN_DONE1"
	"sfx/#6_BUNBUN_SPIN_DONE2"
	"sfx/#6_BUNBUN_SPIN_START1"
	"sfx/#6_BUNBUN_SPIN_START2"
	"sfx/#6_BUNBUN_STOMPED1"
	"sfx/#6_BUNBUN_STOMPED2"
	"sfx/#6_BUNBUN_STOMPED3"
}
//These codes are finished! But I really need help in checking syntax, as there're at least 700 lines. :(