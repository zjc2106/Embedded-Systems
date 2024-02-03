module collatz( input logic         clk,   // Clock
		input logic 	    go,    // Load value from n; start iterating
		input logic  [31:0] n,     // Start value; only read when go = 1
		output logic [31:0] dout,  // Iteration value: true after go = 1
		output logic 	    done); // True when dout reaches 1

   always_ff @(posedge clk) begin
      if (go)		  dout <= n;
      else if (~done)
	if (dout > 1'd1)
	  if (dout[0] == 0) dout <= (dout >> 1);
	  else 		    dout <= ((3*dout)+1);
   end
   
   assign done = dout == 32'd1;
endmodule
