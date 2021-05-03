#!/bin/bash
cd project/obj
setenv PATH /afs/glue.umd.edu/class/old/enee/759c/llvm/llvm-3.4-install/opt/bin/:$PATH
make install
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example0.ll -o example.0inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example1.ll -o example.1inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example2.ll -o example.2inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example3.ll -o example.3inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example4.ll -o example.4inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example5.ll -o example.5inline.bc

llvm-dis example.0inline.bc -o example.0inline.ll
llvm-dis example.1inline.bc -o example.1inline.ll
llvm-dis example.2inline.bc -o example.2inline.ll
llvm-dis example.3inline.bc -o example.3inline.ll
llvm-dis example.4inline.bc -o example.4inline.ll
llvm-dis example.5inline.bc -o example.5inline.ll


opt -constprop example.0inline.ll -o example.0inline.constprop.bc
opt -constprop example.1inline.ll -o example.1inline.constprop.bc
opt -constprop example.2inline.ll -o example.2inline.constprop.bc
opt -constprop example.3inline.ll -o example.3inline.constprop.bc
opt -constprop example.4inline.ll -o example.4inline.constprop.bc
opt -constprop example.5inline.ll -o example.5inline.constprop.bc

llvm-dis example.0inline.constprop.bc -o example.0inline.constprop.ll
llvm-dis example.1inline.constprop.bc -o example.1inline.constprop.ll
llvm-dis example.2inline.constprop.bc -o example.2inline.constprop.ll
llvm-dis example.3inline.constprop.bc -o example.3inline.constprop.ll
llvm-dis example.4inline.constprop.bc -o example.4inline.constprop.ll
llvm-dis example.5inline.constprop.bc -o example.5inline.constprop.ll
