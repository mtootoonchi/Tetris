module title_src (
   input  logic clk,
   input  logic [10:0] x, y,     // treated as x-/y-axis
   output logic [11:0] bar_rgb 
   );

   // signal declaration
   logic [3:0] r, g, b;
   //logic [11:0] reg_d1_reg;
   
   always_comb begin
    r = 4'b0000;
    g = 4'b0000;
    b = 4'b0000;
    if ((203 <= x && x <= 206) || (432 <= x && x <= 435)) begin
        r = 4'b1111;
        g = 4'b1111;
        b = 4'b1111; 
    end
    if ((207 <= x && x <= 431) && (181 <= y && y <= 182)) begin
        r = 4'b1111;
        g = 4'b0000;
        b = 4'b0000; 
    end
   end
   
   // output with 2-stage delay line
//   always_ff @(posedge clk) begin
//      reg_d1_reg <= {r, g, b};
//   end
   assign bar_rgb = {r, g, b};
endmodule
