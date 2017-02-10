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
  
