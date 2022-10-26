#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>


const char* freezeflame_switch [] = { "freezeflame_switch", NULL };

class dFreezeFlameSwitch_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	int timer;

	static dActor_c* build();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
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
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(dFreezeFlameSwitch_c);
	DECLARE_STATE(Initial);
};

CREATE_STATE(dFreezeFlameSwitch_c, Initial);

void dFreezeFlameSwitch_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* dFreezeFlameSwitch_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dFreezeFlameSwitch_c));
	return new(buffer) dFreezeFlameSwitch_c;
}

const SpriteData SwitchFrFlSpriteData = 
{ ProfileId::95, 0, 0, 0, 0, 0x400, 0x400, 0, 0, 0, 0, 0 };

Profile SwitchFrFlProfile(&dFreezeFlameSwitch_c::build, SpriteId::501, &SwitchFrFlSpriteData, ProfileId::95, ProfileId::95, "SwitchFrFl", freezeflame_switch);

bool dFreezeFlameSwitch_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther){
	PlaySoundAsync(this, SE_OBJ_STEP_ON_KSWITCH);
	Vec efPos = {this->pos.x + 28.0f, this->pos.y + 56.0f, this->pos.z + 2000.0f};
	S16Vec nullRot = {0,0,0};
	Vec efScale = {4.0f, 4.0f, 4.0f};
	SpawnEffect("Wm_ob_switch01", 0, &efPos, &nullRot, &efScale);
	doStateChange(&StateID_Initial);
}

bool dFreezeFlameSwitch_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
}

int dFreezeFlameSwitch_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	
	int Model = settings >> 28 & 0xF;
	
	if (Model > 0) {
		resFile.data = getResource("freezeflame_switch", "g3d/freezeflame_switch.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("freezeflame_switch_ice");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);
	} else {
		resFile.data = getResource("freezeflame_switch", "g3d/freezeflame_switch.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("freezeflame_switch_fire");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);
	}
	
	
	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;


	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.36; 
	HitMeBaby.xDistToEdge = 0.32; 
	HitMeBaby.yDistToEdge = 0.48; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();






	this->onExecute();

	return true;
}

int dFreezeFlameSwitch_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int dFreezeFlameSwitch_c::onDelete() {
	return true;
}

int dFreezeFlameSwitch_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void dFreezeFlameSwitch_c::beginState_Initial() {
	this->timer = 0;
	MapSoundPlayer(SoundRelatedClass, SE_AMB_SABAKU_WIND, 1);
	layout.loadArc("TemperatureEffects.arc", true);
	
	static const char *TemperatureEffectOnceSwitched[6] = {
		"89_Effect_Ice_ComingUp.brlan",
		"89_Effect_Ice_Looping.brlan",
		"89_Effect_Ice_Finish.brlan",
		"89_Effect_Fire_ComingUp.brlan",
		"89_Effect_Fire_Looping.brlan",
		"89_Effect_Fire_Finish.brlan",
	};
	
	static const char *groupNames[6] = {
		"A00_IceEffectBegin",
		"A01_IceEffectMiddle",
		"A02_IceEffectDone",
		"B01_FireEffectBegin",
		"B01_FireEffectMiddle",
		"B02_FireEffectDone",
	};
	
	static const int groupIDs[6] = {
		0,
		1,
		2,
		3,
		4,
		5,
	};
	
	layout.build("89_Effect_Temperature.brlyt");

	layout.loadAnimations(TemperatureEffectOnceSwitched, 6);
	layout.loadGroups(groupNames, groupIDs, 6);
	layout.disableAllAnimations();
	
	//Is it okay with environment transformations ???
	
}

void dFreezeFlameSwitch_c::executeState_Initial() {
	if (Model > 0) {
		layout.enableNonLoopAnim(4);
		Stop();
		layout.enableNonLoopAnim(5);
		Stop();
		layout.enableNonLoopAnim(6);
	} else {
		layout.enableNonLoopAnim(1);
		Stop();
		layout.enableNonLoopAnim(2);
		Stop();
		layout.enableNonLoopAnim(3);
	}
}
void dFreezeFlameSwitch_c::endState_Initial() {}