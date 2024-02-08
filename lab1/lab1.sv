// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2023
//
// By: <your name here>
// Uni: <your uni here>

module lab1( input logic        CLOCK_50,  // 50 MHz Clock input
	     
	     input logic [3:0] 	KEY, // Pushbuttons; KEY[0] is rightmost

	     input logic [9:0] 	SW, // Switches; SW[0] is rightmost

	     // 7-segment LED displays; HEX0 is rightmost
	     output logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5,

	     output logic [9:0] LEDR // LEDs above the switches; LED[0] on right
	     );

   logic 			clk, go, done;   
   logic [31:0] 		start;
   logic [15:0] 		count;

   logic [11:0] 		n;
   logic [3:0]                  padded_n;
   logic [14:0]                 counter; 
   logic [15:0]                 range_output;
   logic 			toggle;
   logic [3:0]                  dump_output;  
   assign clk = CLOCK_50;
  
   range #(256, 8) // RAM_WORDS = 256, RAM_ADDR_BITS = 8)
         r ( .* ); // Connect everything with matching names

   // Replace this comment and the code below it with your own code;
   // The code below is merely to suppress Verilator lint warnings

   //assign go = 0;
   //assign start = 32'b0;
   //assign n = {2'b0, SW[9:0]};
   assign padded_n = {2'b00, SW[9:8]};
   hex7seg h1(.a(SW[3:0]), .y(HEX3[6:0]));       
   hex7seg h2(.a(SW[7:4]), .y(HEX4[6:0]));       
   hex7seg h3(.a(padded_n), .y(HEX5[6:0]));
   always_ff @(posedge clk) begin
     state <= next_state;
   end

   typedef enum logic [2:0] {NOTHING, WAIT, DONE} state_t;
   state_t state, next_state;
   assign start = {22'b0, SW[9:0]};
   always_comb begin
     go = 0;
     range_output = 0;
     next_state = state;
     case (state)
       NOTHING: begin
	range_output = 1;
        if (KEY[3])
        begin
          go = 1;
	  next_state = WAIT;
	end
       end
       WAIT: begin
        if (done) 
	begin
	  range_output = count;
	  next_state = DONE;
	end
	next_state = WAIT;
       end
       DONE: begin
	next_state = DONE;
	range_output=count;
       end
       default: next_state = NOTHING;
     endcase
   end
        
   hex7seg h4(.a(range_output[3:0]), .y(HEX0[6:0]));
   hex7seg h5(.a(range_output[7:4]), .y(HEX1[6:0]));
   hex7seg h6(.a(range_output[11:8]), .y(HEX2[6:0]));
 endmodule
