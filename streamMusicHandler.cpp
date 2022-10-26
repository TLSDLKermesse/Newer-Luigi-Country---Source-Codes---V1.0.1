#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>

class daStreamSound_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();
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
	};


dActor_c* daStreamSound_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daStreamSound_c));
	return new(buffer) daStreamSound_c;
}

const SpriteData MusicHandlerSpriteData = 
{ ProfileId::99, 0, 0, 0, 0, 0x000, 0x000, 0, 0, 0, 0, 0 };

Profile MusicHandlerProfile(&daStreamSound_c::build, SpriteId::491, &MusicHandlerSpriteData, ProfileId::99, ProfileId::99, "MusicHandler", );


int daStreamSound_c::onCreate() {

	u8 UseLocation = settings >> 20 & 0xFF;
	int MusicNumber = settings >> 12 & 0xFF;
	bool playsForever = settings >> 8 & 0xF;
	int StreamNumber = settings >> 28 & 0xF;
	bool EventInfluenced = dFlagMgr_c::instance->active(UseLocation);
	if (StreamNumber > 3) {
		StreamNumber = 0;
	}
	
	this->onExecute();

	return true;
}

int daStreamSound_c::onExecute() {
	this->timer = 0;

	while (EventInfluenced) {
		extern "C" bool SpawnEffect(const char *SongNameList [MusicNumber], int, Vec*, S16Vec*, Vec*);
		//But how to get music streams?
	}
	
	this->timer += 1;
	
	return true;
}

int daStreamSound_c::onDelete() {
	return true;
}

int daStreamSound_c::onDraw() {
	return true;
}