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

//Hitboxes for blocks
class Tile {
	uint8_t size{32};
	Tile* block_root = this;

};

//matrix of tiles
class Tilemap {

};

//Block
class Block {

};





int main() {
	InitWindow(800, 800, "Wizard Game");

	//Tilemap map(1, 1);

	while (!WindowShouldClose()) {

	}


	CloseWindow();
	return 0;
}