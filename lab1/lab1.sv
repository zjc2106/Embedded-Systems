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

	     output logic [9:0] LEDR // LEDs above the switches; LED[0] on right
	     );

    logic 			clk, go, done;   
    logic [31:0] 		start;
    logic [15:0] 		count;
 
    logic [23:0]                go_counter=1;
    logic [3:0]                 press_counter=0;
    logic                       waiting=0;
    logic                       done_trig = 0;
    logic [15:0]                count_output=0;

    assign clk = CLOCK_50;
   
     range #(256, 8) // RAM_WORDS = 256, RAM_ADDR_BITS = 8)
          r ( .* ); // Connect everything with matching names
 

    // display switch value on 7 seg display
    hex7seg h1(.a(SW[3:0]), .y(HEX3[6:0]));       
    hex7seg h2(.a(SW[7:4]), .y(HEX4[6:0]));       
    hex7seg h3(.a({2'b00, SW[9:8]}), .y(HEX5[6:0]));



    hex7seg h4(.a(count_output[3:0]), .y(HEX0[6:0]));

    hex7seg h5(.a(count_output[7:4]), .y(HEX1[6:0]));

    hex7seg h6(.a(count_output[11:8], .y(HEX2[6:0]));

    always_ff @(posedge clk) begin
        go<=0;
        waiting<=0;
        if (waiting == 1)
        begin
            count_output <= (count-1);
            waiting <= 0;
        end
        if (~KEY[3])
        begin
            if (go_counter == 24'b0)
            begin 
                go <= 1;
                start <= {22'b0, SW[9:0]};
                go_counter <= 24'b1;
                press_counter <= press_counter + 1;
                done_trig <= 0;
            end 
            else begin
                go_counter <= go_counter +  24'b1;
                go <= 0;
            end
        end
        else go_counter <= 0;
        if (done == 1) 
        begin
            start <= 0;
            waiting <= 1;
            done_trig <= 1;
        end
    end

 endmodule
