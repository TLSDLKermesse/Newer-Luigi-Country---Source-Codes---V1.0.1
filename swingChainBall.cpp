#include <common.h>
#include <g3dhax.h>
#include <stage.h>


const char* guru [] = { "guru", NULL };

class daSwingBall : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

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
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::mdl_c axeModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daSwingBall);
	DECLARE_STATE(Swing);
};

CREATE_STATE(daSwingBall, Swing);

void daSwingBall::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daSwingBall::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->axeModel, anmChr, unk);
	this->axeModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daSwingBall::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daSwingBall));
	return new(buffer) daSwingBall;
}

const SpriteData SwingingChainBallSpriteData = 
if (SpriteSize == 1) {
	{ ProfileId::90, 0, 0, 0, -100, 0x240, 0x240, 0, 0, 0, 0, 0 };
} else if (SpriteSize == 2) {
	{ ProfileId::90, 0, 0, 0, -200, 0x500, 0x500, 0, 0, 0, 0, 0 };
} else {
	{ ProfileId::90, 0, 0, 0, -40, 0x100, 0x100, 0, 0, 0, 0, 0 };
}
	

Profile SwingingChainBallProfile(&daSwingBall::build, SpriteId::486, &SwingingChainBallSpriteData, ProfileId::90, ProfileId::90, "SwingingChainBall", guru);


int daSwingBall::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("guru", "g3d/guru.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("guru");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Rotate");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);
	
	resFile.data = getResource("guru", "g3d/guru.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("guru");
	axeModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&axeModel, 0);

	allocator.unlink(); 

	int SpriteSize = settings >> 28 & 0xF;
	
	if (SpriteSize == 1) {
		this->scale.x = 2.5; 
		this->scale.y = 2.5; 
		this->scale.z = 2.5;
	}
	if (SpriteSize == 2) {
		this->scale.x = 5.0; 
		this->scale.y = 5.0; 
		this->scale.z = 5.0;
	}
	if ((SpriteSize != 1) && (SpriteSize != 2)) {
		this->scale.x = 1.0; 
		this->scale.y = 1.0; 
		this->scale.z = 1.0;
	}


	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 0.0; 
	HitMeBaby.yDistToEdge = 0.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFE7FF7; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	bindAnimChr_and_setUpdateRate("Rotate", 1, 0.0, 1.0);

	doStateChange(&StateID_Swing);



	this->onExecute();

	return true;
}

int daSwingBall::onExecute() {
	acState.execute();

	updateModelMatrices();
	
	int ObjectType = settings >> 12 & 0xF;
	
	if (ObjectType == 1) {
		bodyModel._vf1C();
	} else {
		axeModel._vf1C();
	}


	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daSwingBall::onDelete() {
	return true;
}

int daSwingBall::onDraw() {
	bodyModel.scheduleForDrawing();
	axeModel.scheduleForDrawing();

	return true;
}

void daSwingBall::beginState_Swing() {
	this->rot.z = ((360 / 15) * (settings >> 24 & 0xF)) / 2;
	int initialPose = this->rot.z;
	int SwingSpeed = settings & 0xF;
	
}
void daSwingBall::executeState_Swing(int initialPose, SwingSpeed) {
	while ( this->rot.z > (initialPose - (initialPose * 2))) {
		this->rot.z -= (SwingSpeed * 3.0);
	}
	
	Stop(((settings >> 24 & 0xF) / 5));

	while ( this->rot.z < initialPose) {
		this->rot.z += (SwingSpeed * 3.0);
	}
	
	Stop(((settings >> 24 & 0xF) / 5));
	
}
void daSwingBall::endState_Swing() {
	
	doStateChange(&StateID_Swing);
	
}

//HEY DON'T FORGET THE COLLISIONS!!! :) 