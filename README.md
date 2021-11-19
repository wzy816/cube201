# cube201

```bash
# create build folder
mkdir build

# build && run test
cmake -S . -B build && cmake --build build && ./build/cubepos_test

# run pair_search
./build/pair_search
# step size = 1

# compress pair.txt
cd data
tar -cvjf pair.tar.bz2 ./pair/pair*.txt

# pair lookup
```

## ref

- [cube20](http://www.cube20.org/)
