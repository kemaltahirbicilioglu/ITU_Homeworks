`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/25/2024 05:02:17 PM
// Design Name: 
// Module Name: CPUSimulation
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Project2Test();
    reg Clock, Reset;
    wire [7:0] Tx;
    wire [15:0] PCQ;
    wire [15:0] IRQ;
    wire [15:0] SPQ;
    wire [15:0] ARQ;
    wire [15:0] R1Q;
    wire [15:0] R2Q;
    wire [15:0] R4Q;
    wire [15:0] S1Q;
    wire [15:0] S2Q;
    wire [15:0] ALUOut;
    wire [3:0] ALUFlags;
    wire [7:0] RAM_DATA1;
    wire [7:0] RAM_DATA2;
    wire  [2:0] REACHED;
    always 
    begin
        Clock = 1; 
        #50; 
        Clock = 0; 
        #50; // 50 ns period
    end
    initial begin
       #1;
       Reset = 1;
       #1;
       Reset = 0;
       #1;
    end
    CPUSystem _CPUSystem( 
            .Clock(Clock),
            .Reset(Reset),
            .T(Tx)    
        );
    assign PCQ = _CPUSystem._ALUSystem.ARF.PC.Q;
    assign IRQ = _CPUSystem._ALUSystem.IR.IROut;
    assign SPQ = _CPUSystem._ALUSystem.ARF.SP.Q;
    assign ARQ = _CPUSystem._ALUSystem.ARF.AR.Q;
    assign R1Q = _CPUSystem._ALUSystem.RF.R1.Q;
    assign R2Q = _CPUSystem._ALUSystem.RF.R2.Q;
    assign R4Q = _CPUSystem._ALUSystem.RF.R4.Q;
    assign S1Q = _CPUSystem._ALUSystem.RF.S1.Q;
    assign S2Q = _CPUSystem._ALUSystem.RF.S2.Q;
    assign ALUOut = _CPUSystem._ALUSystem.ALUOut;
    assign ALUFlags = _CPUSystem._ALUSystem.ALU.FlagsOut;
    assign RAM_DATA1 = _CPUSystem._ALUSystem.MEM.RAM_DATA[6];
    assign RAM_DATA2 = _CPUSystem._ALUSystem.MEM.RAM_DATA[7];
    assign REACHED = _CPUSystem.REACHED;
    always @(negedge Clock)
        if (Reset) // skip during reset
        begin
            $display("Output Values:");
            $display("T: %d", Tx);
            $display("Address Register File: PC: %d, AR: %d, SP: %d", _CPUSystem._ALUSystem.ARF.PC.Q, _CPUSystem._ALUSystem.ARF.AR.Q, _CPUSystem._ALUSystem.ARF.SP.Q);
            $display("Instruction Register : %d", _CPUSystem._ALUSystem.IR.IROut);
            $display("Register File Registers: R1: %d, R2: %d, R3: %d, R4: %d", _CPUSystem._ALUSystem.RF.R1.Q, _CPUSystem._ALUSystem.RF.R2.Q, _CPUSystem._ALUSystem.RF.R3.Q, _CPUSystem._ALUSystem.RF.R4.Q);
            $display("Register File Scratch Registers: S1: %d, S2: %d, S3: %d, S4: %d", _CPUSystem._ALUSystem.RF.S1.Q, _CPUSystem._ALUSystem.RF.S2.Q, _CPUSystem._ALUSystem.RF.S3.Q, _CPUSystem._ALUSystem.RF.S4.Q);
            $display("ALU Flags: Z: %d, N: %d, C: %d, O: %d", _CPUSystem._ALUSystem.ALU.FlagsOut[3], _CPUSystem._ALUSystem.ALU.FlagsOut[2], _CPUSystem._ALUSystem.ALU.FlagsOut[1], _CPUSystem._ALUSystem.ALU.FlagsOut[0]);
            $display("ALU Result: ALUOut: %d", _CPUSystem._ALUSystem.ALU.ALUOut);
            $display("\n");
        end
endmodule
