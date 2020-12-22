gcc -shared -fPIC dbg.c -o dbg.so
g++ BKUTree.cpp
export MALLOC_TRACE=out.txt
LD_PRELOAD=./dbg.so ./a.out
