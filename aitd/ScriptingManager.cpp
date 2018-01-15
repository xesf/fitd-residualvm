#include "ScriptingManager.hpp"
#include "ObjectManager.hpp"
#include <hqr.h>


CommandId CommandMacroTable[] = {
	LM_DO_MOVE,
	LM_ANIM_ONCE,
	LM_ANIM_ALL_ONCE,
	LM_BODY,
	LM_IF_EGAL,
	LM_IF_DIFFERENT,
	LM_IF_SUP_EGAL,
	LM_IF_SUP,
	LM_IF_INF_EGAL,
	LM_IF_INF,
	LM_GOTO,
	LM_RETURN,
	LM_END,
	LM_ANIM_REPEAT,
	LM_ANIM_MOVE,
	LM_MOVE,
	LM_HIT,
	LM_MESSAGE,
	LM_MESSAGE_VALUE,
	LM_VAR,
	LM_INC,
	LM_DEC,
	LM_ADD,
	LM_SUB,
	LM_LIFE_MODE,
	LM_SWITCH,
	LM_CASE,
	LM_CAMERA,
	LM_START_CHRONO,
	LM_MULTI_CASE,
	LM_FOUND,
	LM_LIFE,
	LM_DELETE,
	LM_TAKE,
	LM_IN_HAND,
	LM_READ,
	LM_ANIM_SAMPLE,
	LM_SPECIAL,
	LM_DO_REAL_ZV,
	LM_SAMPLE,
	LM_TYPE,
	LM_GAME_OVER,
	LM_MANUAL_ROT,
	LM_RND_FREQ,
	LM_MUSIC,
	LM_SET_BETA,
	LM_DO_ROT_ZV,
	LM_STAGE,
	LM_FOUND_NAME,
	LM_FOUND_FLAG,
	LM_FOUND_LIFE,
	LM_CAMERA_TARGET,
	LM_DROP,
	LM_FIRE,
	LM_TEST_COL,
	LM_FOUND_BODY,
	LM_SET_ALPHA,
	LM_STOP_BETA,
	LM_DO_MAX_ZV,
	LM_PUT,
	LM_C_VAR,
	LM_DO_NORMAL_ZV,
	LM_DO_CARRE_ZV,
	LM_SAMPLE_THEN,
	LM_LIGHT,
	LM_SHAKING,
	LM_INVENTORY,
	LM_FOUND_WEIGHT,
	LM_UP_COOR_Y,
	LM_SPEED,
	LM_PUT_AT,
	LM_DEF_ZV,
	LM_HIT_OBJECT,
	LM_GET_HARD_CLIP,
	LM_ANGLE,
	LM_REP_SAMPLE,
	LM_THROW,
	LM_WATER,
	LM_PICTURE,
	LM_STOP_SAMPLE,
	LM_NEXT_MUSIC,
	LM_FADE_MUSIC,
	LM_STOP_HIT_OBJECT,
	LM_COPY_ANGLE,
	LM_END_SEQUENCE,
	LM_SAMPLE_THEN_REPEAT,
	LM_WAIT_GAME_OVER
};

ScriptingManager::ScriptingManager(EntityManager::Ptr em, World::Ptr w) :
	entity_manager(em),
	world(w)
{
	listLife = new Fitd::hqrEntryStruct("LISTLIFE", 10000, 100);
}

void ScriptingManager::loadScriptsFromObjects(const std::map<int, ObjectData> &object_map) {

	//pre-load all script indices
	std::vector<int16> life_ids;
	for (auto& obj : object_map) {
		if (obj.second.life != -1) {
			life_ids.push_back(obj.second.life);
		}
	}

	//remove duplicates and load
	std::unique(life_ids.begin(), life_ids.end());	
	for (auto idx : life_ids) {
		std::cout << "loading " << idx << std::endl;
		char *life_ptr = listLife->get(idx);
	 	script_map[idx] = loadScript(life_ptr);
	}
}

int16 getArg(char *life_ptr) {
	int16 arg = *life_ptr;
	life_ptr += 2;
	return arg;
}

