#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "raylib.h"
#include "raymath.h"

//ORIGINAL UNPLANNED SYSTEM
//struct TileAttr {
//	int id{-1};
//	Texture texture{};
//};
//
//class TileAttrManager {
//	static std::vector<TileAttr> attrs;
//
//	//Input values use C string format specifiers
//	//See: https://learningmonkey.in/wp-content/uploads/2021/05/Coversion-Specifiers-in-C.jpg 
//	static TileAttr makeAttr(const char* values) {
//		std::vector<const char*> vals;
//		
//
//	}
//
//public:
//	static TileAttr* getAttrPtr(size_t id) {
//		return &attrs.at(id);
//	}
//};
//
////Contains a ptr to TileAttr to save on memory for repeated tiles
//class Tile {
//	size_t id;
//	TileAttr* attributes; //stores constant data about the tile
//
//public:
//	Tile(size_t id) {
//		this->id = id;
//		attributes = TileAttrManager::getAttrPtr(id);
//	}
//};
//
//class Tilemap {
//	typedef std::unique_ptr<Tile> uptr_tile;
//	typedef std::vector<std::vector<uptr_tile>> matrix;
//
//	matrix map;
//public:
//	Tilemap(size_t width, size_t height) {
//		map.resize(height);
//		for (auto& row : map) {
//			row.resize(width, nullptr);
//		}
//	}
//
//	//get reference to uptr at provided indices
//	uptr_tile& at(size_t idx_x, size_t idx_y) {
//		return map.at(idx_y).at(idx_x);
//	}
//
//	//sets uptr to tile of input id
//	void place(size_t idx_x, size_t idx_y, size_t ID) {
//		if (!empty(idx_x, idx_y)) return;
//
//		at(idx_x, idx_y) = std::move(std::make_unique<Tile>(ID));
//	}
//
//	//resets uptr to nullptr at provided indices
//	void destroy(size_t idx_x, size_t idx_y) {
//		if (empty(idx_x, idx_y)) return;
//
//		at(idx_x, idx_y).reset();
//	}
//
//	//returns if target slot is nullptr
//	bool empty(size_t idx_x, size_t idx_y) {
//		return (at(idx_x, idx_y) == nullptr);
//	}
//};

//NEW PLANNED SYSTEM

//info for blocks
struct BlockInfo {
	uint16_t id;

	uint16_t max_hp;

	uint8_t tile_width;
	uint8_t tile_height;

	Texture2D texture;
};

class BlockInfoArray {
	static std::vector<BlockInfo> data;
public:
	//UNIMPLEMENTED
	static void load(const char* file) {
		//how to store data?
		//formatted strings?


	}

	static BlockInfo* fetch(uint16_t id) {
		return &data.at(id);
	}
};

//Hitboxes for blocks, 9 bytes
struct Tile {
	//Tile* block_root = this; //"this" indicates that the tile is the block's root. otherwise, set the ptr to the block root 
	uint16_t rootX = 0; //2 bytes
	uint16_t rootY = 0; //2 bytes
	uint16_t block_id = -1; //2 bytes, only set for block source tile

	static const uint8_t size = 32; // 1 byte
	bool occupied = false; //1 byte, block inhabits area but may not be the source of the block
	bool solid = false;   //1 byte
};

//Block
class Block {
	BlockInfo* info;
	uint16_t hp;
};

//Matrix of tiles
class Tilemap {
	typedef std::unique_ptr<Tile> uptr_Tile;
	
	std::vector<std::vector<uptr_Tile>> map;
public:
	//Tilemap width & height = pow(2, power), min 1, max 16
	Tilemap(uint8_t power) {
		int size = pow(2, Clamp(power, 1, 16));
		
		map.resize(size);
		for (auto& row : map) {
			row.resize(size);
		}
	}



	//BASIC FUNCTIONALITY:
	//~~~~~~~~~~~~~~~~~~~~
	
	//returns tile ptr ref at provided indices
	uptr_Tile& at(uint16_t x, uint16_t y) {
		return map.at(y).at(x);
	}

	//returns if a tile is occupied
	bool empty(uint16_t x, uint16_t y) {
		return at(x, y)->occupied; // if a tile is occupied, then it is not empty
	}

	//set targeted tile's block id
	void set_BlockID(uint16_t x, uint16_t y, uint16_t id) {
		at(x, y)->block_id = id;
	}
		
	//set targeted tile's occupied flag
	void set_occupied(uint16_t x, uint16_t y, bool occ) {
		at(x, y)->occupied = occ;
	}
		
	//set targeted tile's solid flag
	void set_solid(uint16_t x, uint16_t y, bool s) {
		at(x, y)->solid = s;
	}
	
	//returns if the input indices mark a valid tile index
	bool validTarget(uint16_t x, uint16_t y) {
		try {
			at(x, y);
			return true;
		}
		catch (std::out_of_range) {
			return false;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~



	//REGION MODIFICATION
	//~~~~~~~~~~~~~~~~~~~~

	//sets "occupied" flag for each tile in region. No bounds checking.
	void occupyRegion(uint16_t x, uint16_t y, uint8_t width, uint8_t height, bool occ) {
		for (int dY = 0; dY < height; dY++) {
			for (int dX = 0; dX < width; dX++) {
				set_occupied(x + dX, y + dY, occ);
			}
		}
	}

	//sets "solid" flag for each tile in region. No bounds checking.
	void solidRegion(uint16_t x, uint16_t y, uint8_t width, uint8_t height, bool s) {
		for (int dY = 0; dY < height; dY++) {
			for (int dX = 0; dX < width; dX++) {
				set_solid(x + dX, y + dY, s);
			}
		}
	}
		
	//returns true if the entire region is a valid tile
	bool validateRegion(uint16_t x, uint16_t y, uint8_t width, uint8_t height) {
		for (int dY = 0; dY < height; dY++) {
			for (int dX = 0; dX < width; dX++) {
				if (!validTarget(x + dX, y + dY)) return false;
			}
		}
	}



	//HIGH-LEVEL FUNCS
	//~~~~~~~~~~~~~~~~~~~~
	void Place(uint16_t id, uint16_t x, uint16_t y) {
		if (!empty(x, y)) return;
		BlockInfo* info = BlockInfoArray::fetch(id);

		//Rectangle region = { x * Tile::size, y * Tile::size, info->tile_width * Tile::size, info->tile_height * Tile::size };


		//transform block tile region to rectangle
		//transform tilemap into rectangle
		//if returned rectangle is not block, then consider out of bounds
		//Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2);
	}

	//
	void Destroy(uint16_t x, uint16_t y) {
		if (empty(x, y)) return;



	}

	//
	void Damage(uint32_t damage, uint16_t x, uint16_t y) {
		if (empty(x, y)) return;



	}
};







int main() {
	InitWindow(800, 800, "Wizard Game");
	SetTargetFPS(60);

	Tilemap foreground(4); // 4x4, 16 

	while (!WindowShouldClose()) {
		
		//draw
		BeginDrawing();
		ClearBackground(DARKBLUE);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}