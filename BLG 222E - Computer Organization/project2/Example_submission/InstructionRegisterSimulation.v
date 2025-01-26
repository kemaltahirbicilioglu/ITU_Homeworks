`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU Computer Engineering Department
// Engineer: Kadir Ozlem
// Project Name: BLG222E Project 1 Simulation
//////////////////////////////////////////////////////////////////////////////////

module InstructionRegisterSimulation();
    reg[7:0] I;
    reg Write, LH;
    wire[15:0] IROut;
    integer test_no;
    
    CrystalOscillator clk();
    InstructionRegister IR(.I(I), .Write(Write), .LH(LH), 
                            .Clock(clk.clock), .IROut(IROut));
        
    FileOperation F();
    
    
    initial begin
        F.SimulationName ="InstructionRegister";
        F.InitializeSimulation(0);
        clk.clock = 0;
        
        //Test 1
        test_no = 1;
        IR.IROut = 16'h2367;
        LH = 0;
        Write = 1;
        I = 8'h15; 
        #5;
        clk.Clock();
        F.CheckValues(IROut,16'h2315, test_no, "IROut");
        
        //Test 2
        test_no = 2;
        IR.IROut = 16'h2367;
        LH = 1;
        Write = 1;
        I = 8'h15; 
        #5;
        clk.Clock();
        F.CheckValues(IROut,16'h1567, test_no, "IROut");
        
        F.FinishSimulation();
    end
endmodule