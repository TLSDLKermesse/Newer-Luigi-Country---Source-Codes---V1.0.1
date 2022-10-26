#include <common.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"
//#include "KingBoo.h" (unexisting maybe)
//#include "msgbox.h" (useless)


const char* King_Teresa [] = { "King_Teresa", NULL };

class TheKingBoo : public KingBoo {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void setupModels();

	static dActor_c* build();

	char damage;
	char isDown;
	int timer;
	bool isBouncing;
	int HeyLetMeDontYouSeeImHurt;
	
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	void addScoreWhenHit(void *other);
	
	bool isNearWall();
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::mdl_c hurtModel;	//Maybe works?
	m3d::mdl_c angryModel;	//Same?
	m3d::mdl_c fogModel;
	m3d::anmChr_c animationChr;
	
	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c anmFog;
	m3d::anmChr_c anmA;
	m3d::anmChr_c anmB;
	
	nw4r::g3d::ResAnmTexSrt resTexSrt;
	m3d::anmTexSrt_c fogSrt;

	USING_STATES(TheKingBoo);
	DECLARE_STATE(Intro);
	DECLARE_STATE(Laugh);
	DECLARE_STATE(Move);
	DECLARE_STATE(RandomAttack);
	DECLARE_STATE(Stomp);
	DECLARE_STATE(Drop);
	DECLARE_STATE(ThunderAttack);
	DECLARE_STATE(Damage);
	DECLARE_STATE(DamageLast);
	DECLARE_STATE(Turn);
	DECLARE_STATE(Outro);
};

CREATE_STATE(TheKingBoo, Intro);
CREATE_STATE(TheKingBoo, Laugh);
CREATE_STATE(TheKingBoo, Move);
CREATE_STATE(TheKingBoo, RandomAttack);
CREATE_STATE(TheKingBoo, Stomp);
CREATE_STATE(TheKingBoo, Drop);
CREATE_STATE(TheKingBoo, ThunderAttack);
CREATE_STATE(TheKingBoo, Damage);
CREATE_STATE(TheKingBoo, DamageLast);
CREATE_STATE(TheKingBoo, Turn);
CREATE_STATE(TheKingBoo, Outro);

void TheKingBoo::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	
	matrix.translation(pos.x + 160.0, pos.y + (scale.x * 12.0) - 80.0, pos.z);
	//idk if this still will work :|
	
	fogModel.setDrawMatrix(matrix);
	fogModel.setScale(&scale);
	fogModel.calcWorld(false);
	
	hurtModel.setDrawMatrix(matrix);
	hurtModel.setScale(&scale);
	hurtModel.calcWorld(false);
	
	angryModel.setDrawMatrix(matrix);
	angryModel.setScale(&scale);
	angryModel.calcWorld(false);
	
}

void TheKingBoo::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
	
	this->animationChr.bind(&this->hurtModel, anmChr, unk);
	this->hurtModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
	
	this->animationChr.bind(&this->fogModel, anmChr, unk);
	this->fogModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate); //(Are these 3 orders necessary for that model?)
	
	this->animationChr.bind(&this->angryModel, anmChr, unk);
	this->angryModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

