#ifndef _SHAPE_H_INCLUDED
#define _SHAPE_H_INCLUDED

#include "chu_init.h"
#include <stdlib.h>

#include "vga_core.h"

class Shape {
public:
	Shape();
	Shape(BlocksCore *block_p);
	~Shape();

	int calculate_tetris();
	void create_shape_random();
	void rotate_current_shape();
	void move_current_shape_left_or_right(bool is_left);
	bool move_current_shape_down();
	void reset_blocks();
	bool on_red_line();

private:
	BlocksCore block;
	bool block_available[60];
	bool grid_available[15][7];
	int grid_block_number[15][7];
	int design;
	int rotation;
	int current_blocks[4];
	int current_blocks_grid_x[4];
	int current_blocks_grid_y[4];
};

#endif
