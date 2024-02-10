// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2023
//
// By: Zachary Coeur, Sanjay Rajasekharan
// Uni: zjc2106, sr3765

module lab1( input logic        CLOCK_50,  // 50 MHz Clock input
	     
	     input logic [3:0] 	KEY, // Pushbuttons; KEY[0] is rightmost

	     input logic [9:0] 	SW, // Switches; SW[0] is rightmost

	     // 7-segment LED displays; HEX0 is rightmost
	     output logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5,

	     output logic LEDR[0] // LEDs above the switches; LED[0] on right
	     );
     logic 			clk, go, done;   
     logic [31:0] 		start = 1;
     logic [15:0] 		count;
 
     logic [23:0]               go_counter = 1; // counters to prevent button debouncing
     logic [23:0]               up_counter = 1;
     logic [23:0]               down_counter = 1;
     logic [23:0]               reset_counter = 1;

     logic [1:0]                waiting = 0; // range module produces count 2 cycles after 
                                             // start changes, waiting counts the cycles in between

     logic                      done_trig = 0; // tracks if done signal has been seen

     logic [11:0]               count_output = 0; // values shown on 7-seg displays
     logic [11:0]               n_output = 0;

     logic [11:0]	 	n_0 = 0; // number at start of range 

     logic [24:0] 		led_counter = 1; // counter to ensure done indicator flashes ~1s  

     assign clk = CLOCK_50;
   
     range #(256, 8) // RAM_WORDS = 256, RAM_ADDR_BITS = 8)
          r ( .* ); // Connect everything with matching names

     hex7seg h1(.a(n_output[3:0]), .y(HEX3[6:0]));
     hex7seg h2(.a(n_output[7:4]), .y(HEX4[6:0]));
     hex7seg h3(.a(n_output[11:8]), .y(HEX5[6:0]));

     hex7seg h4(.a(count_output[3:0]), .y(HEX0[6:0]));
     hex7seg h5(.a(count_output[7:4]), .y(HEX1[6:0]));
     hex7seg h6(.a(count_output[11:8]), .y(HEX2[6:0]));

     always_ff @(posedge clk) begin
         go<=0; // by default range should not run
         
         // counts out 2^25-1 cycles before LED turns off
         if (led_counter == 0) LEDR[0] <= 0;
         else led_counter <= led_counter + 1;
    
         // cycle after start changes 
         if (waiting == 1) waiting <= waiting + 2'b1;

         // 2 cycles after start changes; count is ready
         if (waiting == 2) begin
             count_output <= count[11:0]; 
             n_output <= (n_0 + start[11:0]);
             waiting <= 0;
         end

         // done for the first time; range complete
         if (done == 1 && ~done_trig) begin
 	    LEDR[0] <= 1;
 	    led_counter <= 1;
            start <= 0;
            waiting <= 1;
            done_trig <= 1;
         end

         // range button is pressed
         if (~KEY[3]) begin
             // button must be pressed for 2^24-1 cycles before registering
            if (go_counter == 24'b0) begin 
                 go <= 1; 
                 start <= {22'b0, SW[9:0]}; 
	         n_0 <= {2'b0, SW[9:0]};
                 go_counter <= 24'b1;
                 done_trig <= 0;
            end else go_counter <= go_counter + 24'b1;
         end else go_counter <= 0;

        // increment button is pressed
        if(~KEY[0]) begin
            if(up_counter == 24'b0 && (start < 256)) begin
                start <= start + 32'b1;
                up_counter <=   24'd4000000;
                waiting <= 1;
            end else up_counter <= up_counter + 24'b1;
 	end else up_counter <= 0;

        // increment button is pressed
        if (~KEY[1]) begin
            if(down_counter == 24'b0 && (start > 0)) begin
                start <= start - 32'b1;
                down_counter <=  24'd4000000; 
                waiting <= 1;
            end else down_counter <= down_counter + 24'b1;
 	end else down_counter <= 0;
        
        // reset button is pressed
        if (~KEY[2]) begin
            if(reset_counter == 24'b0) begin
            LEDR[0] <= 0;
            start <= 32'b0;
            reset_counter <=  24'b1; 
            waiting <= 1;
            // display_output <= n;	
            end else reset_counter <= reset_counter + 24'b1;
 	end else reset_counter <= 0;
    end
 endmodule