void TheKingBoo::setupModels() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("fog");
	this->fogModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&this->fogModel, 0);

	nw4r::g3d::ResMdl mdlB = this->resFile.GetResMdl("king_boo_hurt");
	this->hurtModel.setup(mdlB, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&this->hurtModel, 0);
	
	nw4r::g3d::ResMdl mdlB = this->resFile.GetResMdl("king_boo_angry");
	this->angryModel.setup(mdlB, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&this->angryModel, 0);

	nw4r::g3d::ResAnmChr anmChrC = this->resFile.GetResAnmChr("shay_teresaA");
	ret = this->anmA.setup(mdlB, anmChrC, &this->allocator, 0);

	nw4r::g3d::ResAnmChr anmChrE = this->resFile.GetResAnmChr("shay_teresaB");
	ret = this->anmB.setup(mdlC, anmChrE, &this->allocator, 0);

	nw4r::g3d::ResAnmTexSrt anmSrt = this->resFile.GetResAnmTexSrt("fog");
	this->resTexSrt = anmSrt;
	//setup(ResMdl mdl, ResAnmTexSrt anmSrt, mAllocator* allocator, void* NULL, int count);
	ret = this->fogSrt.setup(mdl, anmSrt, &this->allocator, 0, 1);
	//setEntryByte34(char toSet, int which);
	this->fogSrt.setEntryByte34(0, 0);

	allocator.unlink();
	
	///////////////////////////////////
	//PLEASE CHECK THIS FUNCTION CUZ I HAVEN'T UNDERSTOOD ITS CONTENTS.
	///////////////////////////////////
}

void TheKingBoo::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	
	dStateBase_c *whatState = acState.getCurrentState();
	if (whatState == &StateID_Damage || Laugh || RandomAttack || DamageLast) {
		// does nothing (he'll not calculate hitType if one of these states above are played)
	} else {
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);
	}

	if(hitType == 1 || hitType == 2) {	// regular jump
		MapSoundPlayer(SoundRelatedClass, SE_EMY_KURIBO_L_DAMAGE_01, 1);
		bindAnimChr_and_setUpdateRate("Boss_F_NoDamage", 1, 0.0, 1.0);
		while (!this->animationChr.isAnimationDone()){
			Stop(1);
		}
	} else if (hitType > 0) {
		apOther->someFlagByte |= 2;
		if (this->isDown == 0) {
			if (damage >= 15) {
				doStateChange(DamageLast); }
			else {
				damage += 3;
				doStateChange(Damage);
			}
		} else if(hitType == 3) {
			apOther->someFlagByte |= 2;
			if (this->damage >= 15) {
				doStateChange(DamageLast);
			} else {
				damage += 3;
				doStateChange(Damage);
			}
		}
	} else if(hitType == 0) { //A little problem with conditions :(
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
}

bool TheKingBoo::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	damage = 15;
	this->doStateChange(&StateID_DamageLast);
}

bool TheKingBoo::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
}

bool TheKingBoo::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
}

bool TheKingBoo::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	apOther->someFlagByte |= 2;

	if (this->isDown == 0) {
		damage += 3;
		if (damage >= 15) { doStateChange(&StateID_DamageLast); }
		else { doStateChange(&StateID_Damage); }
	}

	deathInfo.isDead = 0;
	this->flags_4FC |= (1<<(31-7));
	this->counter_504[apOther->owner->which_player] = 5; //What is that?
	bouncePlayerWhenJumpedOn(apOther->owner);
	return true;
}

bool TheKingBoo::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat7_GroundPound(apThis, apOther);
}

dActor_c* TheKingBoo::build() {
	void *buffer = AllocFromGameHeap1(sizeof(TheKingBoo));
	return new(buffer) TheKingBoo;
}

const SpriteData KingBooSpriteData = 
{ ProfileId::76, 0, 0, -5, 0, 0x600, 0x600, 0, 0, 0, 0, 0 };

Profile KingBooProfile(&TheKingBoo::build, SpriteId::483, &KingBooSpriteData, ProfileId::76, ProfileId::76, "KingBoo", King_Teresa);

void TheKingBoo::addScoreWhenHit(void *other) { }


