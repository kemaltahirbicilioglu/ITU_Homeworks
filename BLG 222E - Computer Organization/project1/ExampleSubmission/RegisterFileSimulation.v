`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU Computer Engineering Department
// Engineer: Kadir Ozlem
// Project Name: BLG222E Project 1 Simulation
//////////////////////////////////////////////////////////////////////////////////

module RegisterFileSimulation();
    reg[15:0] I;
    reg [2:0] OutASel, OutBSel, FunSel;
    reg [3:0] RegSel, ScrSel;
    wire[15:0] OutA, OutB;
    integer test_no;
    
    CrystalOscillator clk();
    RegisterFile RF(.I(I), .OutASel(OutASel), .OutBSel(OutBSel), 
                    .FunSel(FunSel), .RegSel(RegSel), .ScrSel(ScrSel), 
                    .Clock(clk.clock), .OutA(OutA), .OutB(OutB));
        
    FileOperation F();
    
    task ClearRegisters;
    begin
        RF.R1.Q = 16'h0;
        RF.R2.Q = 16'h0;
        RF.R3.Q = 16'h0;
        RF.R4.Q = 16'h0;
        RF.S1.Q = 16'h0;
        RF.S2.Q = 16'h0;
        RF.S3.Q = 16'h0;
        RF.S4.Q = 16'h0;
    end
    endtask
    
    task SetRegisters;
        input [15:0] value;
        begin
            RF.R1.Q = value;
            RF.R2.Q = value;
            RF.R3.Q = value;
            RF.R4.Q = value;
            RF.S1.Q = value;
            RF.S2.Q = value;
            RF.S3.Q = value;
            RF.S4.Q = value;
        end
    endtask
    
    task DisableAll;
        begin
            RegSel = 4'b1111;
            ScrSel = 4'b1111;
        end
    endtask
    
    initial begin
        F.SimulationName ="RegisterFile";
        F.InitializeSimulation(0);
        clk.clock = 0;
        
        //Test 1
        test_no = 1;
        DisableAll();
        ClearRegisters();
        RF.R1.Q = 16'h1234;
        RF.R2.Q = 16'h5678;
        OutASel = 3'b000;
        OutBSel = 3'b001;
        #5;
        
        F.CheckValues(RF.OutA,16'h1234, test_no, "OutA");
        F.CheckValues(RF.OutB,16'h5678, test_no, "OutB");
        
        //Test 2
        test_no = 2;
        DisableAll();
        SetRegisters(16'h1234);
        RegSel = 4'b0101;
        ScrSel = 4'b1010;
        FunSel = 3'b010;
        I = 16'h3548;
        OutASel  = 3'b001;
        OutBSel  = 3'b101;
        clk.Clock();

        F.CheckValues(RF.OutA,16'h1234, test_no, "OutA");
        F.CheckValues(RF.OutB,16'h3548, test_no, "OutB");
        
         
        F.FinishSimulation();
    end
endmodule