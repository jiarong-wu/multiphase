### A multi-phase flow solver
Tianju Xue, Yiheng Tao, and Jiarong Wu <br>
APC 524, Fall 2018, Princeton University <br>
January 15, 2019

In this final project, we developed a solver that could be used to solve for problems relevant to mutli-phase flow in porous media. There are 5 folders (namely: `solver`, `eigen`, `test`, `plot`, and `doc`) and this `README` file in the submission bundle. The folder `solver` contains the header files and source files we created to develop the solver, and the files relavent to the three test cases. The folder `eigen` is an external library we employed in our solver to solve the system of linear equations. The folder `test` contains files we wrote to test the code along the development process. The folder `plot` includes two Python files, which we used to plot the comparison between numerical and analytical solutions in the test cases. Finally, the folder `doc` contains the Doxyfile that we used to do documentation. 

We designed three test cases, which can be used to evaluate the solver. In the first test case, the solver solves the Poisson's Equation. In the second test case, the solver solves a multiphase flow problem. In the third test case, the solver solves a geological CO2 injection problem with realistic parameters. The results of test cases #1 and #3 are presented in the final report. 

To run a test case, simply assign a value to the 'constant integter FLAG' in solver/include/common_definitions.h. FLAG = 0, 1, 2 indicates test case #1, #2, #3, respectively. Then go to solver/ and type "make" to start running the test case. While running, the screen will show the current time step and the total time step as an indication of current progress. The solver will generate output files in progression and, at the end of simulation, the solver will put the output files to solver/output. Those output files can be viweded using the software Paraview for further analyses.

Next, we provide the instruction to assess the files we wrote for testing and profiling. The testing was done using googletest. And we use gprof for the profiling. (Jiarong could you please write something about testing and profiling here?)







Instruction on using googletest:

1. Clone the repo from https://github.com/google/googletest
2. Follow the README file under /googletest

To do:
Create a script for google test setup using relative path.

Suppose the test file is under ${GTEST_DIR}/src/gtest-all.cc
Compile like:
g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} \
    -pthread -c ${GTEST_DIR}/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o


Instructions on using gprof for profiling purpose:
1. Insert -pg flag when compiling
2. In a terminal, use command "gprof -a Flow gmon.out > analysis.txt"
