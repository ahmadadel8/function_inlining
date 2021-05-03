#!/bin/bash
cd project/obj
make install
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example0.ll -o ~/project/test_codes/Function-inlining/example.0inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example1.ll -o ~/project/test_codes/Function-inlining/example.1inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example2.ll -o ~/project/test_codes/Function-inlining/example.2inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example3.ll -o ~/project/test_codes/Function-inlining/example.3inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example4.ll -o ~/project/test_codes/Function-inlining/example.4inline.bc
opt -load ../opt/lib/libFunction_Inlining.so -func_inline ~/project/test_codes/Function-inlining/example5.ll -o ~/project/test_codes/Function-inlining/example.5inline.bc

llvm-dis ~/project/test_codes/Function-inlining/example.0inline.bc -o ~/project/test_codes/Function-inlining/example.0inline.ll
llvm-dis ~/project/test_codes/Function-inlining/example.1inline.bc -o ~/project/test_codes/Function-inlining/example.1inline.ll
llvm-dis ~/project/test_codes/Function-inlining/example.2inline.bc -o ~/project/test_codes/Function-inlining/example.2inline.ll
llvm-dis ~/project/test_codes/Function-inlining/example.3inline.bc -o ~/project/test_codes/Function-inlining/example.3inline.ll
llvm-dis ~/project/test_codes/Function-inlining/example.4inline.bc -o ~/project/test_codes/Function-inlining/example.4inline.ll
llvm-dis ~/project/test_codes/Function-inlining/example.5inline.bc -o ~/project/test_codes/Function-inlining/example.5inline.ll


opt -constprop ~/project/test_codes/Function-inlining/example.0inline.ll -o ~/project/test_codes/Function-inlining/example.0inline.constprop.bc
opt -constprop ~/project/test_codes/Function-inlining/example.1inline.ll -o ~/project/test_codes/Function-inlining/example.1inline.constprop.bc
opt -constprop ~/project/test_codes/Function-inlining/example.2inline.ll -o ~/project/test_codes/Function-inlining/example.2inline.constprop.bc
opt -constprop ~/project/test_codes/Function-inlining/example.3inline.ll -o ~/project/test_codes/Function-inlining/example.3inline.constprop.bc
opt -constprop ~/project/test_codes/Function-inlining/example.4inline.ll -o ~/project/test_codes/Function-inlining/example.4inline.constprop.bc
opt -constprop ~/project/test_codes/Function-inlining/example.5inline.ll -o ~/project/test_codes/Function-inlining/example.5inline.constprop.bc

llvm-dis ~/project/test_codes/Function-inlining/example.0inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.0inline.constprop.ll
llvm-dis ~/project/test_codes/Function-inlining/example.1inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.1inline.constprop.ll
llvm-dis ~/project/test_codes/Function-inlining/example.2inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.2inline.constprop.ll
llvm-dis ~/project/test_codes/Function-inlining/example.3inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.3inline.constprop.ll
llvm-dis ~/project/test_codes/Function-inlining/example.4inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.4inline.constprop.ll
llvm-dis ~/project/test_codes/Function-inlining/example.5inline.constprop.bc -o ~/project/test_codes/Function-inlining/example.5inline.constprop.ll
