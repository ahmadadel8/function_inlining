#define DEBUG_TYPE "func_inline"
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
struct Function_Inlining :  public FunctionPass
{
        /** Constructor. */
	static char ID;
	Function_Inlining() : FunctionPass(ID) {}

        virtual bool runOnFunction(llvm::Function &F){
				  Function *callerFunc = &F;
				  Function *calleeFunc;
				  CallInst* callInst;
				  Instruction* calleeInst;
				  StoreInst* strInst;
					LoadInst* ldInst;
					ReturnInst* retInst;
				  bool areArgsConst;
				  ConstantInt* constArg;
					std::vector<Value*> actualArgVector;
				  Value* actualArg;
					Value* ret;
					Value* retVal;
					Value* retPtr;
				  unsigned numArgs;


					//CallBase value;

					for (inst_iterator I = inst_begin(callerFunc), E=inst_end(callerFunc); I!=E; ++I)
					{
						callInst = dyn_cast<CallInst>(&*I);
						if (callInst){
							areArgsConst= true;
							numArgs=callInst->getNumArgOperands();
							for (unsigned ArgIdx=0; ArgIdx<numArgs; ++ArgIdx){
								actualArg=callInst->getArgOperand(ArgIdx);
								if(!isa<Constant>(actualArg)) areArgsConst= false;
								else actualArgVector.push_back(actualArg);
								}
								if (areArgsConst){
									calleeFunc=callInst->getCalledFunction();
									inst_iterator callee_I = inst_begin(calleeFunc);
									if(&*callee_I){  //temporary fix. Might want to reisit. Ensures printf and scanf..etc aren't inlined
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
											{ retInst = dyn_cast<ReturnInst>(&*callee_I);
												if (retInst)
												{ret=retInst->getReturnValue();
													if(!(ret)) {
														I++->eraseFromParent();
														break;}}
												calleeInst = callee_I->clone();
												calleeInst->insertBefore(&*I);
										    //&*I->getParent()->getInstList().insert(&*I,&*calleeInst);
												vmap[&*callee_I] = calleeInst;
												RemapInstruction(calleeInst, vmap, RF_NoModuleLevelChanges);
												inst_iterator dummyItrator=callee_I;
												strInst=dyn_cast<StoreInst>(calleeInst);
												ldInst=dyn_cast<LoadInst>(&*(++dummyItrator));
												retInst = dyn_cast<ReturnInst>(&*(++dummyItrator));
												if(strInst)
												 	if (ldInst)
														if(retInst){
															ret=retInst->getReturnValue();
															if(ret){
																	I++->eraseFromParent();
																	StoreInst* caller_stInst=dyn_cast<StoreInst>(&*I);
																	if(caller_stInst){
																		retPtr=strInst->getValueOperand();
																		retVal=caller_stInst->getPointerOperand();
																		errs()<<*retVal <<*ret <<*retPtr;
																		StoreInst *str= new StoreInst(retPtr,retVal,  &*I);
																		I++->eraseFromParent();
																		calleeInst->eraseFromParent();
																		break;
												}
											}
										}
									}
								}
							}
						}
					}
					return true;
				}

};
}

char Function_Inlining::ID = 1;
static RegisterPass<Function_Inlining> X("func_inline", "Function Inlining Pass", false, false);
