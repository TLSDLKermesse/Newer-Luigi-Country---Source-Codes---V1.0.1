#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>


const char* block_roulette_coin [] = { "block_roulette_coin", NULL };

class daBigCoinRoll_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	void blockWasHit(bool isDown);
	int timer;

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
	m3d::mdl_c InitialModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daBigCoinRoll_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Hit);
};

CREATE_STATE(daBigCoinRoll_c, Wait);
CREATE_STATE(daBigCoinRoll_c, Hit);

void daBigCoinRoll_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	InitialModel.setDrawMatrix(matrix);
	InitialModel.setScale(&scale);
	InitialModel.calcWorld(false);
}

dActor_c* daBigCoinRoll_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBigCoinRoll_c));
	return new(buffer) daBigCoinRoll_c;
}

const SpriteData CoinRouletteSpriteData = 
{ ProfileId::101, 0, 0, -8, -8, 0x200, 0x200, 0, 0, 0, 0, 0 };

Profile CoinRouletteProfile(&daBigCoinRoll_c::build, SpriteId::487, &CoinRouletteSpriteData, ProfileId::101, ProfileId::101, "CoinRoulette", block_roulette_coin);


int daBigCoinRoll_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);


	resFile.data = getResource("block_roulette_coin", "g3d/block_roulette_coin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("block_roulette_coin");
	InitialModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&InitialModel, 0);
	if (RouletteBlockType == 1) { nw4r::g3d::ResAnmTexPat ftResAnm = this->GetResAnmTexPat("block_roulette_coin_big");
	} else {nw4r::g3d::ResAnmTexPat ftResAnm = this->GetResAnmTexPat("block_roulette_coin");} //Short Orders :)
	nw4r::g3d::ResAnmTexSrt ftResAnm = this->GetResAnmTexSrt("block_roulette_glass");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 
	
	int RouletteBlockType = (settings & 0xF);
	
	if (RouletteBlockType == 1) {
		this->scale.x = 2.0; 
		this->scale.y = 2.0; 
		this->scale.z = 2.0;
	} else {
		this->scale.x = 1.0; 
		this->scale.y = 1.0; 
		this->scale.z = 1.0;
	}


	ActivePhysics::Info HitMeBaby; 
	if (RouletteBlockType == 1) {
		HitMeBaby.xDistToCenter = 48.0;
		HitMeBaby.yDistToCenter = 48.0;
		HitMeBaby.xDistToEdge = 48.0;
		HitMeBaby.yDistToEdge = 48.0;
	} else {
		HitMeBaby.xDistToCenter = 24.0;
		HitMeBaby.yDistToCenter = 24.0;
		HitMeBaby.xDistToEdge = 24.0;
		HitMeBaby.yDistToEdge = 24.0;
	}

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF; //Not modified so idk
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();


	if (RouletteBlockType == 1) { setupAnim("block_roulette_coin_big", 1.0); // I just used a different operation
	} else {setupAnim("block_roulette_coin", 1.0);}
	
	setupAnim("block_roulette_glass", 1.0); //As I thought it works for any type of animation.
	
	doStateChange(&StateID_Wait);



	this->onExecute();

	return true;
}

int daBigCoinRoll_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	InitialModel._vf1C();

	return true;
}

int daBigCoinRoll_c::onDelete() {
	return true;
}

int daBigCoinRoll_c::onDraw() {
	InitialModel.scheduleForDrawing();

	return true;
}

void daBigCoinRoll_c::blockWasHit(bool isDown) {}

void daBigCoinRoll_c::beginState_Wait() {}
void daBigCoinRoll_c::executeState_Wait() {}
void daBigCoinRoll_c::endState_Wait() {}

void daBigCoinRoll_c::beginState_Hit() {}
void daBigCoinRoll_c::executeState_Hit() {}
void daBigCoinRoll_c::endState_Hit() {}