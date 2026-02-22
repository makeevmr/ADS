# CustomDS

My implementation of widely used algorithms and data structures

## Run tests

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=[Release|Debug]
cmake --build . --target ${TARGET_NAME}
${EXECUTABLE_PATH}
```

For example
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target test_segment_tree
./tests/ds/test_segment_tree
```

## Tests targets

### Algorithms
- `test_euclidean`
- `test_kmp`
- `test_sieve`

### Data structures
- `test_aho_corasick`
- `test_segment_tree`

## Executable paths

### Algorithms
- `./tests/algo/test_euclidean`
- `./tests/algo/test_kmp`
- `./tests/algo/test_sieve`

### Data structures
- `./tests/ds/test_aho_corasick`
- `./tests/ds/test_segment_tree`
