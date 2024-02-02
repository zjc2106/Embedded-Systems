module hex7seg(input logic  [3:0] a,
	       output logic [6:0] y);

   /* Replace this comment and the code below it with your solution */
   // assign y = { a[2:0], a[3:0] };
   
   always_comb
	   case(a)
		   4'h0: y = 7'b1000000;
		   4'h1: y = 7'b1111001;
		   4'h2: y = 7'b0100100;
		   4'h3: y = 7'b0110000;
		   4'h4: y = 7'b0011001;
		   4'h5: y = 7'b0010010;
		   4'h6: y = 7'b0000010;
		   4'h7: y = 7'b1111000;
		   4'h8: y = 7'b0000000;
		   4'h9: y = 7'b0010000;
		   4'ha: y = 7'b0001000; 
		   4'hb: y = 7'b0000011;
		   4'hc: y = 7'b1000110;
		   4'hd: y = 7'b0100001;
		   4'he: y = 7'b0000110;
		   4'hf: y = 7'b0001110;
		   default: y = 7'b0000000;
	   endcase
	
endmodule