int TheKingBoo::onCreate() {
	
	setupModels();
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("King_Teresa", "g3d/King_Teresa.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("king_boo");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);
	
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("fog");
	this->anmFog.bind(&this->fogModel, anmChr, 1);
	this->fogModel.bindAnim(&this->anmFog, 0.0);
	this->anmFog.setUpdateRate(1.0);
	
	nw4r::g3d::ResAnmTexSrt anmSrt = this->resFile.GetResAnmTexSrt("fog");
	this->fogSrt.bindEntry(&this->fogModel, anmSrt, 0, 1);
	this->fogModel.bindAnim(&this->fogSrt, 1.0);
	this->fogSrt.setFrameForEntry(1.0, 0);
	this->fogSrt.setUpdateRateForEntry(1.0, 0);
	//Fog stuff were added from Boo model. I hope there're no errors here also.

	allocator.unlink(); 

	this->scale.x = 7.0; 
	this->scale.y = 7.0; 
	this->scale.z = 7.0;

	int MessageFirst = settings >> 20 & 0xFFF;
	int MessageLast = settings >> 4 & 0xFFFF;
	int delay = 0;
	
	HeyLetMeDontYouSeeImHurt = 0;
	
	float finalXspeed = 4.001;
	float finalYspeed = 2.001;
	
	ActivePhysics::Info GhostsPhysics; 
	GhostsPhysics.xDistToCenter = 0.0; 
	GhostsPhysics.yDistToCenter = 36.0; 
	GhostsPhysics.xDistToEdge = 48.0; 
	GhostsPhysics.yDistToEdge = 72.0; 
	GhostsPhysics.category1 = 0x3; 
	GhostsPhysics.category2 = 0x0; 
	GhostsPhysics.bitfield1 = 0x4F; 
	GhostsPhysics.bitfield2 = 0xFFC5B8FF; 
	GhostsPhysics.unkShort1C = 0; 
	GhostsPhysics.callback = &dEn_c::collisionCallback;
	
	this->aPhysics.initWithStruct(this, &GhostsPhysics); 
	this->aPhysics.addToList();

	//bindAnimChr_and_setUpdateRate("", 1, 0.0, 1.0); No!

	
	cmgr_returnValue = collMgr.isOnTopOfTile();
	
	doStateChange(&StateID_Intro);

	this->onExecute();

	return true;
}

int TheKingBoo::onExecute() {
	acState.execute();
	
	this->fogModel._vf1C();

	updateModelMatrices();
	if (HeyLetMeDontYouSeeImHurt == 1) {hurtModel._vf1C();
	} else if (HeyLetMeDontYouSeeImHurt == 2) {angryModel._vf1C();
	} else {bodyModel._vf1C();}

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	
	if(this->anmFog.isAnimationDone()) //Copied
		this->anmFog.setCurrentFrame(0.0);

	this->fogSrt.process();
	if(this->fogSrt.isEntryAnimationDone(0))
		this->fogSrt.setFrameForEntry(1.0, 0);//Copied!!

	return true;
}

int TheKingBoo::onDelete() {
	return true;
}

int TheKingBoo::onDraw() {

	if (this->HeyLetMeDontYouSeeImHurt == 1) {
		hurtModel.scheduleForDrawing(); }
	else if (this->HeyLetMeDontYouSeeImHurt == 2) {
		angryModel.scheduleForDrawing(); }
	else {bodyModel.scheduleForDrawing();}
	
	return true;
}

void TheKingBoo::beginState_Intro() {
	this->timer = 0;
	
	StopBGMMusic();
	
	dStage32C_c::instance->freezeMarioBossFlag = 2;
	WLClass::instance->_4 = 4;
	WLClass::instance->_8 = 0;
	
	rot.y = 0x2800; // Y is horizontal axis
	speed.x = 0.0;
	speed.y = 0.0;

	pos.x = pos.x - 400.0;
	pos.y = pos.y - 70.0;
}

void TheKingBoo::executeState_Intro() {

	this->timer += 1;

	OSReport("Timer: %d", timer);
	if (timer == 310) { bindAnimChr_and_setUpdateRate("Boss_AppearFromLeft", 1, 0.0, 1.0); }

	if ((timer > 310) && (timer < 350)) {
		MapSoundPlayer(SoundRelatedClass, SE_BOSS_CS_KAMECK_FLY, 1);
		pos.x += 6.0;
		pos.y -= 0.25;
	}
	
	if ((timer > 350) && (timer < 390)) {
		MapSoundPlayer(SoundRelatedClass, SE_BOSS_CS_KAMECK_FLY, 1);
		pos.x += 6.0;
		pos.y += 1.0;
	}
	
	if ((timer > 390) && (timer < 430)) {
		MapSoundPlayer(SoundRelatedClass, SE_BOSS_CS_KAMECK_FLY_SHIP, 1);
		pos.x += 6.0;
		pos.y += 6.0;
	}
		
	if (timer == 490) { bindAnimChr_and_setUpdateRate("Boss_Laugh", 1, 0.0, 1.0); }
		
	if (timer == 430) {
		this->direction = 1; //left
		doStateChange(&StateID_Turn);
	}
		
	if (done) {
		doStateChange(&StateID_Laugh);
	}
}
void TheKingBoo::endState_Intro() {}

void TheKingBoo::beginState_Laugh() {
	
	Stop(10);
	extern "C" bool SpawnEffect(const char *KingBooVoice[1], int, Vec*, S16Vec*, Vec*);
	//Play custom sounds listed at the bottom
	bindAnimChr_and_setUpdateRate("Boss_Laugh", 1, 0.0, 1.0);
	
	if (this->animationChr.isAnimationDone()){
		Stop(10);
		bindAnimChr_and_setUpdateRate("Boss_WaitedFor", 1, 0.0, 1.0);
		extern "C" bool SpawnEffect(const char *KingBooVoice[8], int, Vec*, S16Vec*, Vec*);
		Stop(12);
		
		while (!this->animationChr.isAnimationDone()){
			//Do nothing, just wait until the condition becomes false :)
		}
	}
}
void TheKingBoo::executeState_Laugh() {
	
	delay = 270;
	int DialogNum = 4;
	
	if (MessageFirst < MessageLast) {
		
		for (MessageFirst = settings >> 20 & 0xFFF; MessageFirst <= MessageLast; MessageFirst++) {
		
			extern "C" bool SpawnEffect(const char *KingBooVoice[DialogNum], int, Vec*, S16Vec*, Vec*);
			Stop(20);
			dMsgBoxManager_c::instance->showMessage(MessageFirst, true, delay); //I'm not sure :|
			
			if (DialogNum < 9) {
				DialogNum++;
			}
			
		}
		
	} else {
		
		for (MessageLast = settings >> 4 & 0xFFFF; MessageLast <= MessageFirst; MessageLast++) {
			
			extern "C" bool SpawnEffect(const char *KingBooVoice[DialogNum], int, Vec*, S16Vec*, Vec*);
			Stop(20);
			dMsgBoxManager_c::instance->showMessage(MessageLast, true, delay);
			//King Boo'll display some message, you're compulsory to start a conversation with Luigi XD
			
			if (DialogNum < 9) {
				DialogNum++;
			}
			
		}
	}
}
void TheKingBoo::endState_Laugh() {
	
	Stop(10);
	extern "C" bool SpawnEffect(const char *KingBooVoice[1], int, Vec*, S16Vec*, Vec*);
	bindAnimChr_and_setUpdateRate("Boss_Laugh", 1, 0.0, 1.0);
	
	while (!this->animationChr.isAnimationDone()){
		//Suffer eternally until you finish that ******** animation
	}
	
	Stop(20);
	this->doStateChange(&StateID_Move);
}

void TheKingBoo::beginState_Move() {
	
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("king_boo");
	//Will use this model. Is it correctly written?
	bindAnimChr_and_setUpdateRate("Boss_F_Move", 1, 0.0, 1.0);
	this->pos.y = 150;
	HeyLetMeDontYouSeeImHurt = 0;
	
}
void TheKingBoo::executeState_Move() {
	speed.x = (this->direction) ? -this->1.5 : this->1.5;
	this->timer = 0;
	
	for (timer = 0; timer < 300; timer++) {
		//Please correct this loop condition's syntax if it's wrong.
		
		if (timer < 60 || ((timer > 120) && (timer < 180)) || timer > 240)
			speed.y = 0.3;
		} else {
			speed.y = -0.3;
		}
		
	}
	
	this->doStateChange(&StateID_RandomAttack);
	
}
void TheKingBoo::endState_Move() {}

