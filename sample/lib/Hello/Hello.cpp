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
#include "llvm/IR/InstrTypes.h"

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
		errs() << "Hello: " ;
		errs().write_escaped(F.getName())<< "arguments are: ";
		Function *func = &F;
		//CallBase value;
		for (Function::arg_iterator ArgIdx = func->arg_begin(), ArgEnd= func->arg_end(); ArgIdx !=ArgEnd; ++ArgIdx)
			errs().write_escaped(func->getArgumentList())<<"\n";
		for (Function::iterator blk =func->begin(), blk_e=func->end(); blk!=blk_e; ++blk){
			errs() << "Basic block (name=" << blk->getName() << ") has " <<blk->size() << "instructions which are:\n";
		for(BasicBlock::iterator i=blk->begin(), e=blk->end(); i!=e; ++i)
			errs()<< *i << "\n";

		}
		return false;
	}

};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);
