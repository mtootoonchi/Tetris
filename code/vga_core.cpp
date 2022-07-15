/*****************************************************************//**
 * @file timer_core.cpp
 *
 * @brief implementation of various video core classes
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/

#include "vga_core.h"

/**********************************************************************
 * General purpose video core methods
 *********************************************************************/
GpvCore::GpvCore(uint32_t core_base_addr) {
   base_addr = core_base_addr;
}
GpvCore::~GpvCore() {
}

void GpvCore::wr_mem(int addr, uint32_t data) {
   io_write(base_addr, addr, data);
}

void GpvCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

/**********************************************************************
 * Block core methods
 *********************************************************************/
BlocksCore::BlocksCore() {

}

BlocksCore::BlocksCore(uint32_t core_base_addr3, uint32_t core_base_addr2, uint32_t core_base_addr1, uint32_t core_base_addr0) {
	base_addr0 = core_base_addr0;
	base_addr1 = core_base_addr1;
	base_addr2 = core_base_addr2;
	base_addr3 = core_base_addr3;
	for (int i = 0; i < 60; i++) {
		wr_blocks_location_en(i, 0, 0, 1);
	}
}
BlocksCore::~BlocksCore() {

}

void BlocksCore::wr_mem(int addr, uint32_t color) {
   io_write(base_addr0, addr, color);
   io_write(base_addr1, addr, color);
   io_write(base_addr2, addr, color);
   io_write(base_addr3, addr, color);
}

void BlocksCore::wr_blocks(uint32_t block, uint32_t x, uint32_t y, uint32_t block_bypass, uint32_t rgb) {
	if ((block >= 0) && (block < 15)) {
		io_write(base_addr0, WR_REG + XY_BYPASS_REG + (block & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
		io_write(base_addr0, WR_REG + COLOR_REG + (block & 0x0F), rgb & 0x7);
	}
	else if ((block >= 15) && (block < 30)) {
		io_write(base_addr1, WR_REG + XY_BYPASS_REG + ((block + 1) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
		io_write(base_addr1, WR_REG + COLOR_REG + ((block + 1) & 0x0F), rgb & 0x7);
	}
	else if ((block >= 30) && (block < 45)) {
		io_write(base_addr2, WR_REG + XY_BYPASS_REG + ((block + 2) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
		io_write(base_addr2, WR_REG + COLOR_REG + ((block + 2) & 0x0F), rgb & 0x7);
	}
	else if ((block >= 45) && (block < 60)) {
		io_write(base_addr3, WR_REG + XY_BYPASS_REG + ((block + 3) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
		io_write(base_addr3, WR_REG + COLOR_REG + ((block + 3) & 0x0F), rgb & 0x7);
	}
}

void BlocksCore::wr_blocks_location_en(uint32_t block, uint32_t x, uint32_t y, uint32_t block_bypass) {
	if ((block >= 0) && (block < 15)) {
		io_write(base_addr0, WR_REG + XY_BYPASS_REG + (block & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 15) && (block < 30)) {
		io_write(base_addr1, WR_REG + XY_BYPASS_REG + ((block + 1) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 30) && (block < 45)) {
		io_write(base_addr2, WR_REG + XY_BYPASS_REG + ((block + 2) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 45) && (block < 60)) {
		io_write(base_addr3, WR_REG + XY_BYPASS_REG + ((block + 3) & 0x0F), ((block_bypass & 0x1) << 22) + ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
}

void BlocksCore::wr_blocks_location(uint32_t block, uint32_t x, uint32_t y) {
	if ((block >= 0) && (block < 15)) {
		io_write(base_addr0, WR_REG + XY_BYPASS_REG + (block & 0x0F), ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 15) && (block < 30)) {
		io_write(base_addr1, WR_REG + XY_BYPASS_REG + ((block + 1) & 0x0F), ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 30) && (block < 45)) {
		io_write(base_addr2, WR_REG + XY_BYPASS_REG + ((block + 2) & 0x0F), ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
	else if ((block >= 45) && (block < 60)) {
		io_write(base_addr3, WR_REG + XY_BYPASS_REG + ((block + 3) & 0x0F), ((y & 0x7FF) << 11) + (x & 0x7FF));
	}
}

void BlocksCore::wr_blocks_color(uint32_t block, uint32_t rgb) {
	if ((block >= 0) && (block < 15)) {
		io_write(base_addr0, WR_REG + COLOR_REG + (block & 0x0F), rgb & 0x7);
	}
	else if ((block >= 15) && (block < 30)) {
		io_write(base_addr1, WR_REG + COLOR_REG + ((block + 1) & 0x0F), rgb & 0x7);
	}
	else if ((block >= 30) && (block < 45)) {
		io_write(base_addr2, WR_REG + COLOR_REG + ((block + 2) & 0x0F), rgb & 0x7);
	}
	else if ((block >= 45) && (block < 60)) {
		io_write(base_addr3, WR_REG + COLOR_REG + ((block + 3) & 0x0F), rgb & 0x7);
	}
}

void BlocksCore::bypass(int by) {
   io_write(base_addr0, BYPASS_REG, (uint32_t ) by);
   io_write(base_addr1, BYPASS_REG, (uint32_t ) by);
   io_write(base_addr2, BYPASS_REG, (uint32_t ) by);
   io_write(base_addr3, BYPASS_REG, (uint32_t ) by);
}


/**********************************************************************
 * FrameCore core methods
 *********************************************************************/
FrameCore::FrameCore(uint32_t frame_base_addr) {
   base_addr = frame_base_addr;
}
FrameCore::~FrameCore() {
}
// not used

void FrameCore::wr_pix(int x, int y, int color) {
   uint32_t pix_offset;

   pix_offset = HMAX * y + x;
   io_write(base_addr, pix_offset, color);
   return;
}

void FrameCore::clr_screen(int color) {
   int x, y;

   for (x = 0; x < HMAX; x++)
      for (y = 0; y < VMAX; y++) {
         wr_pix(x, y, color);
      }
   return;
}

void FrameCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

// from AdaFruit
void FrameCore::plot_line(int x0, int y0, int x1, int y1, int color) {
   int dx, dy;
   int err, ystep, steep;

   if (x0 > x1) {
      swap(x0, x1);
      swap(y0, y1);
   }
   // slope is high
   steep = (abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
   if (steep) {
      swap(x0, y0);
      swap(x1, y1);
   }
   dx = x1 - x0;
   dy = abs(y1 - y0);
   err = dx / 2;
   if (y0 < y1) {
      ystep = 1;
   } else {
      ystep = -1;
   }
   for (; x0 <= x1; x0++) {
      if (steep) {
         wr_pix(y0, x0, color);
      } else {
         wr_pix(x0, y0, color);
      }
      err = err - dy;
      if (err < 0) {
         y0 = y0 + ystep;
         err = err + dx;
      }
   }
}

void FrameCore::swap(int &a, int &b) {
   int tmp;

   tmp = a;
   a = b;
   b = tmp;
}

