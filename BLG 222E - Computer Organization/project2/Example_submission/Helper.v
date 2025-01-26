`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU Computer Engineering Department
// Engineer: Kadir Ozlem
// Project Name: BLG222E Project 1 Simulation
//////////////////////////////////////////////////////////////////////////////////

`define EVALUATION_FILE 1
`define DEBUG_FILE 1
`define DEBUG_PRINT 1
`define E_FILE "./evaluation.csv"
`define D_FILE "./debug.txt"


module FileOperation();
    integer df, ef;
    integer fails = 0;
    integer success = 0; 
    reg [64*8-1:0] SimulationName;
    task  InitializeSimulation;
            input FirstTest;
            begin
                if (`DEBUG_FILE) begin
                    if (FirstTest) begin
                        df = $fopen(`D_FILE,"w");
                    end
                    else begin
                        df = $fopen(`D_FILE,"a+");
                    end
                    $fwrite(df,"------------------------------------\n");
                    $fwrite(df,"%0s Simulation Started\n", SimulationName);
                end
                if (`EVALUATION_FILE) begin
                    if (FirstTest) begin
                        ef = $fopen(`E_FILE,"w");
                        $fwrite(ef,"module_name;testno,test_name;passed;actual_value;expected_value\n");
                    end
                    else begin
                        ef = $fopen(`E_FILE,"a+");
                    end
                end
                if (`DEBUG_PRINT) begin
                    $display("------------------------------------");
                    $display("%0s Simulation Started",SimulationName);
                end
            end
        endtask
        
        task FinishSimulation;
            begin
                if (`DEBUG_FILE) begin
                    $fwrite(df,"%0s Simulation Finished\n",SimulationName);
                    $fwrite(df,"------------------------------------\n");
                    $fwrite(df,"\n");
                    $fwrite(df,"\n");
                    $fwrite(df,"\n");
                    $fclose(df);
                end
                if (`EVALUATION_FILE) begin
                    $fclose(ef);
                end
                if (`DEBUG_PRINT) begin
                    $display("%0s Simulation Finished",SimulationName);
                    $display("%0d Test Failed",fails);
                    $display("%0d Test Passed",success);
                    $display("------------------------------------");
                end
            end
        endtask
        
        
        task CheckValues;
            input [15:0] actual, expected, testno;
            input [32*8-1:0] name;
            begin
                if (actual === expected) begin
                    success = success + 1; 
                    if (`DEBUG_FILE) begin
                        $fwrite(df,"[PASS] Test No: %0d, Component: %0s, Actual Value: 0x%h, Expected Value: 0x%h\n", testno, name, actual, expected); 
                    end
                    if (`EVALUATION_FILE) begin
                        $fwrite(ef,"%0s;%0d;%0s;1;0x%h;0x%h\n", SimulationName, testno, name, actual, expected); 
                    end
                    if (`DEBUG_PRINT) begin
                        $display("[PASS] Test No: %0d, Component: %0s, Actual Value: 0x%h, Expected Value: 0x%h", testno, name, actual, expected); 
                    end
                end
                else begin
                    fails = fails+1;
                    if (`DEBUG_FILE) begin
                        $fwrite(df,"[FAIL] Test No: %0d, Component: %0s, Actual Value: 0x%h, Expected Value: 0x%h\n", testno, name, actual, expected); 
                    end
                    if (`EVALUATION_FILE) begin
                        $fwrite(ef,"%0s;%0d;%0s;0;0x%h;0x%h\n", SimulationName, testno, name, actual, expected);
                    end
                    if (`DEBUG_PRINT) begin
                        $display("[FAIL] Test No: %0d, Component: %0s, Actual Value: 0x%h, Expected Value: 0x%h", testno, name, actual, expected); 
                    end
                end
            end
        endtask
endmodule

module CrystalOscillator;
    reg clock;
    
    task Clock;
        begin
            clock = 0; #20; clock=1; #20;
        end
    endtask

endmodule
    