void TheKingBoo::beginState_RandomAttack() {}
void TheKingBoo::executeState_RandomAttack() {
	//Play custom sound
	int AttackNum = 0;
	//I created a new variable
	srand (time(NULL));
	AttackNum = rand() % 3 + 1;
	//Choses a random attack
	
	if (AttackNum == 2) {
		this->doStateChange(&StateID_Drop);
	} else if (AttackNum == 3) {
		this->doStateChange(&StateID_ThunderAttack);
	} else {
		this->doStateChange(&StateID_Stomp);
	}
}
void TheKingBoo::endState_RandomAttack() {}

void TheKingBoo::beginState_Stomp() {
	this->timer = 0;
	if (timer == 0) { bindAnimChr_and_setUpdateRate("Boss_F_StompS", 1, 0.0, 1.0); }
	
	MapSoundPlayer(SoundRelatedClass, SE_PLY_SPIN_ONCE, 1);
	this->timer += 1;
	
}
void TheKingBoo::executeState_Stomp() {
	this->timer = 0;
	if (timer == 0) { bindAnimChr_and_setUpdateRate("Boss_F_Stomp", 1, 0.0, 1.0); }
	
	MapSoundPlayer(SoundRelatedClass, SE_PLY_PRPL_LETDOWN_FAST, 1);
	this->timer += 1;
	//To do endstate_stomp he must repeat the animation until he touches the ground.
	
	while (!collMgr.isOnTopOfTile) {
	
		speed.x = 0.0;
		speed.y = -4.0;
		
	}

}
void TheKingBoo::endState_Stomp() {
	
	this->timer = 0;
	speed.x = 0.0;
	speed.y = 0.0;
	if (timer == 0) {
		MapSoundPlayer(SoundRelatedClass, SE_PLY_PRPL_LETDOWN_FAST_LAND, 1);
		bindAnimChr_and_setUpdateRate("Boss_F_StompL", 1, 0.0, 1.0);
	}
	
	if ((timer > 50) && (pos.y < 150)) {
		speed.y = 1.0;
	} else {
		speed.y = 0.0;
	}
	
	this->timer += 1;
	
	this->doStateChange(&StateID_Move);
	
}

