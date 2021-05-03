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
#include "llvm/IR/LLVMContext.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


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
					ReturnInst* retInst;
					Instruction* calleeInst;
					StoreInst* caller_strInst; //holds the store instruction that comes after the call instruction in non void functions
					//Miscellaneous
				  Value* actualArg; //A value type variable that iteratively holds the actual arguments
					std::vector<ConstantInt*> actualArgVector; //A vector that holds the arguments if they are constants.
					Value* strVal; //The value in the store instruction replacing the call instruction.
					Value* strPtr; //The return in the store instruction replacing the call instruction.
					ConstantInt* constArg; //A ConstInt type variable that holds constant arguments after casting from Value to ConstInt
				  unsigned numArgs; //number of arguments in a call instruction
					bool areArgsConst; //a flag. True if all arguments of a call instruction are constants.
					ValueToValueMapTy vmap;
					Value* retValue; //Holds the value of the return of the callee

//First, we need to iterate over all the instructions in the code, until we find a call instruction

						for (Function::iterator i = callerFunc->begin(), e = callerFunc->end(); i != e; ++i)
							for (BasicBlock::iterator I = i->begin(), E = i->end(); I!= E; ++I){
						//tries to cast every instruction to callInst class. Returns NULL if not a callInst
						if (!(callInst = dyn_cast<CallInst>(&*I))) continue; //if callInst is  NULL, i.e. intruction is not a call instruction, skip the rest of the iteration
						//Now that we found the call instruction, we need to check if all the argments are indeed constants.
						areArgsConst= true; //initialize the flag to be true. It will be set to false if we encounter a non const argument
						numArgs=callInst->getNumArgOperands(); //the number of the arguments of the all instruction so we can iterate over them
						for (unsigned ArgIdx=0; ArgIdx<numArgs; ++ArgIdx){
							actualArg=callInst->getArgOperand(ArgIdx); //checks one argument at a time
							if((constArg=dyn_cast<ConstantInt>(actualArg))) 	actualArgVector.push_back(constArg); //if the casting succeeds i.e. the argument is a constant, add the argument to vector
							else
							{ areArgsConst= false; //if at least one argument is not constant, lower the flag, and break the loop over the instructions
								break;
								}
							}
							if (!areArgsConst) continue; //if at least one  argument of the call instructions is not a constant, skip the rest of the iteration
							calleeFunc=callInst->getCalledFunction(); //get the callee definition. Now this callee function might be local, or external (defined in another file), or in some libarary
							//for e.g. printf, scanf. These might have constant arguments (printf("some string");). We need to ignore thes calls as they cannot be inlined.
							if(!(&*(inst_begin(calleeFunc)))) continue; //It checks the first intruction of the function definition. If it is external, it returns 0x0, which we can check for.

							if(std::distance(inst_begin(calleeFunc), inst_end(calleeFunc))>10) continue;//limits the number of IR instructions to inline to 10


							//Now that we know that
							//1-We have a call instruction.
							//2-All the arguments are constants
							//3-The function declaration is local within the file.
							//4-The calee function has less than 10 IR instructions
							//We will replace all the formal arguments with the actual constant arguments within the function definition.


							unsigned Idx=0; //An iterator to iterate over the vector actualArgVector, along with the arguments of the function definition
							for (Function::arg_iterator ArgPtr = calleeFunc->arg_begin(), ArgEnd= calleeFunc->arg_end(); ArgPtr !=ArgEnd; ++ArgPtr){ //iterating over the formal arguments
								constArg = actualArgVector[Idx++]; //get the correspoding actual argument
								ArgPtr->replaceAllUsesWith(constArg); //and replaces all the uses of the formal argument with the actual argument.
								}
							actualArgVector.clear();  //ensures the vector is indeed empty in the case of multiple call functions.


							//Now, the function declaration is ready to be copied into the call site. We will clone each intruction and move the clone right before the call instruction.
							//This makes the function declaration dead.
							for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I){	//iterating over the instructions in the callee definition
								//Now, we copy the modified function definition to the call site. We want to copy ALL the instructions in the function definition except the return instruction, so we break when we find it,
								//we store the return value because it will be useful later
								if ((retInst = dyn_cast<ReturnInst>(&*callee_I)))
									{	retValue=retInst->getReturnValue();
										//for ret void, getNumOperands returns 0.
											break;}
									//we map the new instructions to the old instructions' values. This copies the cfg subtree from the definition to the call site, and fixes broken references
									calleeInst = callee_I->clone(); //Now, for a normal instruction, we first clone int
									calleeInst->insertBefore(&*I); //then move it just before the call instruction
							    //&*I->getParent()->getInstList().insert(&*I,&*calleeInst); //this is an alternative way to do so that will also work
									vmap[&*callee_I] = calleeInst; //then we remap the new instructions to map to the original values
									RemapInstruction(calleeInst, vmap, RF_NoModuleLevelChanges);
						}

						ReplaceInstWithValue(i->getInstList(), I, retValue); //We replaces the call instruction with the return value of the callee and deletes it, so uses of the call instruction now uses the return value directly.
						//Fixing broken references
						for (BasicBlock::iterator lookahead_iterator = I; lookahead_iterator!= E; ++lookahead_iterator)
							RemapInstruction(lookahead_iterator, vmap, RF_NoModuleLevelChanges);





					}
					return true; //return true as the pass has changed the file
				}

};
}

char Function_Inlining::ID = 1;
static RegisterPass<Function_Inlining> X("func_inline", "Function Inlining Pass", false, false);// ReplaceInstWithValue(bs->getInstList(), I, retValue);
