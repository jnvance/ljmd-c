This package contains simplified MD code with multi-threading
parallelization for simulating atoms with a Lennard-Jones potential.

The bundled makefiles are set up to compile the executable once
with OpenMP disabled and once with OpenMP enabled with each build
placing the various object files in separate directories.

The examples directory contains 3 sets of example input decks
and the reference directory the corresponding outputs.

Type: make
to compile everything and: make clean
to remove all compiled objects
Project tree
-----------
bin – Output executables (`targets`). The suffix `_d` indicates debug version.

obj – Binary objects created during compilation.

src – Source files of the project with subdirectories if needed.

include – Files used by the main executable.

test – Source files of tests.

doc – Project’s documentation, possibly auto-generated.

.gitignore – Prevents adding binary and temporary files to the git repository.

README.md – General information about the project in Markdown format.

Compilation
-----------
In the root directory:
  ```
  make
  make shared
  make tests
  ```
Usage
----
  Running the executable directly
  ```
  cd examples
  ../bin/ljmd.x < [inputfile].inp
  ```
  Running from the python script
  ```
  cd examples
  ../python/ljmd.py [inputfile].inp
  ```
Benchmark
---------
Benchmarks are done with `gprof` compilation flag `-pg` added and removed the conflicting flag `-fomit-frame-pointer`
After factorization the program is benchmarked with `argon_108` as input the result is as follows.
```
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 76.22      3.60     3.60    10001   359.72   465.94  force
 21.02      4.59     0.99 346714668     0.00     0.00  pbc
  1.49      4.66     0.07    30006     2.34     2.34  azzero
  1.49      4.73     0.07    10000     7.01   472.95  velverlet
  0.00      4.73     0.00    10001     0.00     0.00  ekin
  0.00      4.73     0.00      101     0.00     0.00  output
  0.00      4.73     0.00       12     0.00     0.00  get_a_line
```
As one can see the pbc function is called many times. One can optimize this using the **Newton's Third law** thereby calculating the force between 2 pair of atoms only once instead of twice. After that implementation we get a significant performance improvement.
```
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 75.75      1.92     1.92    10001   192.38   247.49  force
 20.91      2.46     0.53 173357334     0.00     0.00  pbc
  2.76      2.53     0.07    10000     7.01   254.50  velverlet
  0.79      2.55     0.02    30006     0.67     0.67  azzero
  0.00      2.55     0.00    10001     0.00     0.00  ekin
  0.00      2.55     0.00      101     0.00     0.00  output
  0.00      2.55     0.00       12     0.00     0.00  get_a_line
```
As shown in the table the performance is improved `2x`. But this can be improved further by avoiding to calculate expensive operations such as `pow()` and `sqrt()` inside the force loop. after this optimization we get the results as
```
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 73.12      1.35     1.35    10001   135.26   183.36  force
 25.46      1.82     0.47 173357334     0.00     0.00  pbc
  1.08      1.84     0.02    10000     2.00   185.36  velverlet
  0.54      1.85     0.01    30006     0.33     0.33  azzero
  0.00      1.85     0.00    10001     0.00     0.00  ekin
  0.00      1.85     0.00      101     0.00     0.00  output
  0.00      1.85     0.00       12     0.00     0.00  get_a_line
```
Here the performance is improved by 30% of the original code. 
