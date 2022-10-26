#include <common.h>
#include <g3dhax.h>
#include <stage.h>


const char* obj_chikuwa_block_big [] = { "obj_chikuwa_block_big", NULL };

class daFallBlock_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();


	void updateModelMatrices();
	void updatefallModelMatrices();
	
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	
	static dActor_c* build();
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	// Settings
	CollisionType collisionType;
	u8 rectID, moveSpeed, moveDirection, moveLength;

	u8 moveDelay, currentMoveDelay;

	bool doesMoveInfinitely;

	bool doTheIronEffect;
	bool dontPlayThisSongAgain;

	int timer;
	
	float moveMin, moveMax, moveDelta, moveBaseDelta;
	float *moveTarget;

	bool isMoving;
	void setupMovement();
	void handleMovement();

	Physics physics;
	StandOnTopCollider sotCollider;

	TileRenderer *renderers;
	int rendererCount;

	void findSourceArea();
	void createTiles();
	void deleteTiles();
	void updateTilePositions();

	void checkVisibility();
	void setVisible(bool shown);

	bool isVisible;

	int srcX, srcY;
	int width, height;
	
};


void daFallBlock_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daFallBlock_c::updatefallModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	fallModel.setDrawMatrix(matrix);
	fallModel.setScale(&scale);
	fallModel.calcWorld(false);
}

void daFallBlock_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther); {
	if (playerCollision) {
		updatefallModelMatrices();
		fallModel._vf1C();
		for (timer = 0; timer < 50; timer++) {
			if (playerCollision) {
				pos.x = pos.x + 0.1;
				Stop(1);
				pos.x = pos.x - 0.1;
				Stop(1);
			}
		}
		
		if (playerCollision) {
			for (timer = 0; timer < 200; timer++) {
				speed.y = -0.1;
			}
			
			Vec efPos = {pos.x + 7.0f, pos.y + 14.0f, pos.z - 5500.0f};
			S16Vec nullRot = {0,0,0};
			Vec efScale = {2.0f, 0.8f, 1.0f};
			SpawnEffect("Wm_mr_brakesmoke", 0, &efPos, &nullRot, &efScale);
			this->Delete(0);
		}
	}	
}

dActor_c* daFallBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFallBlock_c));
	return new(buffer) daFallBlock_c;
}

const SpriteData ResizedUnstableBlockSpriteData = 
{ ProfileId::93, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile ResizedUnstableBlockProfile(&daFallBlock_c::build, SpriteId::503, &ResizedUnstableBlockSpriteData, ProfileId::93, ProfileId::93, "ResizedUnstableBlock", obj_chikuwa_block_big);


int daFallBlock_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	int BlockLength = settings >> 28 & 0xF;
	
	if (BlockLength == 1) {
		resFile.data = getResource("obj_chikuwa_block_big", "g3d/obj_chikuwa_block_big.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("obj_chikuwa_block_tile3");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);
	} else {
		resFile.data = getResource("obj_chikuwa_block_big", "g3d/obj_chikuwa_block_big.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("obj_chikuwa_block_tile2");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);
	}
	
	if (BlockLength == 1) {
		resFile.data = getResource("obj_chikuwa_block_big", "g3d/obj_chikuwa_block_big.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("obj_chikuwa_block_tile3_fall");
		fallModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&fallModel, 0);
	} else {
		resFile.data = getResource("obj_chikuwa_block_big", "g3d/obj_chikuwa_block_big.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("obj_chikuwa_block_tile2_fall");
		fallModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&fallModel, 0);
	}
	
	allocator.unlink(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0;

	////Collision Creating///

	this->speed.y = 0.0;
	this->speed.x = 0.0;
	
	float fWidth = (BlockLength + 2) * 16.0f;
	
	sotCollider.init(this,
			/*xOffset=*/0.0f, /*yOffset=*/0.0f,
			/*topYOffset=*/0,
			/*rightSize=*/fWidth, /*leftSize=*/0,
			/*rotation=*/0, /*_45=*/1
			);

		// What is this for. I dunno
		sotCollider._47 = 0xA;
		sotCollider.flags = 0x80180 | 0xC00;

		sotCollider.addToList();
		
	///Collision Created///

	this->onExecute();

	return true;
}

int daFallBlock_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int daFallBlock_c::onDelete() {
	deleteTiles();

	switch (collisionType) {
		case ThinLineLeft: case ThinLineRight:
		case ThinLineTop: case ThinLineBottom:
		case Solid: physics.removeFromList(); break;
	}

	return 1;
}

int daFallBlock_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}