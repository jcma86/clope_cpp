## CLOPE in C++
CLOPE algorithm in C++
Algorithm description: https://loginom.com/blog/clope

### Requirements:
**CMake 3.5+**

### Compilation:
Under Linux/MacOS:
```bash
mkdir build
cd build
cmake ..
make
```

### Usage:
From build folder:
```bash
./test_clope
```

### Notes:
The algorithm is tested with the Mushroom Database: https://archive.ics.uci.edu/ml/datasets/mushroom  

In order to make specific implementations for other datasets you can take a look to the `dbmushroom.?pp` and `txmushroom.?pp` files and make your own for your dataset.
