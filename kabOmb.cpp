#include <common.h>
#include <g3dhax.h>
#include <stage.h>


const char* kab_omb [] = { "kab_omb", NULL };

class daWeirdBomb : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();
	int distance();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	int timer;
	bool stillFalling;
	
	static dActor_c* build();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
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

	USING_STATES(daWeirdBomb);
	DECLARE_STATE(walk);
	DECLARE_STATE(die);
	DECLARE_STATE(burn);
};

CREATE_STATE(daWeirdBomb, walk);
CREATE_STATE(daWeirdBomb, die);
CREATE_STATE(daWeirdBomb, burn);

void daWeirdBomb::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daWeirdBomb::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daWeirdBomb::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daWeirdBomb));
	return new(buffer) daWeirdBomb;
}

const SpriteData KabOmbSpriteData = 
{ ProfileId::100, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile KabOmbProfile(&daWeirdBomb::build, SpriteId::496, &KabOmbSpriteData, ProfileId::100, ProfileId::100, "KabOmb", kab_omb);


int daWeirdBomb::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("kab_omb", "g3d/kab_omb.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kab_omb");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("walk");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	bool fallsOffLedges = 0;
	
	stillFalling = 0;
	distance = 1;
	
	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;


	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; //Hitbox's x position
	HitMeBaby.yDistToCenter = 0.12; //Hitbox's y position
	HitMeBaby.xDistToEdge = 0.8; //Hitbox's X size
	HitMeBaby.yDistToEdge = 0.12; //Hitbox's Y size
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 6<<12);

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);

	doStateChange(&StateID_walk);



	this->onExecute();

	return true;
}

int daWeirdBomb::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daWeirdBomb::onDelete() {
	return true;
}

int daWeirdBomb::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daWeirdBomb::beginState_walk() {
	this->timer = 0;
	this->speed.x = direction ? 0.4 : -0.4;
	this->speed.y = 0.0;
}
void daWeirdBomb::executeState_walk() {
	
	if (timer == 20) {
		Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_mr_cmnlandsmoke", 0, &efPos, &nullRot, &efScale);
		this->timer = 0;
	}
	
	timer += 1;
}
void daWeirdBomb::endState_walk() {}

void daWeirdBomb::beginState_die() {}
void daWeirdBomb::executeState_die() {
	Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {0.9f, 0.9f, 0.9f};
	SpawnEffect("Wm_en_spindamage_rg", 0, &efPos, &nullRot, &efScale);
	bindAnimChr_and_setUpdateRate("stomp", 1, 0.0, 1.0);
	this->removeMyActivePhysics(); //True?
	this->rot.y = 0x8000;
	if (this->animationChr.isAnimationDone()) {
		this->Delete;
	}
}
void daWeirdBomb::endState_die() {}

void daWeirdBomb::beginState_burn() {
	this->speed.x = direction ? 1.0 : -1.0;
	this->timer = 0;
	MapSoundPlayer(SoundRelatedClass, SE_EMY_KERONPA_ALARM, 1);
	bindAnimChr_and_setUpdateRate("walk", 2.5, 0.0, 2.5);
}
void daWeirdBomb::executeState_burn() {
	
	for (timer = 0; timer < 50; timer++) {
		Stop(8);
		Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {0.9f, 0.9f, 0.9f};
		SpawnEffect("Wm_mr_cmnlandsmoke", 0, &efPos, &nullRot, &efScale);
	}
	
	this->rot.y = 0x8000; //horizontal axis
	this->removeMyActivePhysics(); //True?

	for (timer = 0; timer < 20; timer++) {
		Stop(8);
		Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {0.9f, 0.9f, 0.9f};
		SpawnEffect("Wm_mr_cmnlandsmoke", 0, &efPos, &nullRot, &efScale);
	}
	
	for (int i = 0; i < 10; i++) {
		this->pos.y += 2.0;
	}

	MapSoundPlayer(SoundRelatedClass, SE_EMY_KERONPA_BOMB, 1);
	Vec efPos = {this->pos.x + 7.0f, this->pos.y, this->pos.z - 3000.0f};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {4.0f, 4.0f, 4.0f};
	SpawnEffect("Wm_ob_fireworks_gcld", 0, &efPos, &nullRot, &efScale);
	this->Delete;
	
}
void daWeirdBomb::endState_burn() {}

bool daWeirdBomb::fallsOffLedges() {
	if (distance) {
		if (collMgr.isOnTopOfTile()) {
			stillFalling = false;

			if (!willWalkOntoSuitableGround()) {
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				this->speed.x = direction ? 0.4 : -0.4;
				this->rot.y = (this->direction) ? 0xE000 : 0x2000;
				doStateChange(&StateID_walk);
			}
		}
		else {
			if (!stillFalling) {
				stillFalling = true;
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				this->speed.x = direction ? 0.4 : -0.4;
				this->rot.y = (this->direction) ? 0xE000 : 0x2000;
				doStateChange(&StateID_walk);
			}
		}
	}

	bool ret = calculateTileCollisions();
	
	if (ret) {
		doStateChange(&StateID_RealTurn);
	}
}

bool daWeirdBomb::willWalkOntoSuitableGround() {
	static const float deltas[] = {2.5f, -2.5f};
	VEC3 checkWhere = {
			pos.x + deltas[direction],
			4.0f + pos.y,
			pos.z};

	u32 props = collMgr.getTileBehaviour2At(checkWhere.x, checkWhere.y, currentLayerID);

	//if (getSubType(props) == B_SUB_LEDGE)
	if (((props >> 16) & 0xFF) == 8)
		return false;

	float someFloat = 0.0f;
	if (collMgr.sub_8096781C(&checkWhere, &someFloat, currentLayerID, 1, -1)) {
		if (someFloat < checkWhere.y && someFloat > (pos.y - 5.0f))
			return true;
	}

	return false;
}