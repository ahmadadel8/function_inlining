//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "hello"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"

using namespace llvm;



namespace {
struct Hello :  public FunctionPass
{

        /** Constructor. */
	static char ID;
	Hello() : FunctionPass(ID) {}

        //DEFINE_INTPASS_ANALYSIS_ADJUSTMENT(PointerAnalysisPass);

        /**
         * @brief Runs this pass on the given function.
         * @param [in,out] func The function to analyze
         * @return true if the function was modified; false otherwise
        */
        virtual bool runOnFunction(llvm::Function &F){
		errs() <<"In function ";
		errs().write_escaped(F.getName())<<  "\n";
		Function *func = &F;
		Function *calledFunc;
		CallInst* callInst;
		Value* V;
		bool areArgsConst;
		//CallBase value;

		for (inst_iterator I = inst_begin(func), E=inst_end(func); I!=E; ++I)
			if (callInst = dyn_cast<CallInst>(&*I))
			{areArgsConst= True;
				for (unsigned ArgIdx=0; ArgIdx<callInst->getNumArgOperands(); ++ArgIdx){
				V=callInst->getArgOperand(ArgIdx);
				if(!isa<Constant>(V))
					areArgsConst= False;
				}
				if (areArgsConst)
					{
					calledFunc=callInst->getCalledFunction();
					errs()<< "Function ";
					errs().write_escaped(calledFunc->getName())<<  " is called with actual arguments ";
					errs()<< *V << "\n";
				}
			}
		return false;
	}

};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);
