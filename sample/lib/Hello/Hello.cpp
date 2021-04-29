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
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "vector"

using namespace llvm;



namespace {
struct Hello :  public FunctionPass
{
        /** Constructor. */
	static char ID;
	Hello() : FunctionPass(ID) {}

        //DEFINE_INTPASS_ANALYSIS_ADJUSTMENT(PointerAnalysisPass);
				/*
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
					bool areArgsConst;
					//SmallVector<ConstantInt*,10> constArgVector=new ConstantInt[10];
					ConstantInt * constArg;
					Value* V;
					unsigned numArgs;
					//CallBase value;

					for (inst_iterator I = inst_begin(func), E=inst_end(func); I!=E; ++I)
					{
						callInst = dyn_cast<CallInst>(&*I);
						if (callInst){
							std::vector<Value> actualArgVector;
							areArgsConst= true;
							numArgs=callInst->getNumArgOperands();
							for (unsigned ArgIdx=0; ArgIdx<numArgs; ++ArgIdx){
								V=callInst->getArgOperand(ArgIdx);
								if(!isa<Constant>(V)) areArgsConst= false;
								else actualArgVector.push_back(V);
								}
								//constArg=ConstantInt::get(IntegerType::get(V->getContext(),32), (uint64_t)*V);
								if (areArgsConst){
									calledFunc=callInst->getCalledFunction();
									unsigned Idx=0;
									for (Function::arg_iterator ArgPtr = calledFunc->arg_begin(), ArgEnd= calledFunc->arg_end(); ArgPtr !=ArgEnd; ++ArgPtr){
										constArg = dyn_cast<ConstantInt>(actualArgVector[Idx++];
										ArgPtr->replaceAllUsesWith(constArg);
										}
								/*errs()<< "Function ";
								errs().write_escaped(calledFunc->getName())<<  " is called with actual arguments ";
								for (unsigned ArgIdx=0; ArgIdx<callInst->getNumArgOperands(); ++ArgIdx)
									errs()<< *callInst->getArgOperand(ArgIdx) <<",";
								errs() << "\n";*/

								}
						}
					}
					return false;
				}

};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);
