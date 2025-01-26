`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU Computer Engineering Department
// Engineer: Kadir Ozlem
// Project Name: BLG222E Project 1 Simulation
//////////////////////////////////////////////////////////////////////////////////

module AddressRegisterFileSimulation();
    reg[15:0] I;
    reg [1:0] OutCSel, OutDSel;
    reg [2:0] RegSel, FunSel;
    wire[15:0] OutC, OutD;
    integer test_no;
    
    CrystalOscillator clk();
    AddressRegisterFile ARF(.I(I), .OutCSel(OutCSel), .OutDSel(OutDSel), 
                    .FunSel(FunSel), .RegSel(RegSel), .Clock(clk.clock), 
                    .OutC(OutC), .OutD(OutD));
        
    FileOperation F();
    
    task ClearRegisters;
    begin
        ARF.PC.Q = 16'h0;
        ARF.AR.Q = 16'h0;
        ARF.SP.Q = 16'h0;
    end
    endtask
    
    task SetRegisters;
        input [15:0] value;
        begin
            ARF.PC.Q = value;
            ARF.AR.Q = value;
            ARF.SP.Q = value;
        end
    endtask
    
    task DisableAll;
        begin
            RegSel = 3'b111;
        end
    endtask
    
    initial begin
        F.SimulationName ="AddressRegisterFile";
        F.InitializeSimulation(0);
        clk.clock = 0;
        
        //Test 1
        test_no = 1;
        DisableAll();
        ClearRegisters();
        ARF.PC.Q = 16'h1234;
        ARF.SP.Q = 16'h5678;
        OutCSel = 2'b00;
        OutDSel = 2'b11;
        #5;
        
        F.CheckValues(ARF.OutC,16'h1234, test_no, "OutC");
        F.CheckValues(ARF.OutD,16'h5678, test_no, "OutD");
        
        //Test 2
        test_no = 2;
        DisableAll();
        SetRegisters(16'h1234);
        RegSel = 4'b010;
        FunSel = 3'b010;
        I = 16'h3548;
        OutCSel  = 2'b10;
        OutDSel  = 2'b01;
        clk.Clock();

        F.CheckValues(ARF.OutC,16'h1234, test_no, "OutA");
        F.CheckValues(ARF.OutD,16'h3548, test_no, "OutB");
        
         
        F.FinishSimulation();
    end
endmodule
