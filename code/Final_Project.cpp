/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "vga_core.h"
#include "shape.h"

void test_start(GpoCore *led_p) {
   int i;

   for (i = 0; i < 20; i++) {
      led_p->write(0xff00);
      sleep_ms(50);
      led_p->write(0x0000);
      sleep_ms(50);
   }
}

/**
 * check bar generator core
 * @param bar_p pointer to Gpv instance
 */
void title_check(GpvCore *title_p) {
   title_p->bypass(0);
   sleep_ms(3000);
}

/**
 * test frame buffer core
 * @param frame_p pointer to frame buffer instance
 */
void frame_check(FrameCore *frame_p) {
   int x, y, color;

   frame_p->bypass(0);
   for (int i = 0; i < 10; i++) {
      frame_p->clr_screen(0x008);  // dark green
      for (int j = 0; j < 20; j++) {
         x = rand() % 640;
         y = rand() % 480;
         color = rand() % 512;
         frame_p->plot_line(400, 200, x, y, color);
      }
      sleep_ms(300);
   }
   sleep_ms(3000);
}

void ps2_check(Ps2Core *ps2_p) {
   int id;
   int lbtn, rbtn, xmov, ymov;
   char ch;
   unsigned long last;

   uart.disp("\n\rPS2 device (1-keyboard / 2-mouse): ");
   id = ps2_p->init();
   uart.disp(id);
   uart.disp("\n\r");
   last = now_ms();
   do {
      if (id == 2) {  // mouse
         if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov)) {
            uart.disp("[");
            uart.disp(lbtn);
            uart.disp(", ");
            uart.disp(rbtn);
            uart.disp(", ");
            uart.disp(xmov);
            uart.disp(", ");
            uart.disp(ymov);
            uart.disp("] \r\n");
            last = now_ms();

         }   // end get_mouse_activitiy()
      } else {
         if (ps2_p->get_kb_ch(&ch)) {
            uart.disp(ch);
            uart.disp(" ");
            last = now_ms();
         } // end get_kb_ch()
      }  // end id==2
   } while (now_ms() - last < 5000);
   uart.disp("\n\rExit PS2 test \n\r");

}

/**
 * play primary notes with ddfs
 * @param ddfs_p pointer to ddfs core
 * @note: music tempo is defined as beats of quarter-note per minute.
 *        60 bpm is 1 sec per quarter note
 * @note "click" sound due to abrupt stop of a note
 *
 */
void ddfs_check(DdfsCore *ddfs_p, GpoCore *led_p) {
   int i, j;
   float env;

   //vol = (float)sw.read_pin()/(float)(1<<16),
   ddfs_p->set_env_source(0);  // select envelop source
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(500);
   ddfs_p->set_env(1.0);   // set volume
   ddfs_p->set_carrier_freq(262);
   sleep_ms(2000);
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(2000);
   // volume control (attenuation)
   ddfs_p->set_env(0.0);   // set volume
   env = 1.0;
   for (i = 0; i < 1000; i++) {
      ddfs_p->set_env(env);
      sleep_ms(10);
      env = env / 1.0109; //1.0109**1024=2**16
   }
   // frequency modulation 635-912 800 - 2000 siren sound
   ddfs_p->set_env(1.0);   // set volume
   ddfs_p->set_carrier_freq(635);
   for (i = 0; i < 5; i++) {               // 10 cycles
      for (j = 0; j < 30; j++) {           // sweep 30 steps
         ddfs_p->set_offset_freq(j * 10);  // 10 Hz increment
         sleep_ms(25);
      } // end j loop
   } // end i loop
   ddfs_p->set_offset_freq(0);
   ddfs_p->set_env(0.0);   // set volume
   sleep_ms(1000);
}

/**
 * play primary notes with ddfs
 * @param adsr_p pointer to adsr core
 * @param ddfs_p pointer to ddfs core
 * @note: music tempo is defined as beats of quarter-note per minute.
 *        60 bpm is 1 sec per quarter note
 *
 */
void adsr_check(AdsrCore *adsr_p, GpoCore *led_p, GpiCore *sw_p) {
   const int melody[] = { 0, 2, 4, 5, 7, 9, 11 };
   int i, oct;

   adsr_p->init();
   // no adsr envelop and  play one octave
   adsr_p->bypass();
   for (i = 0; i < 7; i++) {
      led_p->write(bit(i));
      adsr_p->play_note(melody[i], 3, 500);
      sleep_ms(500);
   }
   adsr_p->abort();
   sleep_ms(1000);
   // set and enable adsr envelop
   // play 4 octaves
   adsr_p->select_env(sw_p->read());
   for (oct = 3; oct < 6; oct++) {
      for (i = 0; i < 7; i++) {
         led_p->write(bit(i));
         adsr_p->play_note(melody[i], oct, 500);
         sleep_ms(500);
      }
   }
   led_p->write(0);
   // test duration
   sleep_ms(1000);
   for (i = 0; i < 4; i++) {
      adsr_p->play_note(0, 4, 500 * i);
      sleep_ms(500 * i + 1000);
   }
}

