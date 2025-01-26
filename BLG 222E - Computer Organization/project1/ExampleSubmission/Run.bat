call D:\Xilinx\Vivado\2017.4\settings64.bat

set folder=Simulation_Files
mkdir %folder%
copy "RAM.mem" "%folder%/RAM.mem"
cd "%folder%

::Register Simulation
call xvlog ../Register.v  
call xvlog ../RegisterSimulation.v
call xvlog ../Helper.v
call xelab -top RegisterSimulation -snapshot regsim -debug typical
call xsim regsim -R

::Register File Simulation
call xvlog ../Register.v  
call xvlog ../RegisterFile.v  
call xvlog ../RegisterFileSimulation.v
call xvlog ../Helper.v
call xelab -top RegisterFileSimulation -snapshot regfilesim -debug typical
call xsim regfilesim -R

::Address Register File Simulation
call xvlog ../Register.v  
call xvlog ../AddressRegisterFile.v  
call xvlog ../AddressRegisterFileSimulation.v
call xvlog ../Helper.v
call xelab -top AddressRegisterFileSimulation -snapshot addregfilesim -debug typical
call xsim addregfilesim -R

::Instruction Register Simulation
call xvlog ../InstructionRegister.v  
call xvlog ../InstructionRegisterSimulation.v
call xvlog ../Helper.v
call xelab -top InstructionRegisterSimulation -snapshot insregsim -debug typical
call xsim insregsim -R

::Arithmetic Logic Unit Simulation
call xvlog ../ArithmeticLogicUnit.v  
call xvlog ../ArithmeticLogicUnitSimulation.v
call xvlog ../Helper.v
call xelab -top ArithmeticLogicUnitSimulation -snapshot alusim -debug typical
call xsim alusim -R

::Arithmetic Logic Unit System Simulation
call xvlog ../Register.v  
call xvlog ../RegisterFile.v
call xvlog ../AddressRegisterFile.v  
call xvlog ../InstructionRegister.v  
call xvlog ../ArithmeticLogicUnit.v
call xvlog ../Memory.v  
call xvlog ../ArithmeticLogicUnitSystem.v  
call xvlog ../ArithmeticLogicUnitSystemSimulation.v
call xvlog ../Helper.v

call xelab -top ArithmeticLogicUnitSystemSimulation -snapshot alusyssim -debug typical
call xsim alusyssim -R

cd ..