void TheKingBoo::beginState_Drop() {
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[2], int, Vec*, S16Vec*, Vec*);
	
	bindAnimChr_and_setUpdateRate("Boss_F_Drop", 1, 0.0, 1.0);
	Stop(30);
	
}
void TheKingBoo::executeState_Drop() {
	
	//Create and use bombs
	
	for (int iii = 0; iii < 3; iii++) {
		
		BobOmb = (daBomb_c*)createChild(EN_BOMHEI, (dStageActor_c*)this, 0, &BombPos, &BombRot, 0);
		///////////////////////////////////HOW IS IT POSSIBLE? I DON'T HAVE THE NAME OF BOB-OMB'S CLASS!!!!(I've put a random name)
		//No idea also about its state ID names :( I think I need to decompile REL files but I need help with it.
		BobOmb->pos.x = this->pos.x;
		BobOmb->pos.y = this->pos.y - 32.0;
		BobOmb->pos.z = BobOmb->-1000.0;
		
	}
	
	Stop(30);
	
}
void TheKingBoo::endState_Drop() {this->doStateChange(&StateID_Move);}

void TheKingBoo::beginState_ThunderAttack() {
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[2], int, Vec*, S16Vec*, Vec*);
	
	speed.x = 0.0;
	speed.y = 0.0;
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("Boss_F_Thunderattack", 1, 0.0, 1.0);
	
	for (timer = 0; timer < 90; timer++) {
		speed.y = 0.0;
		speed.x = 0.0;
	}
	
}
void TheKingBoo::executeState_ThunderAttack() {
	
	for (int iii = 0; iii < 3; iii++) {
		
		int spawnposX = 48.0;
		
		BornThunderCloud = (dThunderCloud*)createChild(WM_BUBBLE, (dStageActor_c*)this, 0, &ThunderPos, &ThunderRot, 0);
		BornThunderCloud->doStateChange(&dThunderCloud::StateID_Lightning);
		BornThunderCloud->pos.x = BornThunderCloud->spawnposX;
		BornThunderCloud->pos.y = BornThunderCloud->224.0;
		BornThunderCloud->Baseline = BornThunderCloud->pos.y; //Maybe unecessary in mind
		
		spawnposX += 80.0;
		
	}
}
void TheKingBoo::endState_ThunderAttack() {this->doStateChange(&StateID_Move);}

