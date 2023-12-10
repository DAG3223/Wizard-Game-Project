#include <iostream>
#include <vector>
#include <memory>

#include "raylib.h"
#include "raymath.h"

class Tile {

};

class Tilemap {
	typedef std::unique_ptr<Tile> uptr_tile;
	typedef std::vector<std::vector<uptr_tile>> matrix;

	matrix map;
public:
	/*Tilemap() {
		map.resize(0);
	}*/
	Tilemap(size_t width, size_t height) {
		map.resize(height);
		for (auto& row : map) {
			row.resize(width, nullptr);
		}
	}

	//get reference to uptr at provided indices
	uptr_tile& at(size_t idx_x, size_t idx_y) {
		return map.at(idx_y).at(idx_x);
	}



	//resets uptr to nullptr at provided indices
	void destroy(size_t idx_x, size_t idx_y) {
		at(idx_x, idx_y).reset(nullptr);
	}
};

int main() {
	InitWindow(800, 800, "Wizard Game");


	while (!WindowShouldClose()) {

	}


	CloseWindow();
	return 0;
}