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

   logic                         we_trig = 0; // Ensures we is high for 1 cycle
   logic                         din_trig = 0; // Ensures din keeps val for extra cycle 
   
   always_ff @(posedge clk) begin

      // reset default values
      cgo <= 0; 
      done <= 0; 
      we_trig <= 0;
      din_trig <= 0; 

      // start of collatz iterations
      if (go) begin
        running <= go; // running goes high
	n <= start; // start is fed to n (for collatz)
	num <= 0; // RAM address reset to 0
	din <= 1; // Iterations for collatz reset
	cgo <= 1; // starts collatz
      end

      // iterate din while collatz is running
      else if (running & (~cgo) & (~cdone))   din <= (din + 1);
      
      // when collatz is done, trigger we one cycle after
      else if (cdone)                         we_trig <= 1;
      
      if (we) begin       
        // check to see if memory is full
        if (({1'b0, num[RAM_ADDR_BITS-1:0]} == RAM_WORDS-1)) begin
          running <= 0;
          done <= 1;
        end
        
        // if not full, iterate and run collatz
        else begin
          n <= (n+1);
          num <= (num+1);
          din_trig <= 1;
          cgo <= 1;
        end
      end
      
      // resets din one cycle after cgo goes high
      if (din_trig) din <= 1;
    
    end
    
    // ensures we immediately updates with cdone
    // check for din ensures we isn't high at the start
    // of multiple runs of range (for lab1.sv)
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
	     
