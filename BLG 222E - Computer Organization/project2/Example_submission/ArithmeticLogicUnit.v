`timescale 1ns / 1ps
module ArithmeticLogicUnit (
    input wire [15:0] A,
    input wire [15:0] B,
    input wire [4:0] FunSel,
    input wire WF,
    input wire Clock,
    output reg [15:0] ALUOut,
    output reg [3:0] FlagsOut
);

  reg [16:0] carryChecker; // To be able to detect carry bit. Carry bit is 17. bit of the ALUOut for 16 bit inputs A and B.
  reg [15:0] extendedA, extendedB; // Use the same operations as the 16 bit cases for 8 bit cases by extending A and B's 7. bit.

  always @(*) begin
    if (~FunSel[4]) begin
      extendedA = {8'b0, A[7:0]};
      extendedB = {8'b0, B[7:0]};
    end
    case (FunSel)
      5'b00000: begin
        ALUOut = extendedA;
      end
      5'b00001: begin
        ALUOut = extendedB;
      end

      5'b00010: begin
        ALUOut = ~extendedA;
      end
      5'b00011: begin
        ALUOut = ~extendedB;
      end

      5'b00100: begin
        carryChecker = extendedA + extendedB;
        ALUOut = extendedA + extendedB;
      end
      5'b00101: begin
        carryChecker = extendedA + extendedB;
        ALUOut = extendedA + extendedB;
        ALUOut = ALUOut + FlagsOut[2];
      end
      5'b00110: begin
        carryChecker = extendedA - extendedB;
        ALUOut = extendedA - extendedB;
      end

      5'b00111: ALUOut = extendedA & extendedB;
      5'b01000: ALUOut = extendedA | extendedB;
      5'b01001: ALUOut = extendedA ^ extendedB;
      5'b01010: ALUOut = ~(extendedA & extendedB);

      5'b01011: begin
        ALUOut = extendedA << 1;
      end
      5'b01100: begin
        ALUOut = extendedA >> 1;
      end
      5'b01101: begin
        ALUOut = extendedA >>> 1;
      end
      5'b01110: begin
        ALUOut = {extendedA[14:0], extendedA[15]};
      end
      5'b01111: begin
        ALUOut = {extendedA[0], extendedA[15:1]};
      end

      // 16 bit operations
      5'b10000: begin
        ALUOut = A;
      end
      5'b10001: begin
        ALUOut = B;
      end
      5'b10010: begin
        ALUOut = ~A;
      end
      5'b10011: begin
        ALUOut = ~B;
      end
      5'b10100: begin
        carryChecker = A + B;
        ALUOut = A + B;
      end
      5'b10101: begin
        carryChecker = A + B; // Check the carry bit with carryChecker 17bit wire.
        ALUOut = A + B;
        ALUOut = ALUOut + FlagsOut[2];
      end
      5'b10110: begin
        carryChecker = A - B;
        ALUOut = A - B;
      end

      5'b10111: ALUOut = A & B;
      5'b11000: ALUOut = A | B;
      5'b11001: ALUOut = A ^ B;
      5'b11010: ALUOut = ~(A & B);

      5'b11011: begin
        ALUOut = A << 1;
      end
      5'b11100: begin
        ALUOut = A >> 1;
      end
      5'b11101: begin
        ALUOut = A >>> 1;
      end
      5'b11110: begin
        ALUOut = {A[14:0], A[15]};
      end
      5'b11111: begin
        ALUOut = {A[0], A[15:1]};
      end
    endcase 
  end

  /*
    Cases with no statements means that their corresponding flag checks will be done after case blocks are finished.
  */
  always @(posedge Clock) begin
    if (WF) begin
      case (FunSel)
        // 8 bit operations
        5'b00000: begin end
        5'b00001: begin end
        5'b00010: begin end 
        5'b00011: begin end
        5'b00100: begin
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (extendedA[15] && extendedB[15] && ~ALUOut[7]) || (~extendedA[15] && ~extendedB[15] && ALUOut[7]); // Check: pos + pos = neg and neg + neg = pos
        end
        5'b00101: begin
          FlagsOut[3] = ALUOut == 0;
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (extendedA[15] && extendedB[15] && ~ALUOut[7]) || (~extendedA[15] && ~extendedB[15] && ALUOut[7]);
        end
        5'b00110: begin
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (~extendedA[15] && extendedB[15] && ALUOut[7]) || (extendedA[15] && ~extendedB[15] && ~ALUOut[7]); // Check: pos - neg = neg and neg - pos = pos
        end
        5'b00111: begin end
        5'b01000: begin end
        5'b01001: begin end
        5'b01010: begin end
        5'b01011: begin
          FlagsOut[2] = extendedA[15];  // Carry flag
        end
        5'b01100: begin
          FlagsOut[2] = extendedA[0];  // Carry flag
        end
        5'b01101: begin
          FlagsOut[2] = extendedA[0];  // Carry flag
        end
        5'b01110: begin
          FlagsOut[2] = extendedA[15];  // Carry flag
        end
        5'b01111: begin
          FlagsOut[2] = extendedA[0];  // Carry flag
        end

        // 16 bit operations
        5'b10000: begin end
        5'b10001: begin end
        5'b10010: begin end
        5'b10011: begin end
        5'b10100: begin
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (A[15] && B[15] && ~ALUOut[15]) || (~A[15] && ~B[15] && ALUOut[15]); // Check: pos + pos = neg and neg + neg = pos
        end
        5'b10101: begin
          FlagsOut[3] = ALUOut == 0;
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (A[15] && B[15] && ~ALUOut[15]) || (~A[15] && ~B[15] && ALUOut[15]);
        end
        5'b10110: begin
          FlagsOut[2] = carryChecker[16];
          FlagsOut[0] = (~A[15] && B[15] && ALUOut[15]) || (A[15] && ~B[15] && ~ALUOut[15]); // Check: pos - neg = neg and neg - pos = pos
        end

        5'b11011: begin
          FlagsOut[2] = A[15];  // Carry flag
        end
        5'b11100: begin
          FlagsOut[2] = A[0];  // Carry flag
        end
        5'b11101: begin
          FlagsOut[2] = A[0];  // Carry flag
        end
        5'b11110: begin
          FlagsOut[2] = A[15];  // Carry flag
        end
        5'b11111: begin
          FlagsOut[2] = A[0];  // Carry flag
        end
      endcase
      // N flag check
      if (~(FunSel[3:0] == 4'b1101)) begin
        if (FunSel[4]) begin
          FlagsOut[1] = ALUOut[15];
        end else begin
          FlagsOut[1] = ALUOut[7];
        end
      end

      // Z flag check, needs to be set every time, not for 0101 case since the carry addition needs to be done after zero check.
      if (~(FunSel[3:0] == 4'b0101)) begin
        FlagsOut[3] = ALUOut == 0;
      end
    end
  end

endmodule
