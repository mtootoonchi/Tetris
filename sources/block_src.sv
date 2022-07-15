module block_src 
   #(
    parameter CD = 12,      // color depth
              ADDR = 10,    // number of address bits
              KEY_COLOR =0  // chroma key
   )
   (
    input  logic clk,
    input  logic [10:0] x, y,   // x-and  y-coordinate    
    input  logic [10:0] xs [0:14], 
    input  logic [10:0] ys [0:14], // origin of sprite 
    input logic [14:0] block_bypass,
    input  logic [2:0] color [0:14],    // sprite control 
    // sprite ram write 
    input  logic we ,
    input  logic [ADDR-1:0] addr_w,
    input  logic [1:0] pixel_in,
    // pixel output
    output logic [CD-1:0] sprite_rgb
   );
   
   // localparam declaration
   localparam H_SIZE = 32; // horizontal size of sprite
   localparam V_SIZE = 32; // vertical size of sprite
   
   // current block
   logic [2:0] block_color;
   
   // relative x/y position current
    logic signed [11:0] xr_current [0:14];
    logic signed [11:0] yr_current [0:14];
   
   // signal delaration
   logic signed [11:0] xr, yr;  // relative x/y position
   logic in_region;

   logic [ADDR-1:0] addr_r;
   logic [1:0] plt_code;        
    
   logic [CD-1:0]  out_rgb;
   logic [CD-1:0] full_rgb;
   logic [CD-1:0] out_rgb_d1_reg;

   //******************************************************************
   // sprite RAM
   //******************************************************************
   // instantiate sprite RAM
   block_ram_lut #(.ADDR_WIDTH(ADDR), .DATA_WIDTH(2)) ram_unit (
      .clk(clk), .we(we), .addr_w(addr_w), .din(pixel_in),
      .addr_r(addr_r), .dout(plt_code));
   assign addr_r = {yr[4:0], xr[4:0]};
    
   // palette table
   always_comb
      case (plt_code)
         2'b00:   full_rgb = 12'h000;   // chrome key
         2'b01:   full_rgb = 12'hFFF;   // white
         2'b10:   full_rgb = {1'b0, block_color[2], block_color[2], block_color[2], 1'b0, block_color[1], block_color[1], block_color[1], block_color[0], 1'b0, block_color[0], block_color[0]}; // block body color dim
         2'b11:   full_rgb = {block_color[2], block_color[2], block_color[2], block_color[2], block_color[1], block_color[1], block_color[1], block_color[1], block_color[0], block_color[0], block_color[0], block_color[0]}; // block body color
         default: full_rgb = 12'h000;   // chrome key
      endcase   
   //******************************************************************
   // in-region circuit
   //******************************************************************
   // relative coordinate calculation
   
   always_comb begin
    for (int i = 0; i < 15; i = i + 1) begin
        xr_current[i] = -1;
        yr_current[i] = -1;
    end
    for (int i = 0; i < 15; i = i + 1) begin
        xr_current[i] = $signed({1'b0, x}) - $signed({1'b0, xs[i]});
        yr_current[i] = $signed({1'b0, y}) - $signed({1'b0, ys[i]}); 
    end
   end
   
   always_comb begin
    xr = 12'd25;
    yr = 12'd25;
    in_region = 1'b0;
    block_color = 3'b100;
    for (int i = 0; i < 15; i = i + 1) begin
        // in-region comparison
        if ((0 <= xr_current[i]) && (xr_current[i] < H_SIZE) && (0 <= yr_current[i]) && (yr_current[i] < V_SIZE) && (~block_bypass[i])) begin
            xr = xr_current[i];
            yr = yr_current[i];
            in_region = 1'b1;
            block_color = color[i];
        end
    end
   end
   
   // in-region multiplexing 
   assign out_rgb = in_region ? full_rgb : KEY_COLOR;
   
   //******************************************************************
   // delay line (two clock) 
   //******************************************************************
   // output with 2-stage delay line
   always_ff @(posedge clk) begin
      out_rgb_d1_reg <= out_rgb;
   end
   assign sprite_rgb = out_rgb_d1_reg;
endmodule
  