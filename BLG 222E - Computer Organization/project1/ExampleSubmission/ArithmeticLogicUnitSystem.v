`timescale 1ns / 1ps
module MUX4_1 (
    input  wire [15:0] in1, in2, in3, in4,
    input  wire [1:0] sel,
    output reg  [15:0] out
);
  always @(*) begin
    case (sel)
      2'b00: out = in1;
      2'b01: out = in2;
      2'b10: out = in3;
      2'b11: out = in4;
    endcase
  end
endmodule

module MUX2_1 (
    input wire [7:0] in1, in2,
    input wire sel,
    output reg [7:0] out
);
  always @(*) begin
    case (sel)
      1'b0: out = in1;
      1'b1: out = in2;
    endcase
  end
endmodule

module ArithmeticLogicUnitSystem (
    input wire [2:0] RF_OutASel, RF_OutBSel, RF_FunSel,
    input wire [3:0] RF_RegSel, RF_ScrSel,
    input wire [4:0] ALU_FunSel,
    input wire ALU_WF,
    input wire [1:0] ARF_OutCSel, ARF_OutDSel,
    input wire [2:0] ARF_FunSel, ARF_RegSel,
    input wire IR_LH, IR_Write, Mem_WR, Mem_CS,
    input wire [1:0] MuxASel, MuxBSel,
    input wire MuxCSel, Clock
);
  // not in the called parameters but they are set in the test cases.
  wire [15:0] OutA, OutB, ALUOut, MuxAOut, MuxBOut, OutC, OutD, IROut, Address;
  wire [7:0] MuxCOut, MemOut;
  wire [3:0] FlagsOut;
  
  MUX4_1 MuxA (
      .in1(ALU.ALUOut),
      .in2(ARF.OutC),
      .in3({8'b0, MEM.MemOut}),
      .in4({8'b0, IR.IROut[7:0]}),
      .sel(MuxASel),
      .out(MuxAOut)
  );

  MUX4_1 MuxB (
      .in1(ALU.ALUOut),
      .in2(ARF.OutC),
      .in3({8'b0, MEM.MemOut}),
      .in4({8'b0, IR.IROut[7:0]}),
      .sel(MuxBSel),
      .out(MuxBOut)
  );

  MUX2_1 MuxC (
      .in1(ALU.ALUOut[7:0]),
      .in2(ALU.ALUOut[15:8]),
      .sel(MuxCSel),
      .out(MuxCOut)
  );

  ArithmeticLogicUnit ALU (
      .A(RF.OutA),
      .B(RF.OutB),
      .FunSel(ALU_FunSel),
      .WF(ALU_WF),
      .Clock(Clock),
      .ALUOut(ALUOut),
      .FlagsOut(FlagsOut)
  );

  AddressRegisterFile ARF (
      .I(MuxB.out),
      .OutCSel(ARF_OutCSel),
      .OutDSel(ARF_OutDSel),
      .FunSel(ARF_FunSel),
      .RegSel(ARF_RegSel),
      .Clock(Clock),
      .OutC(OutC),
      .OutD(OutD)
  );

  Memory MEM (
      .Data(MuxC.out),
      .Address(ARF.OutD),
      .CS(Mem_CS),
      .WR(Mem_WR),
      .Clock(Clock),
      .MemOut(MemOut)
  );
  assign Address = ARF.OutD;

  InstructionRegister IR (
      .I(MEM.MemOut),
      .LH(IR_LH),
      .Write(IR_Write),
      .Clock(Clock),
      .IROut(IROut)
  );

  RegisterFile RF (
      .I(MuxA.out),
      .OutASel(RF_OutASel),
      .OutBSel(RF_OutBSel),
      .FunSel(RF_FunSel),
      .RegSel(RF_RegSel),
      .ScrSel(RF_ScrSel),
      .Clock(Clock),
      .OutA(OutA),
      .OutB(OutB)
  );

endmodule
