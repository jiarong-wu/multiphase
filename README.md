### A multi-phase flow solver
Tianju Xue, Yiheng Tao, and Jiarong Wu <br>
APC 524, Fall 2018, Princeton University <br>
January 15, 2019

In this final project, we developed a solver that could be used to solve for problems relevant to mutli-phase flow in porous media. There are 5 folders (namely: `solver`, `eigen`, `test`, `plot`, and `doc`) and this `README` file in the submission bundle. The folder `solver` contains the header files and source files we created to develop the solver, and the files relavent to the three test cases. The folder `eigen` is an external library we employed in our solver to solve the system of linear equations. (Notice that the external library is not tracked by git. For the code to run, the user must first download Eigen and put the folder under the main directory.) The folder `test` contains files we wrote to test the code along the development process. The folder `plot` includes two Python files, which we used to plot the comparison between numerical and analytical solutions in the test cases. Finally, the folder `doc` contains the Doxyfile that we used to do documentation. 

We designed three test cases, which can be used to evaluate the solver. In the first test case, the solver solves the Poisson's Equation. In the second test case, the solver solves a multiphase flow problem. In the third test case, the solver solves a geological CO<sub>2</sub> injection problem with realistic parameters. The results of test cases #1 and #3 are presented in the final report. 

To run a case, go to `/solver` and type `make` to start compiling the code. The output executable file is RUN. Use command `./RUN <CASE_FLAG> <ADAPT_FLAG>`. If the parameter number is not correct, a prompt `USAGE: ./RUN <problem type> <adaptive option>` will show on the screen. `FLAG` = 0, 1, 2 indicates test case #1, #2, #3, respectively, and`ADAPT_FLAG` = 0 or 1 indicates using adaptive mesh or not. Notice that the adaptive mesh has not be implemented for case 1 and 2, so now it can be only used with `FLAG` = 0. While running, the screen will show the current time step and the total time step as an indication of current progress. The solver will generate output files in progression and, at the end of simulation, the solver will put the output files to `/solver/output`. Those output files can be viweded using the software Paraview for further analyses.

Additional instruction to assess the files we wrote for testing and profiling. The testing was done using googletest. And we use gprof for the profiling. 

For testing:
If using Ubuntu, simply go to `/test` and use `make` to compile the test code. If using other operating system, an already compiled Google Test library that's compatible with your own OS is needed and should be put under the `/test` directory. The `Makefile`should be change accordingly with your compiled library name in the linking line like `	$(CXX) $(CXXFLAGS) $(CFLAGS) $^ <library> -o $@ $(TESTFLAGS)` instead of `	$(CXX) $(CXXFLAGS) $(CFLAGS) $^ libgtest.a -o $@ $(TESTFLAGS)`. 

For profiling purpose:
1. Insert `-pg` flag when compiling (already done in the given `/solver/Makefile`)
2. In a terminal, use command `gprof -a RUN gmon.out > <target plain txt file>` to get the output flat file and call graph.

