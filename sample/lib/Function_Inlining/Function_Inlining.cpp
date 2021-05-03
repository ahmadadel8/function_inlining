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
					Value* retValue;
					inst_iterator lookahead_iterator; //will be usefull to lookahead when looping over a function

//First, we need to iterate over all the instructions in the code, until we find a call instruction

					for (inst_iterator I = inst_begin(callerFunc), E=inst_end(callerFunc); I!=E; ++I)	{
						//errs()<<*I<<'\n';
						//tries to cast every instruction to callInst class. Returns NULL if not a callInst
						if ((callInst = dyn_cast<CallInst>(&*I))){ //if callInst is not NULL, i.e. intruction is indeed a call instruction
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
								if (areArgsConst){ //if all the arguments of the call instructions as constants
									calleeFunc=callInst->getCalledFunction(); //get the callee definition. Now this callee function might be local, or external (defined in another file), or in some libarary
									//for e.g. printf, scanf. These might have constant arguments (printf("some string");). We need to ignore thes calls as they cannot be inlined.
									if(&*(inst_begin(calleeFunc))){  //It checks the first intruction of the function definition. If it is external, it returns 0x0, which we can check for.

										//Now that we know that
										//1-We have a call instruction.
										//2-All the arguments are constants
										//3-The function declaration is local within the file.
										//We will replace all the formal arguments with the actual constant arguments within the function definition.

										unsigned Idx=0; //An iterator to iterate over the vector actualArgVector, along with the arguments of the function definition
										for (Function::arg_iterator ArgPtr = calleeFunc->arg_begin(), ArgEnd= calleeFunc->arg_end(); ArgPtr !=ArgEnd; ++ArgPtr){ //iterating over the formal arguments
											constArg = actualArgVector[Idx++]; //get the correspoding actual argument
											ArgPtr->replaceAllUsesWith(constArg); //and replaces all the uses of the formal argument with the actual argument.
											}
										actualArgVector.clear();  //ensures the vector is indeed empty in the case of multiple call functions.


										//Now, the function declaration is ready to be copied into the call site. We will clone each intruction and move the clone right before the call instruction.
										//This makes the function declaration dead.
										I++;
										for (inst_iterator callee_I = inst_begin(calleeFunc), callee_E=inst_end(calleeFunc); callee_I!=callee_E; ++callee_I){	//iterating over the instructions in the callee definition
											//This is where the code gets convoluted. We want to iterate over all instructions and copy them as is to the call site. However, there are two cases
											//1-If the function returns void, we want to copy all the instructions as is, except the return instruction, and then delete the call instruction
											//2-If the function returns a value, the IR takes the following form.
											//In the callee function, the last three instructions are storeInst that store the result of the computation in a variable, loadInst that loads such result in memory/registers, then the return instuction.
											// In the call site, the call instruction, which will store the return value in memory, and will then be followed by a store instructions which will link that pointer to the variable.
											//What we want to do is copy all the instructions in the callee except the last three, and then delete the call instruction and the storeInst after it, and replace it with another storeinstruction that stores the
											//resultant of the computation to the variable that the return of the call instruction was originally stored in.

											//So, we first check if we reached the return instruction and that it is indeed returning void
											if ((retInst = dyn_cast<ReturnInst>(&*callee_I)))
												{//for ret void, getNumOperands returns 0.
														break;}
												calleeInst = callee_I->clone(); //Now, for a normal instruction, we first clone int
												calleeInst->insertBefore(&*I); //then move it just before the call instruction
										    //&*I->getParent()->getInstList().insert(&*I,&*calleeInst); //this is an alternative way to do so that will also work
												vmap[&*callee_I] = calleeInst; //then we remap the instructions to update the dominator tree(not sure)
												RemapInstruction(calleeInst, vmap, RF_NoModuleLevelChanges);
									}


								}
							}
						}
						I--;
						//vmap[&*I]=retInst;
						//I->eraseFromParent(); //we break before copying the instruction and erase the call instruction, incrementing the iterator to point to the next instuction
						for(lookahead_iterator=inst_begin(callerFunc); lookahead_iterator!=E; lookahead_iterator++){
							//vmap[&*lookahead_iterator] = &*lookahead_iterator;
							RemapInstruction(&*lookahead_iterator, vmap, RF_NoModuleLevelChanges);}
					}
					errs()<<"Here \n";
					return true; //return true as the pass has changed the file
				}

};
}

char Function_Inlining::ID = 1;
static RegisterPass<Function_Inlining> X("func_inline", "Function Inlining Pass", false, false);

// ReplaceInstWithValue(bs->getInstList(), I, retValue);
// I++;
// errs()<<"Infinite loop\n";
