#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>

class daMusicH : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	
	

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


dActor_c* daMusicH::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daMusicH));
	return new(buffer) daMusicH;
}

const SpriteData EnvironmentSFXsSpriteData = 
{ ProfileId::98, 0, 0, 0, 0, 0x000, 0x000, 0, 0, 0, 0, 0 };

Profile EnvironmentSFXsProfile(&daMusicH::build, SpriteId::505, &EnvironmentSFXsSpriteData, ProfileId::98, ProfileId::98, "EnvironmentSFXs", );


int daMusicH::onCreate() {

	int SFX_ID = settings >> 20 & 0xF;
	SFX_ID = SFX_ID + 1;
	
	if (SFX_ID > 8) {
		SFX_ID = 1;
	}
	
	this->onExecute();

	return true;
}

int daMusicH::onExecute() {
	extern "C" bool SpawnEffect(const char *BoomVoice [SFX_ID], int, Vec*, S16Vec*, Vec*);
	return true;
}

int daMusicH::onDelete() {
	return true;
}

int daMusicH::onDraw() {
	return true;
}

const char* EnvironmentSounds[8] = {
	"sfx/8#0_AMBIENCE_MORI_NIGHT"
	"sfx/8#1_AMBIENCE_SCENE"
	"sfx/8#2_AMBIENCE_CHIKA"
	"sfx/8#3_AMBIENCE_SANBASHI_STORM"
	"sfx/8#4_AMBIENCE_YUKI"
	"sfx/8#5_AMBIENCE_OBAKE"
	"sfx/8#6_AMBIENCE_SORA"
	"sfx/8#7_AMBIENCE_GAKE"
}