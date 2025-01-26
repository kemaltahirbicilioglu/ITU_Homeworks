`timescale 1ns / 1ps
module InstructionRegister(input wire[7:0] I, input wire Write, input wire LH, input wire Clock, output reg[15:0] IROut);
    always @(posedge Clock) begin
        if (Write) begin
            case (LH)
                1'b0: IROut[7:0] = I;
                1'b1: IROut[15:8] = I;
            endcase
        end
    end
endmodule