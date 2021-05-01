///////////////////////////////////////////
//// FUNCTION INLINING PASS FOR LLVM//////
/////////////////////////////////////////


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
					//Function type variables
				  Function *callerFunc = &F; //A pointer to the caller function. More usefull that the reference F.
				  Function *calleeFunc;
					//Intruction Type Variables
				  CallInst* callInst;
				  StoreInst* strInst;
					LoadInst* ldInst;
					ReturnInst* retInst;
					Instruction* calleeInst;
					//Miscellaneous
				  Value* actualArg; //A value type variable that iteratively holds the actual arguments
					std::vector<ConstantInt*> actualArgVector; //A vector that holds the arguments if they are constants.
					Value* strVal; //The value in the store instruction replacing the call instruction.
					Value* strPtr; //The return in the store instruction replacing the call instruction.
					ConstantInt* constArg; //A ConstInt type variable that holds constant arguments after casting from Value to ConstInt
				  unsigned numArgs; //number of arguments in a call instruction
					bool areArgsConst; //a flag. True if all arguments of a call instruction are constants.


//First, we need to iterate over all the instructions in the code, until we find a call instruction

					for (inst_iterator I = inst_begin(callerFunc), E=inst_end(callerFunc); I!=E; ++I)
					{
						//tries to cast every instruction to callInst class. Returns NULL if not a callInst
						if (callInst = dyn_cast<CallInst>(&*I)){ //if callInst is not NULL, i.e. intruction is indeed a call instruction
							//Now that we found the call instruction, we need to check if all the argments are indeed constants.
							areArgsConst= true; //initialize the flag to be true. It will be set to false if we encounter a non const argument
							numArgs=callInst->getNumArgOperands(); //the number of the arguments of the all instruction so we can iterate over them
							for (unsigned ArgIdx=0; ArgIdx<numArgs; ++ArgIdx){
								actualArg=callInst->getArgOperand(ArgIdx); //checks one argument at a time

								if(constArg=dyn_cast<ConstantInt>(actualArg)) 	actualArgVector.push_back(constArg);
								else
								{ //tries to cast
									areArgsConst= false;
									break;}

								// if(!isa<Constant>(actualArg)) { //tries to cast
								// 	areArgsConst= false;
								// 	break;}
								// else actualArgVector.push_back(actualArg);
								}
								if (areArgsConst){
									calleeFunc=callInst->getCalledFunction();
									inst_iterator callee_I = inst_begin(calleeFunc);
									if(&*callee_I){  //temporary fix. Might want to reisit. Ensures printf and scanf..etc aren't inlined
										unsigned Idx=0;
										for (Function::arg_iterator ArgPtr = calleeFunc->arg_begin(), ArgEnd= calleeFunc->arg_end(); ArgPtr !=ArgEnd; ++ArgPtr){
											constArg = cast<ConstantInt>(actualArgVector[Idx++]);
											ArgPtr->replaceAllUsesWith(actualArgVector[Idx++]);
											}
										actualArgVector.clear();

										//auto *ai = new AllocaInst(Type::getInt32Ty(LLVMContext &C)));
										//auto *dummy_Inst = new Instruction(Type::getInt32Ty(), 0, NULL, 0, *I);
										 ValueToValueMapTy vmap;
										for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I)
											{	if (retInst = dyn_cast<ReturnInst>(&*callee_I))
												{	if(retInst->getNumOperands()==0) {
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
														if(retInst)
															if(retInst->getNumOperands()!=0){
																	I++->eraseFromParent();
																	StoreInst* caller_stInst=dyn_cast<StoreInst>(&*I);
																	if(caller_stInst){
																		strVal=strInst->getValueOperand();
																		strPtr=caller_stInst->getPointerOperand();
																		StoreInst *str= new StoreInst(strVal, strPtr, &*I);
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
					return true;
				}

};
}

char Function_Inlining::ID = 1;
static RegisterPass<Function_Inlining> X("func_inline", "Function Inlining Pass", false, false);
