COOL Compiler
-------------

This is an implementation of a compiler for the COOL programming language. 

The manual/standard for COOL is located here: [http://theory.stanford.edu/~aiken/software/cool/cool-manual.pdf](http://theory.stanford.edu/~aiken/software/cool/cool-manual.pdf)

Target
----------

The target architecture for this compiler is MIPS. There is an excellent MIPS simulator by James Larus called [SPIM](http://spimsimulator.sourceforge.net/), which can be used to run the output of the compiler.

Dependencies
------------

* C++ compiler with C++11 support - lambda, auto, shared_ptr, vector initializer, etc
* GNU Flex
* GNU Bison
* Boost
* SPIM

Build
-----

1. Clone the repository: git clone git://github.com/cchuahuico/COOL-Compiler.git
2. cd to the root of the project
3. cd src
4. make

Usage
-----------

To compile COOL source files, simply run: cooc source.cl

To run the output using QtSpim:

1.  From the menu, click Simulator -> Settings
2.  Go to the MIPS tab and change the exception handler to *projectroot*/lib/trap.handler.s
3.  Click OK
4.  From the menu, click File -> Reinitialize and Load file 
5.  Choose the output then OK
