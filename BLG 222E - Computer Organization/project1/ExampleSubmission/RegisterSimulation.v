`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU Computer Engineering Department
// Engineer: Kadir Ozlem
// Project Name: BLG222E Project 1 Simulation
//////////////////////////////////////////////////////////////////////////////////

module RegisterSimulation();
    reg[15:0] I;
    reg E;
    reg[2:0] FunSel;
    wire[15:0] Q;
    integer test_no;
    
    CrystalOscillator clk();
    Register R(.I(I), .E(E), .FunSel(FunSel), .Clock(clk.clock), .Q(Q));
        
    FileOperation F();
    
    initial begin
        F.SimulationName ="Register";
        F.InitializeSimulation(1);
        clk.clock = 0;
        
        //Test 1
        test_no = 1; 
        R.Q=16'h0025; FunSel=3'b000; I=16'h0072;  E=0; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0025, test_no, "Q");
        
        //Test 2 
        test_no = 2;
        R.Q=16'h0025; FunSel=3'b000; E=1; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0024, test_no, "Q"); 
        
        //Test 3 
        test_no = 3;
        R.Q=16'h0025; FunSel=3'b001; E=0; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0025, test_no, "Q");
        
        //Test 4 
        test_no = 4;
        R.Q=16'h0025; FunSel=3'b001; E=1; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0026, test_no, "Q"); 
        
        //Test 5
        test_no = 5; 
        R.Q=16'h0025; I = 16'h0012; FunSel=3'b010; E=0; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0025, test_no, "Q");
        
        //Test 6 
        test_no = 6;
        R.Q=16'h0025; I = 16'h0012; FunSel=3'b010; E=1; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0012, test_no, "Q");

        //Test 7 
        test_no = 7;
        R.Q=16'h0025; FunSel=3'b011; E=0; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h0025, test_no, "Q");
        
        //Test 8
        test_no = 8;
        R.Q=16'h0025; FunSel=3'b011; E=1; #5;
        clk.Clock();
        F.CheckValues(R.Q,16'h000, test_no, "Q");   
        
        
        F.FinishSimulation();
    end
endmodule