void press_start(FrameCore *frame_p, int color) {
	for (int i = 300; i < 350; i++) {
		frame_p->wr_pix(470, i, color);
	}

	for (int i = 471; i < 500; i++) {
		frame_p->wr_pix(i, 300, color);
	}

	for (int i = 300; i < 325; i++) {
		frame_p->wr_pix(499, i, color);
	}

	for (int i = 471; i < 500; i++) {
		frame_p->wr_pix(i, 324, color);
	}

	for (int i = 300; i < 350; i++) {
		frame_p->wr_pix(505, i, color);
	}

	for (int i = 506; i < 535; i++) {
		frame_p->wr_pix(i, 300, color);
	}

	for (int i = 300; i < 325; i++) {
		frame_p->wr_pix(534, i, color);
	}

	for (int i = 506; i < 535; i++) {
		frame_p->wr_pix(i, 324, color);
	}

	for (int i = 506; i < 532; i++) {
		frame_p->wr_pix(i, 324 + i - 506, color);
	}

	for (int i = 300; i < 350; i++) {
		frame_p->wr_pix(540, i, color);
	}

	for (int i = 540; i < 570; i++) {
		frame_p->wr_pix(i, 300, color);
	}

	for (int i = 540; i < 570; i++) {
		frame_p->wr_pix(i, 325, color);
	}

	for (int i = 540; i < 570; i++) {
		frame_p->wr_pix(i, 349, color);
	}

	for (int i = 300; i < 326; i++) {
		frame_p->wr_pix(576, i, color);
	}

	for (int i = 577; i < 601; i++) {
		frame_p->wr_pix(i, 300, color);
	}

	for (int i = 577; i < 600; i++) {
		frame_p->wr_pix(i, 325, color);
	}

	for (int i = 325; i < 350; i++) {
		frame_p->wr_pix(600, i, color);
	}

	for (int i = 577; i < 600; i++) {
		frame_p->wr_pix(i, 349, color);
	}

	for (int i = 300; i < 326; i++) {
		frame_p->wr_pix(606, i, color);
	}

	for (int i = 606; i < 630; i++) {
		frame_p->wr_pix(i, 300, color);
	}

	for (int i = 606; i < 630; i++) {
		frame_p->wr_pix(i, 325, color);
	}

	for (int i = 325; i < 350; i++) {
		frame_p->wr_pix(629, i, color);
	}

	for (int i = 606; i < 630; i++) {
		frame_p->wr_pix(i, 349, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(470, i, color);
	}

	for (int i = 471; i < 500; i++) {
		frame_p->wr_pix(i, 356, color);
	}

	for (int i = 471; i < 500; i++) {
		frame_p->wr_pix(i, 381, color);
	}

	for (int i = 471; i < 500; i++) {
		frame_p->wr_pix(i, 405, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(506, i, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(506 + i - 356, i, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(556, i, color);
	}

	for (int i = 562; i < 590; i++) {
		frame_p->wr_pix(i, 356, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(576, i, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(596, i, color);
	}

	for (int i = 596; i < 615; i++) {
		frame_p->wr_pix(i, 356, color);
	}

	for (int i = 596; i < 615; i++) {
		frame_p->wr_pix(i, 381, color);
	}

	for (int i = 596; i < 615; i++) {
		frame_p->wr_pix(i, 405, color);
	}

	for (int i = 356; i < 406; i++) {
		frame_p->wr_pix(620, i, color);
	}

	for (int i = 620; i < 635; i++) {
		frame_p->wr_pix(i, 356, color);
	}

	for (int i = 356; i < 381; i++) {
		frame_p->wr_pix(634, i, color);
	}

	for (int i = 620; i < 635; i++) {
		frame_p->wr_pix(i, 381, color);
	}

	for (int i = 620; i < 640; i++) {
		frame_p->wr_pix(i, 381 + i - 620, color);
	}

}

bool rotate_reset = false;
bool right_reset = false;
bool left_reset = false;
bool wait_reset = false;

void allow_rotate_but_wait_us(Ps2Core *ps2_p, AdsrCore *adsr_p, Shape* shape_p, int *wait_us, int *points) {
	int current_time_us = (int)now_us();
	int current_time_us_music = (int)now_us();
	while (true) {
		if (((int)now_us() - current_time_us) >= *wait_us) {
			return;
		}

		if (((int)now_us() - current_time_us_music) >= 100) {
			current_time_us_music = (int)now_us();
			adsr_p->play_note((rand() % 12), (3), 1);
		}

		int byte0 = ps2_p->rx_byte();

		if (byte0 == 0xF0) {
			int byte1 = ps2_p->rx_byte();
			while (true) {
				if (byte1 != -1)
					break;
				byte1 = ps2_p->rx_byte();
				}
			if (byte1 == 0x1B)
			{
				wait_reset = false;
				*wait_us = 1000000;
			}
			else if (byte1 == 0x1C)
				left_reset = false;
			else if (byte1 == 0x23)
				right_reset = false;
			else if (byte1 == 0x2d)
				rotate_reset = false;
			}
		else if (byte0 == 0x1B && !left_reset) {
			wait_reset = true;
			*wait_us = 50000;
		}
		else if (byte0 == 0x1C && !left_reset) {
			left_reset = true;
			shape_p->move_current_shape_left_or_right(true);
		}
		else if (byte0 == 0x23 && !right_reset) {
			right_reset = true;
			shape_p->move_current_shape_left_or_right(false);
		}
		else if (byte0 == 0x2d && !rotate_reset) {
			rotate_reset = true;
			shape_p->rotate_current_shape();
		}
	}
}

void allow_rotate_but_wait_ms(Ps2Core *ps2_p, AdsrCore *adsr_p, Shape* shape_p, int *wait_ms, int points) {
	int current_time_ms = (int)now_ms();
	int current_time_ms_music = (int)now_ms();
	points += 2;
	while (true) {
		if (((int)now_ms() - current_time_ms) >= *wait_ms) {
			if (points >= 6)
				points = 6;
			adsr_p->play_note((rand() % 12), points, 1);
			return;
		}

		if (((int)now_ms() - current_time_ms_music) >= 200) {
			current_time_ms_music = (int)now_ms();
			if (points >= 6)
				points = 6;
			adsr_p->play_note((rand() % 12), points, 1);
		}

		int byte0 = ps2_p->rx_byte();

		if (byte0 == 0xF0) {
			int byte1 = ps2_p->rx_byte();
			while (true) {
				if (byte1 != -1)
					break;
				byte1 = ps2_p->rx_byte();
			}
			if (byte1 == 0x1B)
			{
				wait_reset = false;
				*wait_ms = 1000;
			}
			else if (byte1 == 0x1C)
				left_reset = false;
			else if (byte1 == 0x23)
				right_reset = false;
			else if (byte1 == 0x2d)
				rotate_reset = false;
		}
		else if (byte0 == 0x1B && !left_reset) {
			wait_reset = true;
			*wait_ms = 50;
		}
		else if (byte0 == 0x1C && !left_reset) {
			left_reset = true;
			shape_p->move_current_shape_left_or_right(true);
		}
		else if (byte0 == 0x23 && !right_reset) {
			right_reset = true;
			shape_p->move_current_shape_left_or_right(false);
		}
		else if (byte0 == 0x2d && !rotate_reset) {
			rotate_reset = true;
			shape_p->rotate_current_shape();
		}
	}
}

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);

FrameCore frame(FRAME_BASE);
GpvCore title(get_sprite_addr(BRIDGE_BASE, V5_TITLE));
BlocksCore block(get_sprite_addr(BRIDGE_BASE, V4_BLOCK), get_sprite_addr(BRIDGE_BASE, V3_BLOCK), get_sprite_addr(BRIDGE_BASE, V2_BLOCK), get_sprite_addr(BRIDGE_BASE, V1_BLOCK));
Shape shape(&block);

int main() {
	frame.bypass(0);
	title.bypass(1);
	block.bypass(0);

	for (int i = 203; i < 207; i++) {
		for (int j = 0; j < 480; j++) {
			frame.wr_pix(i, j, 0x1FF);
		}
	}

	for (int i = 432; i < 436; i++) {
		for (int j = 0; j < 480; j++) {
			frame.wr_pix(i, j, 0x1FF);
		}
	}

	for (int i = 207; i < 432; i++) {
		for (int j = 181; j < 183; j++) {
			frame.wr_pix(i, j, 0x1C0);
		}
	}

	press_start(&frame, 0x1FF);

	unsigned int us_now = (unsigned int)now_us();

	while (ps2.rx_byte() != 0x5a) {
		adsr.play_note((rand() % 12), 3, 1);
		sleep_ms(300);
	}

	srand((unsigned int)now_us() - us_now);

	press_start(&frame, 0x0);

	int wait_time = 1000;
	int points = 0;

	while (true) {
		shape.create_shape_random();
		allow_rotate_but_wait_ms(&ps2, &adsr, &shape, &wait_time, points);
		while (shape.move_current_shape_down()) {
			allow_rotate_but_wait_ms(&ps2, &adsr, &shape, &wait_time, points);
		}
		if (shape.on_red_line()) {
			wait_time = 1000;

			shape.reset_blocks();

			press_start(&frame, 0x1FF);

			us_now = (unsigned int)now_us();

			while (ps2.rx_byte() != 0x5a) {
				adsr.play_note((rand() % 12), (3), 1);
				sleep_ms(300);
			}

			srand((unsigned int)now_us() - us_now);

			press_start(&frame, 0x0);
		}
		else {
			points += shape.calculate_tetris();
		}
	}
} //main
