`timescale 1ns / 1ps

module AddressRegisterFile(
    input wire [15:0] I,
    input wire [1:0] OutCSel,
    input wire [1:0] OutDSel,
    input wire [2:0] FunSel,
    input wire [2:0] RegSel,
    input wire Clock,
    output reg [15:0] OutC,
    output reg [15:0] OutD
);

    wire [15:0] PC_Q, AR_Q, SP_Q;

    Register PC(.I(I), .E(~RegSel[2]), .FunSel(FunSel), .Clock(Clock), .Q(PC_Q));
    Register AR(.I(I), .E(~RegSel[1]), .FunSel(FunSel), .Clock(Clock), .Q(AR_Q));
    Register SP(.I(I), .E(~RegSel[0]), .FunSel(FunSel), .Clock(Clock), .Q(SP_Q));

    always @* begin // output does not depend on Clock rising edge.
        case (OutCSel)
            2'b00: OutC = PC_Q;
            2'b01: OutC = PC_Q;
            2'b10: OutC = AR_Q;
            2'b11: OutC = SP_Q;
        endcase

        case (OutDSel)
            2'b00: OutD = PC_Q;
            2'b01: OutD = PC_Q;
            2'b10: OutD = AR_Q;
            2'b11: OutD = SP_Q;
        endcase
    end

endmodule
