Function inlining for LLVM 3.4 C compiler
If a function call has constant actual arguments, the function definition replaces this call in the call site after replacing the formal arguents with the actual constant arguments, as long as the function definition has less than 10 IR instructions.
This was a final project for ENEE645 in my second term at UMD.
