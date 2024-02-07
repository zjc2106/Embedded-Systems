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
   logic                         test = 0;
   logic                         test1 = 0;
   /* Replace this comment and the code below with your solution,
      which should generate running, done, cgo, n, num, we, and din */
   always_ff @(posedge clk) begin 
      //we <= 0;
      cgo <= 0;
      done <= 0;
      test <= 0;
      test1 <= 0;
      if (test1) din <= 1;
      if (we)
      begin
	n <= (n+1);
	num <= (num+1);
	//din <= 1;
	test1 <= 1;
	cgo <= 1;
	test <= 0;
      end
      if (go)
      begin
        running <= go;
	n <= start;
	num <= 0;
	din <= 1;
	cgo <= 1;
      end
      else if (running & (~cgo) & (~cdone)) din <= (din + 1);
      else if (cdone && (~test))
      begin
	//we <= 1;
	test <= 1;
      end
      //else we <= 0;
      if ((num[3:0] == 4'd15)&&(we))
      begin
        running <= 0;
	done <= 1;
      end
    end
    assign we = cdone == 1 && test == 0;
      //assign we = cdone == 1;
      //assign cgo = ((cdone == 1) || (running == 1));
      //assign done = num[3:0] == 4'd15;
	/* Replace this comment and the code above with your solution */

   logic 			 we;                    // Write din to addr
   logic [15:0] 		 din;                   // Data to write
   logic [15:0] 		 mem[RAM_WORDS - 1:0];  // The RAM itself
   logic [RAM_ADDR_BITS - 1:0] 	 addr;                  // Address to read/write

   assign addr = we ? num : start[RAM_ADDR_BITS-1:0];
   
   always_ff @(posedge clk) begin
      if (we) mem[addr] <= din;
      count <= mem[addr];      
   end
   
/*   always_ff @(posedge clk) begin
      test <= 0;
      cgo <= 0;
      we <= 0;
      done <= 0;
   end*/
endmodule
	     
