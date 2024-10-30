# Programming Test with NDI
Small refactor of NDI_Find code example to use modern C++ Standard

## Building
To build it, the default Makefile that came with the SDK installation was used, with the following minor changes:

This repository folder was placed inside ```examples/C++``` folder.

In the Makefile, this folder's name was added to the PROJECTS variable.

In the Makefile.proj file, the CXXFLAGS -std=c++11 was changed to -std=c++17

Finally to build this example (and all the other ones), simply:

```
make all
```
The executable will be found alongside this README and the other files from this repository.

## Run the example

After building, just run the generated executable:
```
./Programming-Test-with-NDI/Programming-Test-with-NDI
```

In case you have sources running on your network, this is an example of result you'll be able to see in the terminal:

```
Network sources (4 found).
4. MAGNOPC (Test Pattern)
3. MAGNOPC (Test Pattern 4)
2. MAGNOPC (Test Pattern 3)
1. MAGNOPC (Test Pattern 2)
No change to the sources found.
No change to the sources found.
No change to the sources found.
No change to the sources found.
No change to the sources found.
No change to the sources found.
No change to the sources found.
```

And the list of sources found will be also added to the created file list_of_sources.txt.

```
MAGNOPC (Test Pattern); MAGNOPC (Test Pattern 4); MAGNOPC (Test Pattern 3); MAGNOPC (Test Pattern 2); 
```