void TheKingBoo::beginState_Damage() {

	HeyLetMeDontYouSeeImHurt = 1;
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[9], int, Vec*, S16Vec*, Vec*);
	// ------------------------------------------------------------------------------
	// ----------------LOOK HERE-----------------I WANT TO MAKE SPAWNING THIS SFX: sfx/5#_KING_TERESA_STOMPED--
	// BUT I DON'T KNOW HOW TO DO THIS. THAT KING OF PROGRAMMING SOUND IS A SPECIAL CASE. PLEASE HELP ME AND I'LL GIVE YOUR NICKNAME IN CREDITS
	bindAnimChr_and_setUpdateRate("Boss_F_Damage", 1, 0.0, 1.0);
	this->timer = 0;
	
	for (timer = 0; timer < 40; timer++) {
		speed.x = 0.0;
		speed.y = 4.0;
	}
	
	speed.x = 5.0;
	speed.y = 0.0;

	
	this->timer = 40;
}
void TheKingBoo::executeState_Damage() {

	if ((this->timer > 39.0) && (this->timer < 80.0)) {
		MapSoundPlayer(SoundRelatedClass, SE_BOSS_JR_NOW_DAMAGING, 1);
		S16Vec nullRot = {0,0,0};
		//In the hit state he's gonna play 2 hit effects which may work differently, if it's wrongly programed, CORRECT IT pls//
		Vec hitPos = {pos.x - 133.0f, pos.y + 70.0f, pos.z};
		Vec oneVec = {7.0f, 7.0f, 7.0f};
		SpawnEffect("Wm_en_hit", 0, &hitPos, &nullRot, &oneVec);
		scale = (Vec){7.0, 7.0, 7.0};
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		effect.spawn("Wm_mo_damage", 0, &efPos, &nullRot, &scale);
	}

	if ((this->timer > 80.0) && (this->timer < 120.0)) {
		scale = (Vec){7.0, 7.0, 7.0};
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		effect.spawn("Wm_mo_damage_a", 0, &efPos, &nullRot, &scale);
	}
	
	if ((this->timer > 120.0) && (this->timer < 150.0)) {
		scale = (Vec){7.0, 7.0, 7.0};
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		effect.spawn("Wm_mo_damage_b", 0, &efPos, &nullRot, &scale);
	}

	if ((this->timer > 150.0) && (this->timer < 170.0)) {
		scale = (Vec){7.0, 7.0, 7.0};
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		effect.spawn("Wm_mo_damage_c", 0, &efPos, &nullRot, &scale);
	}
	
	this-> timer += 1;
	
}
void TheKingBoo::endState_Damage() {
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[10], int, Vec*, S16Vec*, Vec*);
	
	while (pos.x < 0.0) {
		pos.y = 120;
		pos.x += 5.0;
		//He'll move right until X reaches 0//
	}
	
	this->doStateChange(&StateID_Move);
}

