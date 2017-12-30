#include "Floor.hpp"
#include "Room.hpp"
#include <fitd.h>
#include <resource.h>
#include <pak.h>

int getNumCameras(char* data) {
	return ((READ_LE_UINT32(data)) / 4);
}

int getNumRooms(char* data, int data_size) {
	int j = 0;
	int num_max = (((READ_LE_UINT32(data)) / 4));
	for(int i = 0; i < num_max; i++) {
		if(data_size >= READ_LE_UINT32(data + i * 4)) {
			j++;
		} else {
			return j;
		}
	}
	return j;
}

using namespace Fitd;
void Floor::load(int floor_idx) {

	char floor_file[256];
	sprintf(floor_file, "ETAGE%02d", floor_idx);
	char *floor_data = g_resourceLoader->loadPakSafe(floor_file, 0);
	char *camera_data = g_resourceLoader->loadPakSafe(floor_file, 1);

	int floor_data_size = getPakSize(floor_file, 0);
	int camera_data_size = getPakSize(floor_file, 1);

	int num_rooms = getNumRooms(floor_data, floor_data_size);

	//TODO: spdlog
	printf("Found %d rooms in floor %d\n", num_rooms, floor_idx);

	// Load rooms
	for (int i = 0; i < num_rooms; i++) {
		char* room_data = (floor_data + READ_LE_UINT32(floor_data + i * 4));
		Room::Ptr room = Room::Ptr(new Room());
		room->load(room_data);
		room_vector.push_back(room);
	}

	// Load cameras
	int num_cameras = getNumCameras(camera_data);
	for (int i = 0; i < num_cameras; i++) {
		uint32 offset = READ_LE_UINT32(camera_data + i * 4);
		if (offset < camera_data_size) {
			char *current_camera_data = camera_data + offset;
			RoomCamera::Ptr camera = RoomCamera::Ptr(new RoomCamera(current_camera_data));
			camera_vector.push_back(camera);
		}
	}
	
}
