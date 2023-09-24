#ifndef DATAPATH
#define DATAPATH
#include <iostream>
#include "data.h"
class DataPath : public Data
{
	public:
		//Set
		DataPath()
		{
			std::cout<<"error in DataPath"<<std::endl;
			exit(1);
		}
		DataPath(Data* _IF,Data* _ID,Data* _EX,Data* _MEM,Data* _WB)
		:IF(_IF),ID(_ID),EX(_EX),MEM(_MEM),WB(_WB),Data()
		{;}
		void Init(void){
			Update();
		}
		void Update(void){ //wire
			SetPC(IF->GetPC());
			SetBranchTarget(WB->GetBranchTarget());
			SetOP(IF->GetOP());
			SetConst(EX->GetConst());
			SetRs1Data(EX->GetRs1Data());
			SetRs2Data(EX->GetRs2Data());
			SetRd(WB->GetRd());
			SetWriteBackData(WB->GetWriteBackData());
			SetALUresult(MEM->GetALUresult());
		}	
		void Stall(bool jump){
			if (jump){
				IF->CleanData();
				ID->CleanData();
				EX->CleanData();
			}
		}
		uint32_t GetOP_A() const{return MEM->GetOP();}
		uint32_t GetOP_B() const{return WB->GetOP();}
		uint32_t GetForwardA() const{ //Rtype
			return MEM->GetALUresult();
		}
		uint32_t GetForwardB() const{
			return WB->GetWriteBackData(); //load Rtype
		}
		//Get is set in Data class
	private:
		Data* IF;
		Data* ID;
		Data* EX;
		Data* MEM;
		Data* WB;	
};
#
#endif
