#include "shape.h"

Shape::Shape() {

}

Shape::Shape(BlocksCore *block_p) {
	block = *block_p;
	for (int i = 0; i < 60; i++) {
		block_available[i] = true;
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			grid_available[i][j] = true;
		}
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			grid_block_number[i][j] = -1;
		}
	}
}

Shape::~Shape() {

}

int Shape::calculate_tetris() {
	int tetrises[4] = {-1, -1, -1, -1};
	int tetrises_index = 0;
	bool found = false;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			if (!grid_available[i][j]) {
				if (j == 6) {
					tetrises[tetrises_index] = i;
					++tetrises_index;
					found = true;
				}
			}
			else {
				j = 7;
				if (found) {
					i = 15;
				}
			}
		}
	}
	if (tetrises_index == 0)
		return 0;
	for (int i = 0; i < tetrises_index; i++) {
		for (int j = 0; j < 7; j++) {
			block.wr_blocks_location_en(grid_block_number[tetrises[i]][j], 0, 0, 1);
			grid_available[tetrises[i]][j] = true;
			block_available[grid_block_number[tetrises[i]][j]] = true;
			grid_block_number[tetrises[i]][j] = -1;
			sleep_ms(50);
		}
	}
	for (int i = tetrises[0] - 1; i >= 0; i--) {
		for (int j = 0; j < 7; j++) {
			block.wr_blocks_location(grid_block_number[i][j], 207 + j * 32, (i + tetrises_index) * 32);
			if (!grid_available[i][j]) {
				grid_available[i][j] = true;
				grid_available[i + tetrises_index][j] = false;
			}
			grid_block_number[i + tetrises_index][j] = grid_block_number[i][j];
			grid_block_number[i][j] = -1;
		}
	}
	return tetrises_index + calculate_tetris();
}

void Shape::create_shape_random() {
	design = rand() % 7;
	rotation = 0;
	int blocks_picked = 0;
	for (int i = 0; i < 60; i++) {
		if (block_available[i]) {
			block_available[i] = false;
			current_blocks[blocks_picked] = i;
			++blocks_picked;
			if (blocks_picked == 4) {
				i = 60;
			}
		}
	}
	uint32_t color = rand() % 6 + 1;
	switch (design) {
		case 0: {
			int x = rand() % 7;
			for (int i = 0; i < 4; i++) {
				current_blocks_grid_x[i] = x;
			}
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 0;
			current_blocks_grid_y[2] = 2;
			current_blocks_grid_y[3] = 3;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 1: {
			int x = (rand() % 5) + 1;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x + 1;
			current_blocks_grid_x[2] = x - 1;
			current_blocks_grid_x[3] = x - 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 1;
			current_blocks_grid_y[2] = 1;
			current_blocks_grid_y[3] = 0;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 2: {
			int x = (rand() % 5) + 1;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x - 1;
			current_blocks_grid_x[2] = x + 1;
			current_blocks_grid_x[3] = x + 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 1;
			current_blocks_grid_y[2] = 1;
			current_blocks_grid_y[3] = 0;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 3: {
			int x = rand() % 6;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x;
			current_blocks_grid_x[2] = x + 1;
			current_blocks_grid_x[3] = x + 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 0;
			current_blocks_grid_y[2] = 1;
			current_blocks_grid_y[3] = 0;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 4: {
			int x = (rand() % 5) + 1;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x - 1;
			current_blocks_grid_x[2] = x;
			current_blocks_grid_x[3] = x + 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 1;
			current_blocks_grid_y[2] = 0;
			current_blocks_grid_y[3] = 0;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 5: {
			int x = (rand() % 5) + 1;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x;
			current_blocks_grid_x[2] = x - 1;
			current_blocks_grid_x[3] = x + 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 0;
			current_blocks_grid_y[2] = 1;
			current_blocks_grid_y[3] = 1;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		case 6: {
			int x = (rand() % 5) + 1;
			current_blocks_grid_x[0] = x;
			current_blocks_grid_x[1] = x;
			current_blocks_grid_x[2] = x - 1;
			current_blocks_grid_x[3] = x + 1;
			current_blocks_grid_y[0] = 1;
			current_blocks_grid_y[1] = 0;
			current_blocks_grid_y[2] = 0;
			current_blocks_grid_y[3] = 1;
			for (int i = 0; i < 4; i++) {
				grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
				grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
				block.wr_blocks(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0, color);
			}
			break;
		}
		default: {
			int temp = 0;
		}
	}
}

void Shape::rotate_current_shape() {
	switch (design) {
		case 0: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 2;
					for (int i = 0; i < 3; i++) {
						new_rotated_locations_y[i] = current_blocks_grid_y[0];
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_y[0] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 2;
					for (int i = 0; i < 3; i++) {
						new_rotated_locations_x[i] = current_blocks_grid_x[0];
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
				default: {
					int temp = 0;
				}
			}
			break;
		}
		case 1: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 2;
					break;
				}
				case 2: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 3;
					break;
				}
				case 3: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
				default: {
					int temp = 0;
				}
			}
			break;
		}
		case 2: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 2;
					break;
				}
				case 2: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 3;
					break;
				}
				case 3: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || !grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
			}
			break;
		}
		case 3: {
			int temp = 0;
			break;
		}
		case 4: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
			}
			break;
		}
		case 5: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0];
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] + 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0];
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 2;
					break;
				}
				case 2: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0];
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 3;
					break;
				}
				case 3: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0];
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
			}
			break;
		}
		case 6: {
			switch (rotation) {
				case 0: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0];
					new_rotated_locations_x[1] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[2] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0] + 1;
					new_rotated_locations_y[1] = current_blocks_grid_y[0];
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 1;
					break;
				}
				case 1: {
					int new_rotated_locations_x[3];
					int new_rotated_locations_y[3];
					new_rotated_locations_x[0] = current_blocks_grid_x[0] + 1;
					new_rotated_locations_x[1] = current_blocks_grid_x[0];
					new_rotated_locations_x[2] = current_blocks_grid_x[0] - 1;
					new_rotated_locations_y[0] = current_blocks_grid_y[0];
					new_rotated_locations_y[1] = current_blocks_grid_y[0] - 1;
					new_rotated_locations_y[2] = current_blocks_grid_y[0] - 1;
					for (int i = 0; i < 3; i++) {
						if ((new_rotated_locations_x[i] < 0) || (new_rotated_locations_x[i] >= 7) || (new_rotated_locations_y[i] < 0) || (new_rotated_locations_y[i] >= 15) || (!grid_available[new_rotated_locations_y[i]][new_rotated_locations_x[i]] && ((current_blocks[1] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[2] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]]) && (current_blocks[3] != grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]])))) {
							return;
						}
					}
					for (int i = 0; i < 3; i++) {
						grid_block_number[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = -1;
						grid_block_number[new_rotated_locations_y[i]][new_rotated_locations_x[i]] = current_blocks[i + 1];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = true;
						current_blocks_grid_x[i + 1] = new_rotated_locations_x[i];
						current_blocks_grid_y[i + 1] = new_rotated_locations_y[i];
						grid_available[current_blocks_grid_y[i + 1]][current_blocks_grid_x[i + 1]] = false;
						block.wr_blocks_location(current_blocks[i + 1], 207 + current_blocks_grid_x[i + 1] * 32, current_blocks_grid_y[i + 1] * 32);
					}
					rotation = 0;
					break;
				}
			}
			break;
		}
		default: {
			int temp = 0;
		}
	}
}

