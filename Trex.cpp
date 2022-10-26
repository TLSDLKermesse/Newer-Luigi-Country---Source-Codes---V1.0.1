#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>


const char* rex [] = { "rex", NULL };

class daRex_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	int distance;
	char damage;
	char isDown;
	int timer;
	int distance;
	
	bool stillFalling;
	
	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daRex_c);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Stomp);
	DECLARE_STATE(Die);
};

CREATE_STATE(daRex_c, Walk);
CREATE_STATE(daRex_c, Stomp);
CREATE_STATE(daRex_c, Die);

void daRex_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daRex_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u16 name = ((dEn_c*)apOther->owner)->name;

	if (acState.getCurrentState() == &StateID_Walk) {

		pos.x = ((pos.x - ((dEn_c*)apOther->owner)->pos.x) > 0) ? pos.x + 1.5 : pos.x - 1.5;
		// pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
		doStateChange(&StateID_RealTurn); 
	}
}

void daRex_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	
	char hitType;

	if (this->isDown == 0) {
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);
	}
	else {
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
	}

	if(hitType == 1) {	// regular jump
		apOther->someFlagByte |= 2;
		if (this->isDown == 0) {
			this->playEnemyDownSound1();
			if (damage >= Health * 2) { //Check syntax
				doStateChange(Die); }
			else {
				doStateChange(Stomp); }
			damage++;
		}
		else { // TREXXXXXXXXXXXXXX ROARRRRRRRRRR
			this->playEnemyDownSound1();
			doStateChange(Die);
		}
	}
	else if(hitType == 3) {	// spinning jump or whatever?
		apOther->someFlagByte |= 2;
		if (this->isDown == 0) {
			this->playEnemyDownSound1();
			if (damage >= Health * 2) {
				doStateChange(Die); }
			else {
				doStateChange(Stomp); }
			damage++;
		}
		else { //A Does it work even it's copied-pasted :|
			this->playEnemyDownSound1();
			doStateChange(Die);
		}
	}
	else if(hitType == 0) {
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
	// else if(hitType == 2) { \\ Minimario? }
}

void daRex_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}

bool daRex_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
	damage = Health * 2;
	Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z + 1000.0f};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 2.0f, 2.0f};
	SpawnEffect("Wm_en_vshit_star", 0, &efPos, &nullRot, &efScale);
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_DAMAGE_LAST, 1);
	this->pos.x = 350.0;
	this->pos.y = -350.0;
	this->Delete;
	return true;
}

bool daRex_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &nullRot, &oneVec);
	//addScoreWhenHit accepts a player parameter.
	//DON'T DO THIS:
	// this->addScoreWhenHit(this);
	damage += 1;
	if (damage >= Health * 2) {
		this->doStateChange(&StateID_Die);
	} else {
		this->doStateChange(&StateID_Stomp);
	}
	
	return true;
}

bool daRex_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage = damage + Health;
	if (damage >= Health * 2) {
		this->doStateChange(&StateID_Die);
	} else {
		this->doStateChange(&StateID_Stomp);
	}
	
	return;
}

bool daRex_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage = Health * 2;
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_DAMAGE_LAST, 1);
	this->doStateChange(&StateID_Die);
	return;
}

bool daRex_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage += 2;
	if (damage >= Health * 2) {
		this->doStateChange(&StateID_Die);
	} else {
		this->doStateChange(&StateID_Stomp);
	}
	
	return;
}

bool daRex_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daRex_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daRex_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daRex_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daRex_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCat7_GroundPound(apThis, apOther);
}

bool daRex_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat7_GroundPound(apThis, apOther);
}

void daRex_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daRex_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daRex_c));
	return new(buffer) daRex_c;
}

const SpriteData T_RexSpriteData = 
{ ProfileId::97, 0, 0, 0, 0, 0x100, 0x200, 0, 0, 0, 0, 0 };

