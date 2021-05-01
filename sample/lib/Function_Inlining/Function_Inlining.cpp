#define DEBUG_TYPE "inline"
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

        //DEFINE_INTPASS_ANALYSIS_ADJUSTMENT(PointerAnalysisPass);
				/*
         * @brief Runs this pass on the given function.
         * @param [in,out] func The function to analyze
         * @return true if the function was modified; false otherwise
        */

        virtual bool runOnFunction(llvm::Function &F){
				  Function *callerFunc = &F;
				  Function *calleeFunc;
				  CallInst* callInst;
				  Instruction* new_Inst;
				  StoreInst* strInst;
					LoadInst* ldInst;
					ReturnInst* ri;
				  bool areArgsConst;
				  bool isNotVoid=false;
				  ConstantInt * constArg;
				  Value* V;
					Value* ret;
					Value* retVal;
					Value* retPtr;
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
											{ ri = dyn_cast<ReturnInst>(&*callee_I);
												if (ri)
												{ret=ri->getReturnValue();
													if(!(ret)) {
														I++->eraseFromParent();
														break;}}
												new_Inst = callee_I->clone();
												new_Inst->insertBefore(&*I);
										    //&*I->getParent()->getInstList().insert(&*I,&*new_Inst);
												vmap[&*callee_I] = new_Inst;
												RemapInstruction(new_Inst, vmap, RF_NoModuleLevelChanges);
												inst_iterator dummyItrator=callee_I;
												strInst=dyn_cast<StoreInst>(new_Inst);
												ldInst=dyn_cast<LoadInst>(&*(++dummyItrator));
												ri = dyn_cast<ReturnInst>(&*(++dummyItrator));
												if(strInst)
												 	if (ldInst)
														if(ri){
															ret=ri->getReturnValue();
															if(ret){
																	I++->eraseFromParent();
																	StoreInst* caller_stInst=dyn_cast<StoreInst>(&*I);
																	if(caller_stInst){
																		retPtr=strInst->getValueOperand();
																		retVal=caller_stInst->getPointerOperand();
																		StoreInst *str= new StoreInst(retPtr,retVal,  &*I);
																		I++->eraseFromParent();
																		new_Inst->eraseFromParent();
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

char Function_Inlining::ID = 0;
static RegisterPass<Function_Inlining> X("inline", "Function Inlining Pass", false, false);