#define ARG getArg(life_ptr);


/* Access to attributes of the current processed actor*/
int ScriptingManager::evalVar(char* life_ptr) {

	//CMD VAR1 VAR2
	//IF(VAR1 ==-1) ret VAR2
	//IF(VAR1 == 0) ret globals[VAR2]

	
	int16 var1 = getArg(life_ptr); 

	if (var1 == -1) {
		int16 var2 = getArg(life_ptr);
		return var2;
	}
	else if (var1 == 0) {
		int16 var2 = getArg(life_ptr);
//		return varTable[var2];
	}
	else {

		//if 5th most significant bit is active. (is gonna query about another actor?). 
		if(var1 & 0x8000) { 
			int16 object_id = getArg(life_ptr);
			int actor_id = ObjectManager::object_map[object_id].ownerIdx; // look for corresponding actor
			
			if(actor_id == -1) { //no owner of the object (actor not in room???)
				switch(var1 & 0x7FFFF) { //check the rest of bits
				case 0x1F: {
					return 0;//getObjectRoomIndex(object_id);
					break;
				}
				case 0x26: {
					return 0;//getObjectStageIndex(object_id);
					break;
				}
				default: {
					//	log_error("Unsupported code %X when actor not in room\n", var1 & 0x7FFF);
				}
				}
									
			}
		}

		var1 &= 0x7FFF;
		var1--;

		switch(var1) {
		case 0x0: { //return colliding actor (-1) otherwise

			break;
		}
		case 0x1: { //return HARD_DEC field (no idea what this does)
			
			break;
		}
		case 0x2: { //return HARD_COL field (hard collision - with environment?)

			break;
		}
		case 0x3: { //return HIT field (index in actor table of actor being hit)

		}
		case 0x4: { // HIT_BY field (actor that is hitting "us")

		}
		case 0x5: { // ANIM field (animation index?)

		}
		case 0x6: { // END_ANIM field. Signals animation has ended
			
		}
		case 0x7: { // FRAME field. key-frame index
			
		}	
		case 0x8: { // FRAME_END field. signals key frame change
			
		}
		case 0x9: { // BODY_NUM. Body index of current actor (from listBody buffer)

		}
		case 0xA: { // MARK. Current mark in the track (track control point?)

		}
		case 0xB: { // TRACK: id of track
			
		}
		case 0xC: { // CHRONO: 

		}
		case 0xD: { // ROOM CHRONO: 
					
		}
		case 0xE: { // Distance between current and object-actor

			int16 object_idx = getArg(life_ptr);
//			int actor_idx = objectTable[object_idx].ownerIdx;
			//if(actor_idx == -1) {
			//	return(32000);
			//}
			//else {
			//	
			//}
		}
		case 0xF: { // COL_BY actor collided by

		}
		case 0x10: { // Found
			//	return objectTable[evalVar(life_ptr)].flags2 & 0x8000;
			break;
		}
		case 0x11: { // User action
			
		}
		case 0x12: { // POSREL: get relative pos between two actors

		}
		case 0x13: { // User input

		}
		case 0x14: { // User button
		}
		case 0x15: { //COL_BY again?
		}
		case 0x16: { // actor alpha
		}
		case 0x17: { // actor beta			
		}
		case 0x18: { // actor gamma

		}
		case 0x19: { // IN_HAND object?
//			return(inHand);
			break;
		}
		case 0x1B: { // current camera????
//			return(*(uint16 *)(((currentCamera + 6) * 2) + cameraPtr));
			break;
		}
		case 0x1C: { // random number in range
			int16 range = getArg(life_ptr);
//			return randRange(0, range);
		}
		case 0x1D: { // falling flag

		}
		case 0x1E: { // room id

		}
		case 0x1F: { // life

		}
		case 0x20: { // query bits of object flags2
			int16 object_id = getArg(life_ptr);
		}
		case 0x21: { // room Y component
			
		}
		case 0x22: { // TEST_ZV_END_ANIM. Tests for location updates after anim
			int16 anim_id = getArg(life_ptr);
			int16 vertical_displacement = getArg(life_ptr);
			
		}
		case 0x23: { // TODO: music
			return(1);
			break;
		}
		case 0x24: { // Returns value of global variable
//			return globals[getArg(life_ptr)];
			break;
		}
		case 0x25: { // etage

		}
		case 0x26: { // THROW
			int16 object_id = getArg(life_ptr);
			// check flags
		}
		default: {
			//	error("Unhandled test type %X in evalVar\n", var1);
			break;
		}
		}
	}
}