void Shape::move_current_shape_left_or_right(bool is_left) {
	if (is_left) {
		for (int i = 0; i < 4; i++) {
			if ((current_blocks_grid_x[i] - 1) < 0) {
				return;
			}
			if (!grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i] - 1]) {
				bool is_itself = false;
				for (int j = 0; j < 4; j++) {
					if (grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i] - 1] == current_blocks[j]) {
						is_itself = true;
					}
				}
				if (!is_itself) {
					return;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = true;
			grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = -1;
			current_blocks_grid_x[i] = current_blocks_grid_x[i] - 1;
		}
		for (int i = 0; i < 4; i++) {
			grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
			grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
			block.wr_blocks_location_en(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			if ((current_blocks_grid_x[i] + 1) >= 7) {
				return;
			}
			if (!grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i] + 1]) {
				bool is_itself = false;
				for (int j = 0; j < 4; j++) {
					if (grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i] + 1] == current_blocks[j]) {
						is_itself = true;
					}
				}
				if (!is_itself) {
					return;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = true;
			grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = -1;
			current_blocks_grid_x[i] = current_blocks_grid_x[i] + 1;
		}
		for (int i = 0; i < 4; i++) {
			grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
			grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
			block.wr_blocks_location_en(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32, 0);
		}
	}
}

bool Shape::move_current_shape_down() {
	for (int i = 0; i < 4; i++) {
		if ((current_blocks_grid_y[i] + 1) >= 15) {
			return false;
		}
		if (!grid_available[current_blocks_grid_y[i] + 1][current_blocks_grid_x[i]]) {
			bool is_itself = false;
			for (int j = 0; j < 4; j++) {
				if (grid_block_number[current_blocks_grid_y[i] + 1][current_blocks_grid_x[i]] == current_blocks[j]) {
					is_itself = true;
				}
			}
			if (!is_itself) {
				return false;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = true;
		grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = -1;
		current_blocks_grid_y[i] = current_blocks_grid_y[i] + 1;
	}
	for (int i = 0; i < 4; i++) {
		grid_available[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = false;
		grid_block_number[current_blocks_grid_y[i]][current_blocks_grid_x[i]] = current_blocks[i];
		block.wr_blocks_location(current_blocks[i], 207 + current_blocks_grid_x[i] * 32, current_blocks_grid_y[i] * 32);
	}
}

void Shape::reset_blocks() {
	for (int i = 0; i < 60; i++) {
		block.wr_blocks_location_en(i, 0, 0, 1);
		block_available[i] = true;
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			grid_available[i][j] = true;
		}
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			grid_block_number[i][j] = -1;
		}
	}
}

bool Shape::on_red_line() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (!grid_available[i][j]) {
				return true;
			}
		}
	}
	return false;
}
