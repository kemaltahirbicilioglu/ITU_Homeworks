`timescale 1ns / 1ps

module RegisterFile(
    input wire [15:0] I,
    input wire [2:0] OutASel,
    input wire [2:0] OutBSel,
    input wire [2:0] FunSel,
    input wire [3:0] RegSel,
    input wire [3:0] ScrSel,
    input wire Clock,
    output reg [15:0] OutA,
    output reg [15:0] OutB
);

    wire [15:0] R1_Q, R2_Q, R3_Q, R4_Q;
    wire [15:0] S1_Q, S2_Q, S3_Q, S4_Q;

    Register R1(.I(I), .E(~RegSel[3]), .FunSel(FunSel), .Clock(Clock), .Q(R1_Q));
    Register R2(.I(I), .E(~RegSel[2]), .FunSel(FunSel), .Clock(Clock), .Q(R2_Q));
    Register R3(.I(I), .E(~RegSel[1]), .FunSel(FunSel), .Clock(Clock), .Q(R3_Q));
    Register R4(.I(I), .E(~RegSel[0]), .FunSel(FunSel), .Clock(Clock), .Q(R4_Q));
    
    Register S1(.I(I), .E(~ScrSel[3]), .FunSel(FunSel), .Clock(Clock), .Q(S1_Q));
    Register S2(.I(I), .E(~ScrSel[2]), .FunSel(FunSel), .Clock(Clock), .Q(S2_Q));
    Register S3(.I(I), .E(~ScrSel[1]), .FunSel(FunSel), .Clock(Clock), .Q(S3_Q));
    Register S4(.I(I), .E(~ScrSel[0]), .FunSel(FunSel), .Clock(Clock), .Q(S4_Q));

    always @* begin // output does not depend on Clock rising edge.
        case (OutASel)
            3'b000: OutA = R1_Q;
            3'b001: OutA = R2_Q;
            3'b010: OutA = R3_Q;
            3'b011: OutA = R4_Q;
            3'b100: OutA = S1_Q;
            3'b101: OutA = S2_Q;
            3'b110: OutA = S3_Q;
            3'b111: OutA = S4_Q;
        endcase

        case (OutBSel)
            3'b000: OutB = R1_Q;
            3'b001: OutB = R2_Q;
            3'b010: OutB = R3_Q;
            3'b011: OutB = R4_Q;
            3'b100: OutB = S1_Q;
            3'b101: OutB = S2_Q;
            3'b110: OutB = S3_Q;
            3'b111: OutB = S4_Q;
        endcase
    end

endmodule