void ScriptingManager::loadObjectCommand(CommandId command, char* life_ptr) {


	std::cout << "obj: " << command << std::endl;
	
/*	switch(command) {
	case LM_BODY: //body = var
		return OCmdSetBody(var);
		break;
	case LM_BODY_RESET: //body = var, anim = var
		return OCmdResetBody(arg1, arg2);
		break;
	case LM_TYPE: //flags = flags & (data1 & TYPE_MASK)
		return OCmdSetType(ARG, ARG);
		break;
	case LM_ANIM_ALL_ONCE: //anim = data1, animInfo = data2, animType = ANIM_ONCE
		return OCmdAnimAllOnce(ARG, ARG);
		break;
	case LM_ANIM_RESET:
		return OCmdAnimReset(ARG, ARG);
		break;
	case LM_MOVE:
		//etage, room
		return OCmdMove(ARG, ARG);
		break;
	case LM_ANGLE:
		//alpha, beta, gamma
		return OCmdAngle(ARG, ARG, ARG);
		break;
	case LM_STAGE:
		//stage, room, x, y, z
		return OCmdStage(ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_TEST_COL:
		return OCmdTestCol(ARG);
		break;
	case LM_LIFE:
		return OCmdSetLife(ARG);
		break;
	case LM_LIFE_MODE:
		return OCmdSetLifeMode(ARG);
		break;
	case LM_FOUND_NAME:
		return OCmdSetFoundName(ARG);
		break;
	case LM_FOUND_BODY:
		return OCmdSetFoundBody(ARG);
		break;
	case LM_FOUND_FLAG:
		return OCmdSetFoundFlag(ARG);
		break;
	case LM_FOUND_WEIGHT:
		return OCmdSetPosInTrack(ARG);
		break;
	case LM_START_CHRONO:
		break;
	default:
		std::cout << "Unsupported opcode" << std::endl;
		break;
	}	
	*/
}

