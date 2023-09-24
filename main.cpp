#include <iostream>
#include <vector>
#include "stage.h"
#include "control.h"
#include "datapath.h"
//you shoud turn on debug mode here (ex. #define D_REG)

#if defined(D_REG) || defined(D_MEM) || defined(D_INST)
 #define DEBUG
#endif
int main(int argc,char** argv){	
	//initialize
	InstructionFetch IF(argv[1]);
	InstructionDecode ID;
	Execute EX;
	Memory MEM;
	WriteBack WB;	

	ControlUnit Control(&IF,&ID,&EX,&MEM,&WB);
	Control.Init();

	DataPath Datapath(&IF,&ID,&EX,&MEM,&WB); //for passing data among stages	
	Datapath.Init();

	//process
	uint32_t PC = 0;
	uint32_t clock = 0;
	while (!Control.GetEnd())
	{
		//passing data to next stage
		WB.PassControlData(&MEM),WB.PassData(&MEM);
		MEM.PassControlData(&EX),MEM.PassData(&EX);
		EX.PassControlData(&ID),EX.PassData(&ID);
		ID.PassControlData(&IF),ID.PassData(&IF);
		Control.Update();
		Datapath.Update();
		//excute stage
		PC = IF.Operate(PC,Control,Datapath);
		ID.Operate(Control,Datapath);
		EX.Operate(Control,Datapath);
		bool Jump = MEM.Operate(Control,Datapath);
		WB.Operate(Control,Datapath);
		//stall 
		Control.Stall(Jump);
		Datapath.Stall(Jump);
		clock++;
	}	
	#ifdef D_REG
	ID.Check();
	#endif
	#ifdef D_INST
	IF.Check();
	#endif
	#ifdef D_MEM
	MEM.Check();
	#endif
	#ifndef DEBUG
	std::cout<<"execution completed."<<std::endl;
	std::cout<<"Filename: "<<argv[1]<<std::endl;
	std::cout<<"CPU_clock_times: "<<clock<<std::endl;
	std::cout<<"Result: "; 
	ID.Check(argv[2]);
	#endif
	return 0;
}