Profile T_RexProfile(&daRex_c::build, SpriteId::497, &T_RexSpriteData, ProfileId::97, ProfileId::97, "T_Rex", rex);


int daRex_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("rex", "g3d/rex.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("rex");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("walk");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	int Health = settings >> 12 & 0xF;

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 12.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 24.0;
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	direction = 1; //facing left first
	this->speed.x = -0.4;
	this->speed.y = 0.0;
	this->rot.y = (this->direction) ? 0xE000 : 0x2000;
	
	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);

	doStateChange(&StateID_Walk);



	this->onExecute();

	return true;
}

int daRex_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daRex_c::onDelete() {
	return true;
}

int daRex_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daRex_c::beginState_Walk() {this->timer = 0;}
void daRex_c::executeState_Walk() {
	
	if (distance) {
		if (collMgr.isOnTopOfTile()) {
			stillFalling = false;

			if (!willWalkOntoSuitableGround()) {
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				this->speed.x = direction ? 0.4 : -0.4;
				this->rot.y = (this->direction) ? 0xE000 : 0x2000;
			}
		}
		else {
			if (!stillFalling) {
				stillFalling = true;
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				this->rot.y = (this->direction) ? 0xE000 : 0x2000;
				this->speed.x = direction ? 0.4 : -0.4;
			}
		}
	}
	
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}	
	
	if ((timer == 20) || (timer == 40)) {
		Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {0.9f, 0.9f, 0.9f};
		SpawnEffect("Wm_mr_cmnlandsmoke", 0, &efPos, &nullRot, &efScale);
	}
}
void daRex_c::endState_Walk() {
	this->timer = 0;
	doStateChange(&StateID_Walk);
}

void daRex_c::beginState_Stomp() {
	MapSoundPlayer(SoundRelatedClass, SE_PLY_YOSHI_HIT_ENEMY, 1);
	Vec efPos = {pos.x, pos.y, pos.z};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {2.0f, 0.8f, 1.0f};
	effect.spawn("Wm_mo_damage", 0, &efPos, &nullRot, &efScale);
	aPhysics.removeFromList();
	//Reset aPhysics content
	
	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 12.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 12.0;
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();
	
	this->speed.x = -0.4;
	this->speed.y = 0.0;
	
	bindAnimChr_and_setUpdateRate("stomped", 3, 0.0, 3.0);
	int Infinity = 2;
}
void daRex_c::executeState_Stomp() {
	this->speed.x = 1.0;
	this->speed.y = 0.0;
	while (int ii = 0; ii < Infinity; ii++) {

		if (distance) {
			if (collMgr.isOnTopOfTile()) {
				stillFalling = false;

				if (!willWalkOntoSuitableGround()) {
					pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
					this->speed.x = direction ? 1.0 : -1.0;
				}
			}
			else {
				if (!stillFalling) {
					stillFalling = true;
					pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
					this->speed.x = direction ? 1.0 : -1.0;
				}
			}
		}
	
		if (this->animationChr.isAnimationDone()) {
			this->animationChr.setCurrentFrame(0.0);
		}	
	
		if ((timer == 5) || (timer == 10) || (timer == 15) || (timer == 20) || (timer == 25) || (timer == 30) || (timer == 35) || (timer == 40)) {
			PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
			Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
			S16Vec nullRot = {0,0,0};
			Vec efScale = {0.9f, 0.9f, 0.9f};
			SpawnEffect("Wm_mr_cmnlandsmoke", 0, &efPos, &nullRot, &efScale);
			if (timer == 40) {this->timer = 0;}
		}

		Infinity += 1;
	}
}
void daRex_c::endState_Stomp() {}

void daRex_c::beginState_Die() {
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("damage", 1, 0.0, 1.0);
}
void daRex_c::executeState_Die() {
	if (this->animationChr.isAnimationDone()) {
		this->Delete;
	}
}
void daRex_c::endState_Die() {}

//That was lazy so I didn't do comments :)))))))