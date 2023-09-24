#ifndef CONTROL
#define CONTROL
#include <iostream>
#include "data.h"
class ControlUnit : public ControlData
{
	public:
		//Set
		ControlUnit()
		{
			std::cout<<"error in ControlUnit"<<std::endl;
			exit(1);
		}
		ControlUnit(ControlData* _IF,ControlData* _ID,ControlData* _EX,ControlData* _MEM,ControlData* _WB)
		:IF(_IF),ID(_ID),EX(_EX),MEM(_MEM),WB(_WB),ControlData()
		{;}
		void Init(void){
			Update();
		}
		void Update(void){ //wire
			SetPCSrc(WB->GetPCSrc());
			SetRegWrite(WB->GetRegWrite());
			SetALUSrc(EX->GetALUSrc()); 
			SetALUop(EX->GetALUop()[1],EX->GetALUop()[0]);
			SetBranch(MEM->GetBranch());
			SetMemWrite(MEM->GetMemWrite());
			SetMemRead(MEM->GetMemRead());
			SetMemToReg(MEM->GetMemToReg());
			SetEnd(WB->GetEnd());
		}	
		//Get is set in Control class
		void Stall(bool jump){
			if (jump){
				IF->CleanControlData();
				ID->CleanControlData();
				EX->CleanControlData();
			}
		}
		bool testForwardA(uint32_t reg_num,uint32_t rd_num) const{
			return (reg_num == rd_num) && MEM->GetRegWrite() && (rd_num != 0);
		}
		bool testForwardB(uint32_t reg_num,uint32_t rd_num) const{
			return (reg_num == rd_num) && WB->GetRegWrite() && (rd_num != 0);
		}
	private:
		ControlData* IF;
		ControlData* ID;
		ControlData* EX;
		ControlData* MEM;
		ControlData* WB;	
};
#endif
