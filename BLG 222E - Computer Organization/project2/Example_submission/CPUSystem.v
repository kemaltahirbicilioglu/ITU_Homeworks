`timescale 1ns / 1ps

module SequenceCounter (
    input wire Clock,
    input wire Reset,
    input wire Load,
    input wire INC,
    input wire [2:0] Data,
    output reg [2:0] Count
);

  always @(posedge Clock) begin
    if (Reset)  //Set Counter to Zero
      Count <= 0;
    else if (Load)  //load the counter with data value
      Count <= Data;
    else if (INC)  //count up
      Count <= Count + 1;
  end
endmodule

module decoder_3_8 (
    input [2:0] in,
    output reg [7:0] out
);

  always @(*) begin
    case (in)
      3'b000:  out = 8'b00000001;
      3'b001:  out = 8'b00000010;
      3'b010:  out = 8'b00000100;
      3'b011:  out = 8'b00001000;
      3'b100:  out = 8'b00010000;
      3'b101:  out = 8'b00100000;
      3'b110:  out = 8'b01000000;
      3'b111:  out = 8'b10000000;
      default: out = 8'b00000000;  // Default case, all outputs are low
    endcase
  end

endmodule

module CPUSystem (
    input wire Clock,
    input wire Reset,
    output wire [7:0] T
);
  reg SC_Load, SC_Inc, SC_Reset;
  wire [2:0] counter_data;
  reg  [2:0] REACHED = 0;

  SequenceCounter SC (
      .Clock(Clock),
      .Reset(SC_Reset),
      .Load (SC_Load),
      .INC  (SC_Inc),
      .Data (counter_data)
  );
  decoder_3_8 DECODER (.in(SC.Count));
  assign T = DECODER.out;

  reg [2:0] RF_OutASel, RF_OutBSel, RF_FunSel;
  reg [3:0] RF_RegSel, RF_ScrSel;
  reg [4:0] ALU_FunSel;
  reg ALU_WF;
  reg [1:0] ARF_OutCSel, ARF_OutDSel;
  reg [2:0] ARF_FunSel, ARF_RegSel;
  reg IR_LH, IR_Write, Mem_WR, Mem_CS;
  reg [1:0] MuxASel, MuxBSel;
  reg MuxCSel;

  ArithmeticLogicUnitSystem _ALUSystem (
      .Clock(Clock),
      .RF_OutASel(RF_OutASel),
      .RF_OutBSel(RF_OutBSel),
      .RF_FunSel(RF_FunSel),
      .RF_RegSel(RF_RegSel),
      .RF_ScrSel(RF_ScrSel),
      .ALU_FunSel(ALU_FunSel),
      .ALU_WF(ALU_WF),
      .ARF_OutCSel(ARF_OutCSel),
      .ARF_OutDSel(ARF_OutDSel),
      .ARF_FunSel(ARF_FunSel),
      .ARF_RegSel(ARF_RegSel),
      .IR_LH(IR_LH),
      .IR_Write(IR_Write),
      .Mem_WR(Mem_WR),
      .Mem_CS(Mem_CS),
      .MuxASel(MuxASel),
      .MuxBSel(MuxBSel),
      .MuxCSel(MuxCSel)
  );

  task ResetAll(); // disable all the components before starting the simulation.
    begin
      SC.Count = 0;
      RF_OutASel = 0;
      RF_OutBSel = 0;
      RF_FunSel = 0;
      RF_RegSel = 4'b1111;
      RF_ScrSel = 4'b1111;
      ALU_FunSel = 0;
      ALU_WF = 0;
      ARF_OutCSel = 0;
      ARF_OutDSel = 0;
      ARF_FunSel = 0;
      ARF_RegSel = 3'b111;
      IR_LH = 0;
      IR_Write = 0;
      Mem_WR = 0;
      Mem_CS = 1;
      MuxASel = 0;
      MuxBSel = 0;
      MuxCSel = 0;
      _ALUSystem.ARF.PC.Q = 0;
      _ALUSystem.ARF.AR.Q = 16'b0000_0000_0000_0110;
      _ALUSystem.ARF.SP.Q = 0;
      _ALUSystem.RF.R1.Q = 16'b0000_0000_1010_1010;
      _ALUSystem.RF.R2.Q = 0;
      _ALUSystem.RF.R3.Q = 0;
      _ALUSystem.RF.R4.Q = 0;
      _ALUSystem.RF.S1.Q = 0;
      _ALUSystem.RF.S2.Q = 0;
      _ALUSystem.RF.S3.Q = 0;
      _ALUSystem.RF.S4.Q = 0;
      _ALUSystem.ALU.FlagsOut = 4'b0000;
    end
  endtask

  /* In each clock cycle, some of the components are enabled.
  After they are used, they need to be turned into the disabled state so that
  we do not modify registers that are not supposed to change.
  */
  task DisableUnits(); 
    begin
      RF_RegSel = 4'b1111;
      RF_ScrSel = 4'b1111;
      ARF_RegSel = 3'b111;
      Mem_CS = 1;
      ALU_WF = 0;
      IR_Write = 0;
    end
  endtask

  function [3:0] EnableRx; // To be able to find the correct ARF.RegSel for the Instructions type 1. 
    input [1:0] RSEL;
    begin
      case (RSEL)
        2'b00: begin
          EnableRx = 4'b0111;
        end
        2'b01: begin
          EnableRx = 4'b1011;
        end
        2'b10: begin
          EnableRx = 4'b1101;
        end
        2'b11: begin
          EnableRx = 4'b1110;
        end
      endcase
    end
  endfunction

/*
Since all the instruction in type 2 requires only 1 clock cycle when the SREG1,2 and DESTREG
are known. So to generalize type 2 instructions for all combinations of SREG's and DESTREG's, 
write SREG1,2 to S1 and S2 registers in 2 clock cycles. Then, give them to ALU and write 
the ALU Out to the DESTREG. This will ease the process of writing different code blocks 
for each combination of RF and ARF SREGs for each instruction.    
*/

  task Type2InstructionSREG; // T2, write to S1 (ALUFunSel = 10000, ScrSel = 0111); T3, write to S2 (ALUFunSel = 10000, ScrSel = 1011).
    input [2:0] SREG;
    input [7:0] move_T;
    output [1:0] move_ARF_OutCSel;
    output [2:0] move_RF_OutASel;
    output [3:0] move_RF_ScrSel; // for T2 S1 is enabled, for T3 S2 is enabled
    output [1:0] move_MuxASel;  // Decide MuxASel by checking SREG is coming from RF or ARF
    // Common wires for the type instructions when clock T2 and T3  
    output [4:0] move_ALU_FunSel;  
    output [2:0] move_RF_FunSel;   
    
    begin
      move_ALU_FunSel = 5'b10000; // ALU_Out = A
      move_RF_FunSel = 3'b010; // load

      case (SREG[2])
        1'b0: begin
          move_ARF_OutCSel = SREG[1:0];  // SREG selection is the same with the OutCSel
          move_RF_OutASel = 0;  // not important because we do not choose it in MuxASel
          move_MuxASel = 2'b01;  // choose ARF OutC
          if(move_T[2] == 1) begin
            move_RF_ScrSel = 4'b0111;
          end
          if(move_T[3] == 1) begin
            move_RF_ScrSel = 4'b1011;
          end
        end
        1'b1: begin
          move_RF_OutASel = {1'b0, SREG[1:0]};  // SREG selection is the same with the OutASel
          move_ARF_OutCSel = 0;  // not important because we do not choose it in MuxASel
          move_MuxASel = 2'b00;  // choose ALU Out
          if(move_T[2] == 1) begin
            move_RF_ScrSel = 4'b0111;
          end
          if(move_T[3] == 1) begin
            move_RF_ScrSel = 4'b1011;
          end
        end
      endcase
    end
  endtask

  task Type2InstructionDESTREG; // T4 write the ALU Out to the DESTREG. MuxASel and MuxBsel should be 2'b00 for ALUOut
    input [2:0] DESTREG;
    output [2:0] write_ARF_RegSel;
    output [3:0] write_RF_RegSel;

    begin
      case (DESTREG)
        3'b000: begin
          write_ARF_RegSel = 3'b011;  // DESTREG = PC
          write_RF_RegSel  = 4'b1111;  // disable all 
        end
        3'b001: begin
          write_ARF_RegSel = 3'b011;  // DESTREG = PC
          write_RF_RegSel  = 4'b1111;  // disable all             
        end
        3'b010: begin
          write_ARF_RegSel = 3'b110;  // DESTREG = SP
          write_RF_RegSel  = 4'b1111;  // disable all 
        end
        3'b011: begin
          write_ARF_RegSel = 3'b101;  // DESTREG = AR
          write_RF_RegSel  = 4'b1111;  // disable all 
        end
        3'b100: begin
          write_ARF_RegSel = 3'b111;  // disable all  
          write_RF_RegSel  = 4'b0111;  // DESTREG = R1
        end
        3'b101: begin
          write_ARF_RegSel = 3'b111;  // disable all 
          write_RF_RegSel  = 4'b1011;  // DESTREG = R2
        end
        3'b110: begin
          write_ARF_RegSel = 3'b111;  // disable all 
          write_RF_RegSel  = 4'b1101;  // DESTREG = R3
        end
        3'b111: begin
          write_ARF_RegSel = 3'b111;  // disable all 
          write_RF_RegSel  = 4'b1110;  // DESTREG = R4
        end
      endcase
    end
  endtask

// TYPE 2 INSTRUCTIONS

  task Type2InstructionTask();
    begin
      if(T[2])begin
        case (_ALUSystem.IROut[15:10])
          6'b000101, // OP_INC() 
          6'b000110,  // OP_DEC()
          6'b000111,  // OP_LSL()
          6'b001000,  // OP_LSR()
          6'b001001,  // OP_ASR()
          6'b001010,  // OP_CSL()
          6'b001011,  // OP_CSR()
          6'b001100,  // OP_AND()
          6'b001101,  // OP_ORR()
          6'b001110,  // OP_NOT()
          6'b001111,  // OP_XOR()
          6'b010000: begin // OP_NAND()
            Type2InstructionSREG(_ALUSystem.IROut[5:3], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end  
          6'b010001, // OP_MOVH()
          6'b010100: begin // OP_MOVL()
            MuxASel = 2'b11; // IROut[7:0] -> S1
            RF_ScrSel = 4'b0111;
            RF_FunSel = 3'b010;
            SC_Inc = 1;
          end
          6'b010101, // OP_ADD()
          6'b010110, // OP_ADC()
          6'b010111, // OP_SUB()
          6'b011000, // OP_MOVS()
          6'b011001, // OP_ADDS()
          6'b011010, // OP_SUBS()
          6'b011011, // OP_ANDS()
          6'b011100, // OP_ORRS()
          6'b011101: begin // OP_XORS()
            Type2InstructionSREG(_ALUSystem.IROut[5:3], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end
        endcase
      end

      if(T[3])begin
        case (_ALUSystem.IROut[15:10])
          6'b000101: begin // OP_INC()
            RF_ScrSel = 4'b0111;  // Increment S1
            RF_FunSel = 3'b001;
            SC_Inc = 1;
          end
          6'b000110: begin  // OP_DEC()
            RF_ScrSel = 4'b0111;  // Decrement S1
            RF_FunSel = 3'b000;
            SC_Inc = 1;
          end
          6'b000111,  // OP_LSL()
          6'b001000,  // OP_LSR()
          6'b001001,  // OP_ASR()
          6'b001010,  // OP_CSL()
          6'b001011: begin  // OP_CSR()
            SC_Inc = 1; // no need to do anything, wait for the destreg clock.
          end
          6'b001100,  // OP_AND()
          6'b001101: begin  // OP_ORR()
            Type2InstructionSREG(_ALUSystem.IROut[2:0], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end
          6'b001110: begin  // OP_NOT()
            SC_Inc = 1; // no need to do anything, wait for the destreg clock.
          end
          6'b001111,  // OP_XOR()
          6'b010000: begin // OP_NAND()
            Type2InstructionSREG(_ALUSystem.IROut[2:0], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end  
          6'b010001, // OP_MOVH()
          6'b010100: begin // OP_MOVL()
            SC_Inc = 1; // no need to do anything, wait for the destreg clock.          
          end
          6'b010101, // OP_ADD() // Operations need to be done are the same.
          6'b010110, // OP_ADC()
          6'b010111: begin // OP_SUB()
            Type2InstructionSREG(_ALUSystem.IROut[2:0], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end
          6'b011000: begin // OP_MOVS()
            SC_Inc = 1; // no need to do anything, wait for the destreg clock.
          end
          6'b011001, // OP_ADDS()
          6'b011010, // OP_SUBS()
          6'b011011, // OP_ANDS()
          6'b011100, // OP_ORRS()
          6'b011101: begin // OP_XORS()
            Type2InstructionSREG(_ALUSystem.IROut[2:0], T, ARF_OutCSel, RF_OutASel, RF_ScrSel, MuxASel, ALU_FunSel, RF_FunSel);
            SC_Inc = 1;
          end
        endcase
      end

      if(T[4])begin
        RF_FunSel = 3'b010;  // Load
        ARF_FunSel = 3'b010;
        MuxASel = 2'b00;
        MuxBSel = 2'b00;
        RF_OutASel = 3'b100; // S1
        RF_OutBSel = 3'b101; // S2
        Type2InstructionDESTREG(_ALUSystem.IROut[8:6], ARF_RegSel, RF_RegSel);
        case (_ALUSystem.IROut[15:10])          
          6'b000101: begin // OP_INC()
            ALU_FunSel = 5'b10000;
          end
          6'b000110: begin  // OP_DEC()
            ALU_FunSel = 5'b10000;
          end
          6'b000111: begin // OP_LSL()
            ALU_FunSel = 5'b11011; // LSL
          end 
          6'b001000: begin  // OP_LSR()
            ALU_FunSel = 5'b11100; // LSR
          end
          6'b001001: begin  // OP_ASR()
            ALU_FunSel = 5'b11101; // ASR
          end
          6'b001010: begin  // OP_CSL()
            ALU_FunSel = 5'b11110; // CSL
          end
          6'b001011: begin // OP_CSR()
            ALU_FunSel = 5'b11111; // CSR
          end
          6'b001100: begin // OP_AND()
            ALU_FunSel = 5'b10111; // AND
          end  
          6'b001101: begin  // OP_ORR()
            ALU_FunSel = 5'b11000; // OR
          end
          6'b001110: begin  // OP_NOT()
            ALU_FunSel = 5'b10010; // NOT
          end
          6'b001111: begin  // OP_XOR()
            ALU_FunSel = 5'b11001; // XOR
          end
          6'b010000: begin // OP_NAND()
            ALU_FunSel = 5'b11010; // NAND
          end  
          6'b010001: begin // OP_MOVH()
            ALU_FunSel = 5'b10000;
            ARF_FunSel = 3'b110;
            RF_FunSel = 3'b110; // high bits
          end
          6'b010100: begin // OP_MOVL()
            ALU_FunSel = 5'b10000;
            ARF_FunSel = 3'b101;
            RF_FunSel = 3'b101; // low bits
          end
          6'b010101: begin // OP_ADD() 
            ALU_FunSel = 5'b10100; // ADD
          end
          6'b010110: begin  // OP_ADC()
            ALU_FunSel = 5'b10101; // ADD with carry
          end
          6'b010111: begin // OP_SUB()
            ALU_FunSel = 5'b10110; // SUB
          end
          6'b011000: begin // OP_MOVS()
            ALU_WF = 1;
            ALU_FunSel = 5'b10000;
          end
          6'b011001: begin // OP_ADDS()
            ALU_WF = 1;
            ALU_FunSel = 5'b10100; // ADD
          end
          6'b011010: begin // OP_SUBS()
            ALU_WF = 1;
            ALU_FunSel = 5'b10110; // SUB
          end
          6'b011011: begin // OP_ANDS()
            ALU_WF = 1;
            ALU_FunSel = 5'b10111; // AND
          end
          6'b011100: begin// OP_ORRS()
            ALU_WF = 1;
            ALU_FunSel = 5'b11000; // OR
          end
          6'b011101: begin // OP_XORS()
            ALU_WF = 1;
            ALU_FunSel = 5'b11001; // XOR
          end
        endcase
          SC_Inc   = 0;
          SC_Reset = 1;
      end
    end
  endtask

  initial begin
    ResetAll();
  end

  always @* begin
    if (Reset) begin
      ResetAll();
    end
    if (!Reset) begin
      if (T[0] == 1) begin
        DisableUnits(); // problem with the Rx turning back to 0 after an operation is done and SC_Reset. 
        SC_Reset = 0;
        ARF_OutDSel = 2'b00;
        Mem_WR = 0;
        Mem_CS = 0;
        IR_LH = 0;
        IR_Write = 1;
        // Increment program counter
        ARF_FunSel = 3'b001;
        ARF_RegSel = 3'b011;
        // Increment sequence counter
        SC_Inc = 1;
      end
      if (T[1] == 1) begin
        ARF_OutDSel = 2'b00;
        Mem_WR = 0;
        Mem_CS = 0;
        IR_LH = 1;
        IR_Write = 1;
        // Increment program counter
        ARF_FunSel = 3'b001;
        ARF_RegSel = 3'b011;
        // Increment sequence counter
        SC_Inc = 1;
      end
      if (T[2] || T[3] || T[4] || T[5] || T[6] || T[7]) begin
        DisableUnits(); // do not accidentally enable a component that has been enabled in the previous clock signal
        
        case (_ALUSystem.IROut[15:10])
          6'b000000: begin // OP_BRA,  Ram -> 0000_1001|0000_0000, 09|00, PC <- 0002 + 0009 = 000b
            case (T)
              8'b0000_0100: begin
                // S1 <- PC 
                MuxASel = 2'b01;
                RF_ScrSel = 4'b0111;
                RF_FunSel = 3'b010;
                ARF_OutCSel = 2'b00;
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // S2 <- IR[7:0] 
                MuxASel = 2'b11;
                RF_ScrSel = 4'b1011;
                RF_FunSel = 3'b010;
                SC_Inc = 1;
              end
              8'b0001_0000: begin  // not sure to split 2 different clock times. ALU output delay?
                // ALUOut <- S1 + S2
                RF_OutASel = 3'b100;
                RF_OutBSel = 3'b101;
                ALU_FunSel = 5'b10100;  // since PC is 16-bit, 16 bit fun-sel
                // PC <- ALUOut
                MuxBSel = 2'b00;
                ARF_FunSel = 3'b010;
                ARF_RegSel = 3'b011;
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end

          6'b000001: begin // OP_BNE, Ram -> 0000_1001|0000_0100, 09|04, if Z == 0 then PC <- 000b
            if (!_ALUSystem.ALU.FlagsOut[3]) begin // If satisfied, apply OP_BRA
              case (T)
                8'b0000_0100: begin
                  // S1 <- PC 
                  MuxASel = 2'b01;
                  RF_ScrSel = 4'b0111;
                  RF_FunSel = 3'b010;
                  ARF_OutCSel = 2'b00;
                  SC_Inc = 1;
                end
                8'b0000_1000: begin
                  // S2 <- IR[7:0] 
                  MuxASel = 2'b11;
                  RF_ScrSel = 4'b1011;
                  RF_FunSel = 3'b010;
                  SC_Inc = 1;
                end
                8'b0001_0000: begin  // not sure to split 2 different clock times. ALU output delay?
                  // ALUOut <- S1 + S2
                  RF_OutASel = 3'b100;
                  RF_OutBSel = 3'b101;
                  ALU_FunSel = 5'b10100;  // since PC is 16-bit, 16 bit fun-sel
                  // PC <- ALUOut
                  MuxBSel = 2'b00;
                  ARF_FunSel = 3'b010;
                  ARF_RegSel = 3'b011;
                  SC_Inc = 0;
                  SC_Reset = 1;
                end
              endcase
            end else begin
              SC_Inc   = 0;
              SC_Reset = 1;
            end
          end

          6'b000010: begin // OP_BEQ Ram -> 0000_1001|0000_1000, 09|08, if Z == 1 then PC <- 000b
            if (_ALUSystem.ALU.FlagsOut[3]) begin // If satisfied, apply OP_BRA
              case (T)
                8'b0000_0100: begin
                  // S1 <- PC 
                  MuxASel = 2'b01;
                  RF_ScrSel = 4'b0111;
                  RF_FunSel = 3'b010;
                  ARF_OutCSel = 2'b00;
                  SC_Inc = 1;
                end
                8'b0000_1000: begin
                  // S2 <- IR[7:0] 
                  MuxASel = 2'b11;
                  RF_ScrSel = 4'b1011;
                  RF_FunSel = 3'b010;
                  SC_Inc = 1;
                end
                8'b0001_0000: begin  // not sure to split 2 different clock times. ALU output delay?
                  // ALUOut <- S1 + S2
                  RF_OutASel = 3'b100;
                  RF_OutBSel = 3'b101;
                  ALU_FunSel = 5'b10100;  // since PC is 16-bit, 16 bit fun-sel
                  // PC <- ALUOut
                  MuxBSel = 2'b00;
                  ARF_FunSel = 3'b010;
                  ARF_RegSel = 3'b011;
                  SC_Inc = 0;
                  SC_Reset = 1;
                end
              endcase
            end else begin
              SC_Inc   = 0;
              SC_Reset = 1;
            end
          end

          6'b000011: begin // OP_POP, Ram -> 0000_0000|0000_1101, 00|0d, SP.Q = 0005, MEM[7. line] = cc, R2 <- MEM[7. line]
              case (T)
                8'b0000_0100: begin
                  // SP <- SP + 1
                  ARF_RegSel = 3'b110;
                  ARF_FunSel = 3'b001;
                  SC_Inc = 1;
                end
                8'b0000_1000: begin
                  // Rx <- M[SP]
                  Mem_CS = 0;
                  Mem_WR = 0;
                  ARF_OutDSel = 2'b11;
                  MuxASel = 2'b10;
                  RF_FunSel = 3'b010;
                  RF_RegSel = EnableRx(_ALUSystem.IROut[9:8]);  // Find Rx
                  SC_Inc = 0;
                  SC_Reset = 1;
                end
              endcase
          end

          6'b000100: begin // OP_PSH, Ram -> 0000_0000|0001_0001, 00|11, SP.Q = 0006, R2 = bbdd, MEM[7. line] = cc, MEM[8. line] = aa, MEM[SP] <- R2
            case (T)
              8'b0000_0100: begin
                // M[SP] <- Rx(LSB)
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                ARF_OutDSel = 2'b11;
                // Increment SP to be able to write the MSB of the Rx in the next clock cycle
                ARF_RegSel = 3'b110;
                ARF_FunSel = 3'b001;
                MuxCSel = 1'b0;  // Write LSB
                Mem_WR = 1;
                Mem_CS = 0;
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // M[SP] <- Rx(MSB)
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                ARF_OutDSel = 2'b11;
                MuxCSel = 1'b1;  // Write MSB
                Mem_WR = 1;
                Mem_CS = 0;
                // SP <- SP - 1
                ARF_RegSel = 3'b110;
                ARF_FunSel = 3'b000;
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end

          6'b010010: begin // OP_LDR, Ram -> 0000_0000|0100_1011, 00|4B, AR.Q = 0006, R4 = 0003, MEM[7. line] = cc, MEM[8. line] = aa, R4 <- MEM[AR]
            case (T)
              8'b0000_0100: begin
                // Rx <- M[AR]
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 0;
                MuxASel = 2'b10;
                RF_FunSel = 3'b101;  // Only write low
                RF_RegSel = EnableRx(_ALUSystem.IROut[9:8]);  // Find Rx
                // Increment AR to be able to write the next memory line to Rx in the next clock cycle.
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b001;
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // Rx <- M[AR]
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 0;
                MuxASel = 2'b10;
                RF_FunSel = 3'b110;  // Only write high
                RF_RegSel = EnableRx(_ALUSystem.IROut[9:8]);  // Find Rx
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end

          6'b010011: begin // OP_STR, Ram -> 0000_0000|0100_1111, 00|4F, R4 = bbdd, AR.Q = 0006, MEM[7. line] <- R4(LSB), MEM[8. line] <- R4(MSB)
            case (T)
              8'b0000_0100: begin
                // M[AR] <- Rx(LSB)
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                ARF_OutDSel = 2'b10;
                MuxCSel = 1'b0;  // ALUOut LSB bits selected to be written on memory.
                Mem_WR = 1;
                Mem_CS = 0;
                // Increment AR to be able to write the MSB of Rx in the next clock cycle.
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b001;
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // M[AR] <- Rx(MSB)
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                ARF_OutDSel = 2'b10;
                MuxCSel = 1'b1;  // ALUOut MSB bits selected to be written on memory.
                Mem_WR = 1;
                Mem_CS = 0;
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end

          6'b011110: begin // OP_BX, Ram -> 0000_0000|0111_1011, 00|7B, R4 = 00cc, SP.Q = 0006, MEM[7. line] <- PC, PC <- R4 
            case (T)
              8'b0000_0100: begin
                // S1 <- PC
                MuxASel = 2'b01;
                RF_ScrSel = 4'b0111;
                RF_FunSel = 3'b010;
                ARF_OutCSel = 2'b00;
                //PC <- Rx
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                MuxBSel = 2'b00;
                ARF_RegSel = 3'b011;
                ARF_FunSel = 3'b010;
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // M[SP] <- PC == M[SP] <- S1
                RF_OutASel = 3'b100;
                ALU_FunSel = 5'b10000;
                ARF_OutDSel = 2'b11;
                MuxCSel = 1'b0;  // ALUOut LSB bits selected to be written on memory.
                Mem_WR = 1;
                Mem_CS = 0;
                SC_Inc = 0;
                SC_Reset = 1;
              end
          endcase
          end

          6'b011111: begin // OP_BL, Ram ->  0000_0000|0111_1100, 00|7C, SP.Q = 0006, MEM[7. line] = cc, PC <- 00cc
            case (T)
              8'b0000_0100: begin
                // PC <- M[SP]
                ARF_OutDSel = 2'b11;
                Mem_WR = 0;
                Mem_CS = 0;
                MuxBSel = 2'b10;
                ARF_RegSel = 3'b011;
                ARF_FunSel = 3'b010;  // Load to PC  
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end

          6'b100000: begin //OP_LDRIM, Ram ->  0000_0111|1000_0001, 07|81, MEM[8. line] = aa,  R2 <- MEM[8. line] 
            case (T)
              8'b0000_0100: begin
                // AR <- IR[7:0]
                MuxBSel = 2'b11;
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b010;  // Load to AR 
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // Rx <- MEM[AR]
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 0;
                MuxASel = 2'b10;
                RF_RegSel = EnableRx(_ALUSystem.IROut[9:8]);  // Find Rx
                RF_FunSel = 3'b010;
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end
          
          6'b100001: begin //OP_STRIM, Ram ->  0000_0110|1000_0101, 06|85, AR = 0001, R2 = aacc, MEM[7.,8. lines] = 05,00, MEM[7.,8. lines] <- R2
            case (T)
              8'b0000_0100: begin
                // AR <- IR[7:0]
                MuxBSel = 2'b11;
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b010;  // Load to AR 
                // S1 <- AR
                ARF_OutCSel = 2'b10;
                MuxASel = 2'b01;
                RF_ScrSel = 4'b0111;
                RF_FunSel = 3'b010;  // Load to S1
                SC_Inc = 1;
              end
              8'b0000_1000: begin
                // S2(LSB) <- MEM[AR], 
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 0;
                MuxASel = 2'b10;
                RF_ScrSel = 4'b1011;  // 
                RF_FunSel = 3'b101;  // Load to LSB of S2
                //AR <- AR + 1
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b001;  // Increment AR 
                SC_Inc = 1;
              end
              8'b0001_0000: begin
                // S2(MSB) <- MEM[AR]
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 0;
                MuxASel = 2'b10;
                RF_ScrSel = 4'b1011;
                RF_FunSel = 3'b110;  // Load to MSB of S2, now S2 = OFFSET        
                SC_Inc = 1;
              end
              8'b0010_0000: begin
                // AR <- S1 + S2
                RF_OutASel = 3'b100;
                RF_OutBSel = 3'b101;
                ALU_FunSel = 5'b10100;  // S1+S2
                MuxBSel = 2'b00;
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b010;  // Load to AR 
                SC_Inc = 1;
              end
              8'b0100_0000: begin
                // M[AR] <- Rx  
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                MuxCSel = 1'b0;  // write low bits
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 1;
                // AR <- AR + 1;
                ARF_RegSel = 3'b101;
                ARF_FunSel = 3'b001;
                SC_Inc = 1;
              end
              8'b1000_0000: begin
                // M[AR] <- Rx  
                RF_OutASel = {1'b0, _ALUSystem.IROut[9:8]};
                ALU_FunSel = 5'b10000;
                MuxCSel = 1'b1;  // write high bits
                ARF_OutDSel = 2'b10;
                Mem_CS = 0;
                Mem_WR = 1;
                SC_Inc = 0;
                SC_Reset = 1;
              end
            endcase
          end
          6'b000101, // OP_INC() Ram -> 1010_0000|0001_0100  A0|14, SREG1 = R1, DESTREG = SP => R1 + 1 = SP
          6'b000110, // OP_DEC() Ram -> 1110_1000|0001_1000  E8|18, SREG1 = R2, DESTREG = AR => R2 - 1 = AR
          6'b000111, // OP_LSL() Ram -> 1110_1000|0001_1100  E8|1C, SREG1 = R2, DESTREG = AR => R2 LSL = AR
          6'b001000, // OP_LSR() Ram -> 1110_1000|0010_0000  E8|20, SREG1 = R2, DESTREG = AR => R2 LSR = AR
          6'b001001, // OP_ASR() Ram -> 1110_1000|0010_0100  E8|24, SREG1 = R2, DESTREG = AR => R2 ASR = AR
          6'b001010, // OP_CSL() Ram -> 1110_1000|0010_1000  E8|28, SREG1 = R2, DESTREG = AR => R2 ASR = AR
          6'b001011, // OP_CSR() Ram -> 1110_1000|0010_1100  E8|2c, SREG1 = R2, DESTREG = AR => R2 ASR = AR
          6'b001100, // OP_AND() Ram -> 1110_0000|0011_0000  E0|30, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 AND PC
          6'b001101, // OP_ORR() Ram -> 1110_0000|0011_0100  E0|34, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 OR PC
          6'b001110, // OP_NOT() Ram -> 1110_0000|0011_1000  E0|38, SREG1 = R1, DESTREG = AR => AR = NOT R1
          6'b001111, // OP_XOR() Ram -> 1110_0000|0011_1100  E0|3c, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 XOR PC
          6'b010000, // OP_NAND()Ram -> 1110_0000|0100_0000  E0|40, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 NAND PC
          6'b010001, // OP_MOVH()Ram -> 1110_0000|0100_0100  E0|44, AR[15:8] = IMMEDIATE
          6'b010100, // OP_MOVL()Ram -> 1110_0000|0101_0000  E0|50, AR[7:0] = IMMEDIATE
          6'b010101, // OP_ADD() Ram -> 1110_0000|0101_0100  E0|54, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 + PC
          6'b010110, // OP_ADC() Ram -> 1110_0000|0101_1000  E0|58, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 + PC + Carry
          6'b010111, // OP_SUB() Ram -> 1110_0000|0101_1100  E0|5c, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 - PC
          6'b011000, // OP_MOVS()Ram -> 1110_0000|0110_0000  E0|60, SREG1 = R1, DESTREG = AR => AR = R1
          6'b011001, // OP_ADDS()Ram -> 1110_0000|0110_0100  E0|64, SREG1 = R1, DESTREG = AR => AR = R1
          6'b011010, // OP_SUBS()Ram -> 1110_0000|0110_1000  E0|68, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 - PC
          6'b011011, // OP_ANDS()Ram -> 1110_0000|0110_1100  E0|6c, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 AND PC
          6'b011100, // OP_ORRS()Ram -> 1110_0000|0111_0000  E0|70, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 OR PC
          6'b011101: // OP_XORS()Ram -> 1110_0000|0111_0100  E0|74, SREG1 = R1, SREG2 = PC, DESTREG = AR => AR = R1 XOR PC
            begin
              Type2InstructionTask();
            end
        endcase
      end
    end
  end
endmodule