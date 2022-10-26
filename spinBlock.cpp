#include <common.h>
#include <g3dhax.h>
#include <stage.h>


const char* block_spin [] = { "block_spin", NULL };

class daSpinBlock_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	

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

	USING_STATES(daSpinBlock_c);
	DECLARE_STATE(Wait);
};

CREATE_STATE(daSpinBlock_c, Wait);

void daSpinBlock_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daSpinBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daSpinBlock_c));
	return new(buffer) daSpinBlock_c;
}

const SpriteData BlockSpinSpriteData = 
{ ProfileId::96, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile BlockSpinProfile(&daSpinBlock_c::build, SpriteId::504, &BlockSpinSpriteData, ProfileId::96, ProfileId::96, "BlockSpin", block_spin);

void daSpinBlock_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->anmFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
	//I'm sure of this, how to add CLR and SRT animations?
}

int daSpinBlock_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("block_spin", "g3d/block_spin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("block_spin");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	int BlockLength = settings >> 16 & 0xFF;
	int BlockHeight = settings >> 24 & 0xFF;
	int BlockDirection = settings >> 12 & 0xF;
	if (BlockDirection > 3) {
		BlockDirection = 0;
	}
	int TravelDistance = (settings >> 4 & 0xFF) * 16;
	int Delays = (settings & 0xF) * 60;
	float XSpeed = 0;
	int PixelCounter = 0;

	doStateChange(&StateID_Wait);



	this->onExecute();

	return true;
}

int daSpinBlock_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int daSpinBlock_c::onDelete() {
	return true;
}

int daSpinBlock_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daSpinBlock_c::beginState_Wait() {
	if (buttonPushed & 0x0100) {
		//If a player is spinning. Is it the right buttonpushed value?
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		Vec efScale = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_ob_startail", 0, &efPos, &nullRot, &efScale);
		for (TravelDistance = 0; TravelDistance >= (settings >> 4 & 0xFF) * 16; TravelDistance++) {
			if (BlockDirection == 3) {
				this->speed.x = 0.0;
				this->speed.y -= 4.0;
			} else if (BlockDirection == 1) {
				this->speed.x -= 4.0;
				this->speed.y = 0.0;
			} else if (BlockDirection == 2) {
				this->speed.x = 0.0;
				this->speed.y = 4.0;
			} else {
				this->speed.x = 4.0;
				this->speed.y = 0.0;
			}
		}
	}
}
void daSpinBlock_c::executeState_Wait() {
	ShakeScreen(ScreenPositionClass, 0, 1, 0, 0);
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	PlaySoundAsync(this, SE_EMY_BIG_DOSSUN);
	Stop(Delays);
	PixelCounter = 0;
	for (int i = 0; i < 32; i++) {
		XSpeed = 0.5;
		if (BlockDirection == 3) {
			this->speed.y -= XSpeed;
		} else if (BlockDirection == 1) {
			this->speed.x -= XSpeed;
		} else if (BlockDirection == 2) {
			this->speed.y = XSpeed;
		} else {
			this->speed.x = XSpeed;
		}
	}
	PixelCounter = 16;
	XSpeed = 1.0;
	for (int ii = 0; ii < 2; ii++) {
		if (PixelCounter < TravelDistance) {
			for (int i = 0; i < 16; i++) {
				if (BlockDirection == 3) {
					this->speed.y -= XSpeed;
				} else if (BlockDirection == 1) {
					this->speed.x -= XSpeed;
				} else if (BlockDirection == 2) {
					this->speed.y = XSpeed;
				} else {
					this->speed.x = XSpeed;
				}
			}
			PixelCounter += 16;
		}
	}
	XSpeed = 2.0;
	while (PixelCounter < TravelDistance) {
		if (BlockDirection == 3) {
			this->speed.y -= XSpeed;
		} else if (BlockDirection == 1) {
			this->speed.x -= XSpeed;
		} else if (BlockDirection == 2) {
			this->speed.y = XSpeed;
		} else {
			this->speed.x = XSpeed;
		}
		PixelCounter += 2;
	}
	PlaySoundAsync(this, SE_PLY_LAND_FENCE);
}
void daSpinBlock_c::endState_Wait() {}