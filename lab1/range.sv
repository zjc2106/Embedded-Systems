module range
   #(parameter
     RAM_WORDS = 16,            // Number of counts to store in RAM
     RAM_ADDR_BITS = 4)         // Number of RAM address bits
   (input logic         clk,    // Clock
    input logic 	go,     // Read start and start testing
    input logic [31:0] 	start,  // Number to start from or count to read
    output logic 	done,   // True once memory is filled
    output logic [15:0] count); // Iteration count once finished

   logic 		cgo;    // "go" for the Collatz iterator
   logic                cdone;  // "done" from the Collatz iterator
   logic [31:0] 	n;      // number to start the Collatz iterator

// verilator lint_off PINCONNECTEMPTY
   
   // Instantiate the Collatz iterator
   collatz c1(.clk(clk),
	      .go(cgo),
	      .n(n),
	      .done(cdone),
	      .dout());

   logic [RAM_ADDR_BITS - 1:0] 	 num;         // The RAM address to write
   logic 			 running = 0; // True during the iterations
   // triggers to reset we and din at the right cycles
   logic                         we_trig = 0;
   logic                         din_trig = 0;
   
   always_ff @(posedge clk) begin 
      // reset default vars
      cgo <= 0;
      done <= 0;
      we_trig <= 0;
      din_trig <= 0;
      // start of iterations
      if (go)
      begin
        running <= go;
	n <= start;
	num <= 0;
	din <= 1;
	cgo <= 1;
      end
      // iterate din
      else if (running & (~cgo) & (~cdone))   din <= (din + 1);
      // when iteration is done, trigger we
      else if (cdone)                         we_trig <= 1;
      // check to see if iterations are done
      if (we)
        begin
      if (({1'b0, num[RAM_ADDR_BITS-1:0]} == RAM_WORDS-1))
      begin
        running <= 0;
	done <= 1;
      end
      // after write, reset vars
      else  
      begin
	n <= (n+1);
	num <= (num+1);
	din_trig <= 1;
	cgo <= 1;
      end
end
      // check to reset din
      if (din_trig) din <= 1;
    end
    // assign we here so that it immediately updates with cdone
    assign we = cdone == 1 && we_trig == 0 && (din != 1||start==1);
   
   logic                         we;
   logic [15:0] 		 din;                   // Data to write
   logic [15:0] 		 mem[RAM_WORDS - 1:0];  // The RAM itself
   logic [RAM_ADDR_BITS - 1:0] 	 addr;                  // Address to read/write

   assign addr = we ? num : start[RAM_ADDR_BITS-1:0];
   
   always_ff @(posedge clk) begin
      if (we) mem[addr] <= din;
      count <= mem[addr];      
   end
   
endmodule
	     