void TheKingBoo::beginState_DamageLast() {
	
	HeyLetMeDontYouSeeImHurt = 1;
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[9], int, Vec*, S16Vec*, Vec*);
	
	bindAnimChr_and_setUpdateRate("Boss_F_Damage", 1, 0.0, 1.0);
	while (!this->animationChr.isAnimationDone()){
		//Same thing
	}
	
	while (this->pos.y < -8.0) {
		this->speed.x = 0.0;
		this->speed.y = 4.0;
	}
	
	while (this->pos.y > 8.0) {
		this->speed.x = 0.0;
		this->speed.y = -4.0;
	}
	
	this->speed.y = 0.0;
	this->timer = 0;
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[10], int, Vec*, S16Vec*, Vec*);
	
	bindAnimChr_and_setUpdateRate("Boss_F_DamageLast", 1, 0.0, 1.0);
	aPhysics.removeFromList();	
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	
}
void TheKingBoo::executeState_DamageLast() {
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	
	for (timer = 0; timer < 5; timer++) {
		
		Stop(5);
		scale = (Vec){7.0, 7.0, 7.0};
		Vec efPos = {pos.x, pos.y, pos.z};
		S16Vec nullRot = {0,0,0};
		effect.spawn("Wm_mo_damage", 0, &efPos, &nullRot, &scale);
		Stop(5);
		effect.spawn("Wm_mo_damage_a", 0, &efPos, &nullRot, &scale);
		Stop(5);
		effect.spawn("Wm_mo_damage_b", 0, &efPos, &nullRot, &scale);
		Stop(5);
		effect.spawn("Wm_mo_damage_c", 0, &efPos, &nullRot, &scale);
		
	}
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[3], int, Vec*, S16Vec*, Vec*);
	
	for (int iii = 0; iii < 20; iii++) {
		
		finalYspeed = finalYspeed / 1.1;
		this->pos.x += finalXspeed;
		this->pos.y += finalYspeed;
		
	}
	
	finalYspeed = finalYspeed - finalYspeed * 2;
	
	for (iii = 0; iii < 100; iii++) {
		
		this->pos.x += finalXspeed;
		finalXspeed = finalXspeed / 1.05;
		this->pos.y += finalYspeed;
		finalYspeed = finalYspeed * 1.0875;
		
	}
	
	Stop(30);
	MapSoundPlayer(SoundRelatedClass, SE_BOSS_CMN_FAT_LAND_QUAKE, 1);
	
	for (iii = 0; iii < 5; iii++) {
		
		extern "C" void *ShakeScreen(void *, unsigned int, unsigned int, unsigned int, unsigned int);
		Stop(10);
		
	}
	
	Stop(30);
	
}
void TheKingBoo::endState_DamageLast() {}

void TheKingBoo::beginState_Turn() {
	bindAnimChr_and_setUpdateRate("Boss_F_Move", 1, 0.0, 0.5);
	this->speed.x = (direction) ? 0.5f : -0.5f;
	this->direction ^= 1;
	x_speed_inc = 0.0;
}
void TheKingBoo::executeState_Turn() {

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x1000);

	if(done) {
		this->doStateChange(&StateID_Move);
	}
}
void TheKingBoo::endState_Turn() {this->doStateChange(&StateID_Outro);}

