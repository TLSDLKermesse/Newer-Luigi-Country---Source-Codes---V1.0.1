#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <stage.h>


const char* kanban_yajirushi [] = { "kanban_yajirushi", NULL };

class daHitboxArrow_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void setupModels();
	

	static daHitboxArrow_c* build();

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

};


void daHitboxArrow_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daHitboxArrow_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

void daHitboxArrow_c::setupModels() {
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	//Intended use is unknown.

	resFile.data = getResource("kanban_yajirushi_message", "g3d/kanban_yajirushi.brres");	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi_0");
	this->newModelA.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&this->newModelA, 0);
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi_1");
	this->newModelB.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&this->newModelB, 0);
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi_2");
	this->newModelC.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&this->newModelC, 0);
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi_3");
	this->newModelD.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&this->newModelD, 0);
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi_4");
	this->newModelE.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&this->newModelE, 0);
	
}

daHitboxArrow_c* daHitboxArrow_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daHitboxArrow_c));
	return new(buffer) daHitboxArrow_c;
}


int daHitboxArrow_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	int UsedModel = settings >> 8 & 0xF000000;
	int SignType = settings & 0xF000000;
	
	resFile.data = getResource("kanban_yajirushi", "g3d/kanban_yajirushi.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanban_yajirushi");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("kanban_yajirushi");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink(); 
	
	
	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	bindAnimChr_and_setUpdateRate("kanban_yajirushi", 1, 0.0, 1.0);

	this->onExecute();

	return true;
}

int daHitboxArrow_c::onExecute() {
	updateModelMatrices();
	if (UsedModel == 1) {
		if (SignType == 1) {newModelB._vf1C();}
		else if (SignType == 2) {newModelC._vf1C();}
		else if (SignType == 3) {newModelD._vf1C();}
		else if (SignType == 4) {newModelE._vf1C();}
		else {newModelA._vf1C();}
	} else {
		
		bodyModel._vf1C();
		this->bodyModel.setCurrentFrame(SignType);
		
	}

	return true;
}

int daHitboxArrow_c::onDelete() {
	return true;
}

int daHitboxArrow_c::onDraw() {
	if (UsedModel == 1) {
		if (SignType == 1) {newModelB.scheduleForDrawing();}
		else if (SignType == 2) {newModelC.scheduleForDrawing();}
		else if (SignType == 3) {newModelD.scheduleForDrawing();}
		else if (SignType == 4) {newModelE.scheduleForDrawing();}
		else {newModelA.scheduleForDrawing();}
	} else {bodyModel.scheduleForDrawing();}

	return true;
}

//Wow, that was easy for once. Isn't it?