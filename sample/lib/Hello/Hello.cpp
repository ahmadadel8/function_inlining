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
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Type.h"

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
					Function *callerFunc = &F;
					Function *calleeFunc;
					CallInst* callInst;
					bool areArgsConst;
					ConstantInt * constArg;
					Value* V;
					unsigned numArgs;
					std::vector<Value*> actualArgVector;

					//CallBase value;

					for (inst_iterator I = inst_begin(callerFunc), E=inst_end(callerFunc); I!=E; ++I)
					{
						callInst = dyn_cast<CallInst>(&*I);
						if (callInst){
							areArgsConst= true;
							numArgs=callInst->getNumArgOperands();
							for (unsigned ArgIdx=0; ArgIdx<numArgs; ++ArgIdx){
								V=callInst->getArgOperand(ArgIdx);
								if(!isa<Constant>(V)) areArgsConst= false;
								else actualArgVector.push_back(V);
								}
								//constArg=ConstantInt::get(IntegerType::get(V->getContext(),32), (uint64_t)*V);
								if (areArgsConst){
									calleeFunc=callInst->getCalledFunction();
									inst_iterator callee_I = inst_begin(calleeFunc);
									if(&*callee_I){  //temporary fix. Might want to revisit. Ensures printf and scanf..etc aren't inlined
										unsigned Idx=0;
										for (Function::arg_iterator ArgPtr = calleeFunc->arg_begin(), ArgEnd= calleeFunc->arg_end(); ArgPtr !=ArgEnd; ++ArgPtr){
											constArg = dyn_cast<ConstantInt>(actualArgVector[Idx++]);
											ArgPtr->replaceAllUsesWith(constArg);
											}
										actualArgVector.clear();

										//auto *ai = new AllocaInst(Type::getInt32Ty(LLVMContext &C)));
										//auto *dummy_Inst = new Instruction(Type::getInt32Ty(), 0, NULL, 0, *I);
										ValueToValueMapTy vmap;
										for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I)
											{ ReturnInst *ri = dyn_cast<ReturnInst>(&*callee_I);
												if (ri) break;
												Instruction* new_Inst = callee_I->clone();
												new_Inst->insertBefore(&*I);
												vmap[&*callee_I] = new_Inst;
												llvm::RemapInstruction(new_Inst, vmap,
								                           RF_NoModuleLevelChanges);
											}
										*I->eraseFromParent();
										errs()<<"Current Instruction:"<< *I<<"\n";
										// for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I){
										//   Instruction* temp = callee_I->clone();
										//
										//   temp->insertBefore(&*I);
										//
										//   Value* copyInstVal = temp;
										//   Value* originalInstVal = &(*callee_I);
										//
										//   copyInstVal->setName(originalInstVal->getName());
										// }



										// for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I)
										// 		{errs()<<"Current Instruction:"<< *callee_I <<", End :" << *callee_E <<"\n";
										// 			&*I->getParent()->getInstList().insert(&*I,&*callee_I);
										// 		}
										// I->eraseFromParent();
									}
							}
						}
					}
					return false;
				}

};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);