void ScriptingManager::loadActionCommand(CommandId command, char* life_ptr) {

	std::cout << "action: " << command << std::endl;
	
	switch(command) {
/*	case LM_BODY: //body = var
		return ACmdSetBody(VAR);
		break;
	case LM_BODY_RESET: //body = var, anim = var
		return ACmdResetBody(VAR, VAR);
		break;
	case LM_DO_REAL_ZV:
		return ACmdDoRealZV();
		break;
	case LM_DEF_ZV:
		return ACMdDefineZV();
		break;
	case LM_DEF_ABS_ZV:
		return ACmdDefineAbsZV();
		break;
	case LM_DO_ROT_ZV:
		return ACmdDoRotZV();
		break;
	case LM_CARRE_ZV:
		return ACmdCarreZV();
		break;
	case LM_TYPE: //flags = flags & (data1 & TYPE_MASK)
		return ACmdSetType(ARG);
		break;
	case LM_GET_HARD_CLIP:
		return ACmdGetHardClip();
		break;
	case LM_ANIM_ONCE:
		return ACmdAnimOnce(ARG, ARG);
		break;
	case LM_ANIM_REPEAT:
		return ACmdAnimRepeat(ARG);
		break;
	case LM_ANIM_ALL_ONCE: //anim = data1, animInfo = data2, animType = ANIM_ONCE
		return ACmdAnimAllOnce(ARG, ARG);
		break;
	case LM_ANIM_RESET:
		return ACmdAnimReset(ARG, ARG);
		break;
	case LM_HYBRID_ONCE: //TODO
		life_ptr += 4;
		break;
	case LM_HYBRID_REPEAT: //TODO
		life_ptr += 4;
		break;
    case LM_HIT:
		return ACmdHit(ARG, ARG, ARG, ARG, VAR, ARG);
		break;
	case LM_FIRE:
		return ACmdFire(ARG, ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_HIT_OBJECT:
		return ACmdHitObject(ARG, ARG);
		break;
	case LM_STOP_HIT_OBJECT:
		return ACmdStopHitObject();		
		break;
	case LM_THROW:
		return ACmdThrow(ARG, ARG, ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_MOVE:
		//etage, room
		return ACmdMove(ARG, ARG);
		break;
	case LM_RESET_MOVE_MANUAL:
		return ACmdResetMoveManual();
		break;
	case LM_CONTINUE_TRACK:
		return ACmdContinueTrack(ARG);
		break;
	case LM_DO_MOVE:
		return ACmdDoMove();
		break;
	case LM_ANIM_MOVE:
		return ACmdAnimMove(ARG, ARG, ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_MANUAL_ROT:
		return ACmdManualRot();
		break;
	case LM_SET_BETA:
		return ACmdSetBetaRot(ARG, ARG);
		break;
	case LM_SET_ALPHA:
		return ACmdSetAlphaRot(ARG, ARG);
		break;
	case LM_ANGLE:
		//alpha, beta, gamma
		return ACmdAngle(ARG, ARG, ARG);
		break;
	case LM_COPY_ANGLE: //TODO
		life_ptr += 2;
		break;
	case LM_STAGE:
		//stage, room, x, y, z
		return ACmdStage(ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_TEST_COL:
		return ACmdTestCol(ARG);
		break;
	case LM_UP_COOR_Y:
		return ACmdUpCoorY();
		break;
	case LM_LIFE:
		return ACmdSetLife(ARG);
		break;
	case LM_STAGE_LIFE:
		return ACmdSetStageLife(ARG);
		break;
	case LM_LIFE_MODE:
		return ACmdSetLifeMode(ARG);
		break;
	case LM_DELETE:
		return ACmdDelete(ARG);
		break;
	case LM_SPECIAL:
		return ACmdSpecial(ARG);
		break;
	case LM_START_CHRONO:
		return ACmdStartChrono();
		break;
	case LM_FOUND:
		return ACmdFound(ARG);
		break;
	case LM_TAKE:
		return ACmdTake(ARG);
		break;
	case LM_IN_HAND:
		return ACmdInHand(ARG);
		break;
	case LM_DROP:
		return ACmdDrop(ARG);
		break;
	case LM_PUT:
		return ACmdPut(ARG, ARG, ARG, ARG, ARG, ARG, ARG, ARG, ARG);
		break;
	case LM_PUT_AT:
		return ACmdPutAt(ARG, ARG);
		break;
	case LM_FOUND_NAME:
		return ACmdSetFoundName(ARG);
		break;
	case LM_FOUND_BODY:
		return ACmdSetFoundBody(ARG);
		break;
	case LM_FOUND_FLAG:
		return ACmdSetFoundFlag(ARG);
		break;
	case LM_FOUND_WEIGHT:
		return ACmdSetPosInTrack(ARG);
		break;
	case LM_FOUND_LIFE:
		return ACmdSetFoundLife(ARG);
		break;
	case LM_READ:
		return ACmdRead(ARG, ARG);
		break;
	case LM_READ_ON_PICTURE:
		life_ptr += 2;
		break;
	case LM_ANIM_SAMPLE:
		return ACmdAnimSample(VAR, ARG, ARG);
		break;
	case LM_2D_ANIM_SAMPLE:
		return ACmd2DAnimSample(VAR);
		break;
	case LM_REP_SAMPLE:
		return ACmdRepSample(VAR, ARG);
		break;
	case LM_STOP_SAMPLE:
		//TODO
		break;
	case LM_SAMPLE_THEN:
		return AcmdSampleThen(VAR, VAR);
		break;
	case LM_SAMPLE_THEN_REPEAT:
		return ACmdSampleThenRepeat(VAR, VAR);
		break;
	case LM_MUSIC:
		return ACmdMusic(ARG);
		break;
	case LM_NEXT_MUSIC:
		return ACmdNextMusic(ARG);
		break;
	case LM_FADE_MUSIC:
		return ACmdFaceMusic(ARG);
		break;
	case LM_RND_FREQ: //TODO
		life_ptr += 2;
		break;
	case LM_LIGHT:
		return ACmdLight(ARG);
		break;
	case LM_SHAKING:
		return ACmdShaking(ARG);
		break;
	case LM_RAIN:
		break;
	case LM_WATER:
		ARG; //TODO
		break;
	case LM_CAMERA_TARGET:
		return ACmdCameraTarget(ARG);
		break;
	case LM_PICTURE:
		return ACmdPicture(ARG, ARG, ARG);
		break;
	case LM_PLAY_SEQUENCE:
		return ACmdPlaySequence(ARG, ARG, ARG);
		break;
	case LM_DEF_SEQ_SAMPLE:
		int16 num_params = ARG;
		std::vector<int16> params;
		for (int i = 0; i < num_params; i++) {
			params.push_back(ARG);
		}
		return ACmdDefSeqSample(params);
		break;
	case LM_PROTECT: //TODO
		break;
	case LM_INVENTORY:
		return ACmdInventory(ARG);
		break;
	case LM_SET_INVENTORY:
		return ACmdSetInventory(ARG);
		break;
	case LM_SET_GROUND:
		return ACmdSetGround(ARG);
		break;
	case LM_MESSAGE:
		return ACmdMessage(ARG);
		break;
	case LM_MESSAGE_VALUE:
		return ACmdMessageValue(ARG, ARG);
		break;
	case LM_END_SEQUENCE: //TODO
		break;
	case LM_VAR:
		break;
	case LM_INC:
		break;
	case LM_DEC:
		break;
	case LM_ADD:
		break;  
	case LM_SUB:
		break;			
	case LM_MODIF_C_VAR:
	case LM_C_VAR:
		break;			
	case LM_IF_EGAL:
		break;
	case LM_IF_DIFFERENT:
		break;
	case LM_IF_SUP_EGAL:
		break;
	case LM_IF_SUP:
		break;
	case LM_IF_INF_EGAL:
		break;
	case LM_IF_INF:
		break;
	case LM_GOTO:
		break;
	case LM_SWITCH:
		break;
	case LM_CASE: 
		break;
	case LM_MULTI_CASE:
	break;*/
	case LM_RETURN:
		finished_loading = true;
	 	break;
	case LM_END:
		finished_loading = true;
	 	break;
	 default:
	 	std::cout << "Unsupported opcode" << std::endl;
	 	break;
	 }
}

Script ScriptingManager::loadScript(char *life_ptr_src) {

	Script script;
	char* life_ptr = life_ptr_src;
	
	finished_loading = false;
	while(!finished_loading) {
	
		int16 current_opcode = *(int16 *)(life_ptr);
		life_ptr += 2;
		
		CommandId command = CommandMacroTable[current_opcode & 0x7FFF];

		if(current_opcode & 0x8000) {
			int16 object_idx = *(int16 *)life_ptr;
			life_ptr += 2;

			//	assert(object_idx != -1);
			if (object_idx != -1) {

				// if the object has owner, run as actorCommand instead
				int16 actor_idx = ObjectManager::object_map[object_idx].ownerIdx;

				std::cout << "actor:" << actor_idx << std::endl;
				
				if (actor_idx != -1) {
					loadObjectCommand(command, life_ptr);
				}
				else {
					loadActionCommand(command, life_ptr);
				}
			}
		}
		else {
			loadObjectCommand(command, life_ptr);
		}

		getchar();
//		script.command_vector.push_back();
	}

	return script;
}

void ScriptingManager::runScript(int idx) {
	std::cout << "Trying to run script: " << idx << std::endl;
}
