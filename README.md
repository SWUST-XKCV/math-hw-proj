<div align="center">
    <h1>
        Math-HW-Proj
    </h1>
</div>

# Requirements

- Python 3.9+
    - Pandas
- CMake
- Gcc / Clang
- Ninja (optional)

# Build

```bash
cmake -B build -G Ninja && cmake --build build
```

# Run

```bash
cmake -B build -G Ninja && cmake --build build && python ./scripts/data_preprocessing.py && ./build/bin/app
```
