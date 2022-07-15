`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/03/2022 07:54:15 PM
// Design Name: 
// Module Name: chu_vga_block_core
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module chu_vga_block_core
   #(parameter CD = 12,   // color depth
               ADDR_WIDTH = 10,
               KEY_COLOR = 0
   )
   (
    input  logic clk, reset,
    // frame counter
    input logic [10:0] x, y,
    // video slot interface
    input  logic cs,      
    input  logic write,  
    input  logic [13:0] addr,    
    input  logic [31:0] wr_data,
    // stream interface
    input  logic [11:0] si_rgb,
    output logic [11:0] so_rgb
   );

   // delaration
   logic wr_en, wr_ram, wr_reg, wr_bypass;
   logic [14:0] wr_xy_bypass;
   logic [14:0] wr_color;
   logic [CD-1:0] sprite_rgb, chrom_rgb;
   logic [10:0] xs_reg [0:14];
   logic [10:0] ys_reg [0:14];
   logic [14:0] block_bypass_reg;
   logic [2:0] color_reg [0:14];
   logic bypass_reg;

   // body
   // instantiate sprite generator
   block_src #(.CD(12), .KEY_COLOR(0)) ghost_src_unit (
       .clk(clk), .x(x), .y(y), .xs(xs_reg), .ys(ys_reg), .block_bypass(block_bypass_reg),
       .color(color_reg), .we(wr_ram), .addr_w(addr[ADDR_WIDTH-1:0]),
       .pixel_in(wr_data[1:0]), .sprite_rgb(sprite_rgb));
       
   // register
   always_ff @(posedge clk, posedge reset) begin
    if (reset) begin
        bypass_reg <= 0;
        block_bypass_reg <= 14'b1;
        for (int i = 0; i < 15; i = i + 1) begin
            xs_reg[i] <= 11'b0;
            ys_reg[i] <= 11'b0;
            color_reg[i] <= 3'b0;
        end
    end
    else begin
        if (wr_bypass)
            bypass_reg <= wr_data[0];
        for (int i = 0; i < 15; i = i + 1) begin
            if (wr_xy_bypass[i]) begin
                xs_reg[i] <= wr_data[10:0];
                ys_reg[i] <= wr_data[21:11];
                block_bypass_reg[i] <= wr_data[22];
            end
            if (wr_color[i]) begin
                color_reg[i] = wr_data[2:0];
            end
        end
    end
   end
   
   always_comb begin
    for (int i = 0; i < 15; i = i + 1) begin
        if (wr_reg && (~addr[4]) && (addr[3:0] == i)) begin
            wr_xy_bypass[i] = 1'b1;
        end
        else begin
            wr_xy_bypass[i] = 1'b0;
        end
        if (wr_reg && (addr[4]) && (addr[3:0] == i)) begin
            wr_color[i] = 1'b1;
        end
        else begin
            wr_color[i] = 1'b0;
        end
    end
   end
   
   // decoding 
   assign wr_en = write & cs;
   assign wr_ram = ~addr[13] && ~addr[12] && wr_en;
   assign wr_reg = addr[13] && ~addr[12] && wr_en;
   assign wr_bypass = addr[12] && wr_en;
   // chrome-key blending and multiplexing
   assign chrom_rgb = (sprite_rgb != KEY_COLOR) ? sprite_rgb : si_rgb;
   assign so_rgb = (bypass_reg) ? si_rgb : chrom_rgb;
endmodule   