void TheKingBoo::beginState_Outro() {
	
	this->timer = 0;
	this->rot.x = 0x0; // X is vertical axis
	this->rot.y = 0xE000; // Y is horizontal axis
	this->rot.z = 0x0; // Z is ... an axis >.>

	OutroSetup(this);
	
}
void TheKingBoo::executeState_Outro() {
	
	if (this->anmB.isAnimationDone())
		this->anmB.setCurrentFrame(0.0);

	PlayerVictoryCries(this);
	
	MapSoundPlayer(SoundRelatedClass, STRM_BGM_LAST_BOSS_CLEAR, 1);
	//BossGoalForAllPlayers(); I think,...not now. Don't forget the 2nd phase.

	Stop(180);
	
	//Revival
	
	HeyLetMeDontYouSeeImHurt = 2;
	extern "C" bool SpawnEffect(const char *KingBooVoice[2], int, Vec*, S16Vec*, Vec*);
	
	CREATE_STATE(dEndingMgr_c, LookUp); //What?
	acState.setState(dEndingMgr_c, &StateID_LookUp);///////////UH NOO!!
	
	Stop(60);
	
	this->pos.x = 200.0;
	this->pos.y = -400.0;
	
	while (this->pos.y < -8.0) {
		
		this->pos.y += 4.0;
		
	}
	
	this->rot.z = -0x0100; //Should look at Luigi (and co.)
	Stop(10);
	
	extern "C" bool SpawnEffect(const char *KingBooVoice[4], int, Vec*, S16Vec*, Vec*);
	
	Stop(20);
	
	//Create and use sprite [191] : Tile Spawner/Swapper
	////////////////IRREVELABLE NAME XD///////////////////////
	TileCreator = (daChengeBlock_c*)createChild(AC_CHENGE_BLOCK, (dStageActor_c*)this, 0, &TilePos, &TileRot, 0);
	//&Tilepos, &TileRot? What do they mean?
	TileCreator->doStateChange(daChengeBlock_c::StateID_doStuff(Destroy, false));
	TileCreator->pos.x = TileCreator-> -128.0;
	TileCreator->pos.y = TileCreator-> -80.0;
	TileCreator->blockType = 0;
	TileCreator->height = 15; //0xf, 0x15 or 15?
	TileCreator->width = 15;
	TileCreator->pattern = (Pattern)(0); //Bruh not sure again
	TileCreator->isPermanent = 1;
	TileCreator->Action = (Action)(0);
	//Should be done, huh?
	
}
void TheKingBoo::endState_Outro() {}

const char* KingBooVoice[10] = {
	"sfx/5#_KING_TERESA_APPEAR",
	"sfx/5#_KING_TERESA_ATTACK",
	"sfx/5#_KING_TERESA_DAMAGE_LAST",
	"sfx/5#_KING_TERESA_DIALOG1",
	"sfx/5#_KING_TERESA_DIALOG2",
	"sfx/5#_KING_TERESA_DIALOG3",
	"sfx/5#_KING_TERESA_DIALOG4",
	"sfx/5#_KING_TERESA_LAUGH",
	"sfx/5#_KING_TERESA_STOMP",
	"sfx/5#_KING_TERESA_STOMPED"
};

bool TheKingBoo::isNearWall() {
	// back up our current settings
	VEC3 savePos = pos;
	VEC3 saveSpeed = speed;
	int saveDirection = direction;

	float checkLeft = (direction == 0) ? 8.0f : 104.0f;
	float checkRight = (direction == 0) ? 104.0f : 8.0f;

	bool result = false;

	speed.x = -5.0f;
	speed.y = 0.0f;
	pos.x = savePos.x - checkLeft;
	direction = 1; // left
	if (collMgr.calculateAdjacentCollision())
		result = true;

	speed.x = 5.0f;
	speed.y = 0.0f;
	pos.x = savePos.x + checkRight;
	direction = 0; // right
	if (collMgr.calculateAdjacentCollision())
		result = true;

	// restore our settings
	pos = savePos;
	speed = saveSpeed;
	direction = saveDirection;

	return result;
}

bool TheKingBoo::isBouncing {
	
	if (whatState == &StateID_Damage || Laugh || RandomAttack || DamageLast) {
		stuffRelatingToCollisions(0.0, 0.0f, 0.0f);
	} else {
		GhostsPhysics.xDistToCenter = 0.0; 
		GhostsPhysics.yDistToCenter = 36.0; 
		GhostsPhysics.xDistToEdge = 48.0; 
		GhostsPhysics.yDistToEdge = 72.0;
		//Give back its normal physics
	}
	
}
//It's my first source code, so that's normal to get many comment at the top XD. There are many errors.