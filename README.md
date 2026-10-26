# DBLayer C++ ORM

## Short Description

**DBLayer** is a C++ ORM (Object-Relational Mapping) library developed circa 2006-2015 by Roberto Rocco Angeloni. It provides a database abstraction layer supporting multiple DBMS (MySQL, PostgreSQL, SQLite, ODBC, XML-RPC) with a factory pattern for entity creation, type introspection, and SQL generation.

### Core Features
- **Entity-system**: `DBEntity` base class with schema metadata, field management, and CRUD operations
- **Field types**: `BooleanField`, `IntegerField`, `FloatField`, `StringField`, `DateField` with type checks (`isBoolean()`, `isInteger()`, `isFloat()`, `isString()`, `isDate()`)
- **Factory pattern**: `Factory<T>` template for dynamic entity creation by table name
- **Connection plugins**: 5+ database drivers (sqlite, mysql, pg, odbc, xmlrpc) via `registerConnectionType()`
- **Schema→SQL**: `toSql()` generates CREATE TABLE statements with column types and foreign keys
- **Query building**: `_buildInsertString`, `_buildUpdateString`, `_buildDeleteString`, `_buildWhereCondition` with LIKE support
- **UUID generation**: `getNextUuid()` with Boost or Qt fallback

### Project Structure
```
src/dblayer/     - Core ORM code (entities, fields, connections, manager)
src/qdblayer/    - Qt-specific extensions
src/schema/      - Schema system (Field base class, type traits)
src/apps/        - Tools (DBShell, code generation)
src/tests/       - Unit tests (Catch2)
catch/           - Catch2 test framework header
```

---

## Installing Catch2

### Option A: System package (Cachyos/Arch)
```bash
# Already installed via pacman
pacman -S catch2
```

Catch2 headers will be at `/usr/include/catch2/`.

### Option B: Header-only manual download
```bash
# Create catch directory
mkdir -p /path/to/dblayer/catch

# Download single-header Catch2 (latest v3.x)
cd /path/to/dblayer/catch
curl -sL "https://github.com/catch2/catch3/archive/refs/tags/v3.9.1.tar.gz" -o catch3.tar.gz
tar xzf catch3.tar.gz
# Or simpler: just grab the catch.hpp from a release
curl -sL https://github.com/catch2/catch3/raw/refs/heads/develop/catch.hpp -o catch.hpp
```

### Option C: CMake integration
```cmake
# In CMakeLists.txt
find_package(Catch2)
# Or for header-only:
include_directories(${CMAKE_SOURCE_DIR}/catch)
```

---

## Running Unit Tests

### Prerequisites
- GCC 11+ (C++17)
- Catch2 library (see above)
- Project compiled with C++17 support

### Building & Running a Single Test

```bash
# Build the test executable
cd /path/to/dblayer
mkdir -p build && cd build

# Method 1: Manual g++ compilation
g++ -std=c++17 \
    -I/usr/include/catch2 \
    -I. -Isrc -Idblayer \
    src/tests/integer2string_test.cpp \
    -o tests/integer2string_test \
    -L/usr/lib -lcatch2  # may need adjustment

# Method 2: Using CMake (if cmake available)
cmake .. -DCMAKE_BUILD_TYPE=Debug
make integer2string_test
```

### Running a Single Test
```bash
./tests/integer2string_test
# Output will show TEST PASS/FAIL for each SECTION
```

### Building & Running All Tests

```bash
# Build all three test executables
g++ -std=c++17 \
    -I/usr/include/catch2 \
    -I. -Isrc -Idblayer \
    src/tests/integer2string_test.cpp \
    src/tests/isFloat_test.cpp \
    src/tests/field_destructor_test.cpp \
    -o tests/run_all_tests \
    -lcatch2

# Run all tests
./tests/run_all_tests
```

### Expected Output Format
```
 integer2string produces correct output -------------------- PASS
   positive small number ------------------------------ PASS
   positive medium number ----------------------------- PASS
   positive large number ------------------------------ PASS
   zero ------------------------------------------------- PASS
   negative number -------------------------------------- PASS
   large negative number -------------------------------- PASS
   number at boundary of 10 ----------------------------- PASS
 integer2string from schema namespace works ------------ PASS
   SchemaNS::integer2string(42) == "42" --------------- PASS
   SchemaNS::integer2string(-1) == "-1" --------------- PASS
   SchemaNS::integer2string(0) == "0" ----------------- PASS
 isFloat test -------------------------------------------- PASS
   Boolean field is not float --------------------------- PASS
   Integer field is not float --------------------------- PASS
   Float field IS float --------------------------------- PASS
   String field is not float ---------------------------- PASS
   Date field is not float ------------------------------ PASS
   isInteger works correctly alongside isFloat ---------- PASS
 field destructor test -------------------------------- PASS
   BooleanField destructor does not leak -------------- PASS
   IntegerField destructor does not leak -------------- PASS
   StringField destructor properly cleans stringValue -- PASS
   FloatField destructor does not leak ---------------- PASS
   DateField destructor does not leak ----------------- PASS
```

**All tests: 15 passed** (or similar summary line)

---

## Practical Information

### Compiling the Library (without tests)
```bash
mkdir -p build && cd build
g++ -std=c++17 -O2 -I. -Isrc -Idblayer \
    -c src/dblayer/dblayer.cpp -o dblayer.o \
    -c src/schema/schema.cpp -o schema.o \
    -c src/dblayer/booleanfield.cpp -o booleanfield.o \
    # ... other .cpp files
# Link
ar rcs libdblayer.a *.o
```

### Notes on C++ Standard
The project was written for C++11/14 era but compiles with C++17. Key findings:
- `integer2string()` had an off-by-one bug fixed by using `std::to_string()`
- `isDouble()` always returned `type==Field::FLOAT` — renamed to `isFloat()`
- Empty destructors in field classes are safe (base class handles `stringValue`)
- Heavy `#ifdef` usage for platform differences (WIN32, __i386__)

### Known Quirks
- `main()` is not defined in the library; tests use `CATCH_CONFIG_MAIN`
- `config.h` is expected in the build directory (generated from `config.h.in`)
- Some old C++ patterns: `std::ptr_fun`, `std::not1`, variable-length arrays
- Static global state: `DBEntity::_columns`, `_column_order`, `_fkv` shared across entity types

### Debugging Tips
- Run with verbose: `./dbShell dblayer:sqlite:./examples/test.db` then type `help`
- Check `SchemaNS::getFieldCreati()` / `getFieldDistrutti()` for leak detection
- Use `getNextUuid()` to test UUID generation with/without Boost
- The `table2cpp` command generates C++ code from DB introspection

---

## Building the Full Project

If cmake is available:
```bash
cd /data/roberto/projects/dblayer
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

Or manually with g++ (see individual .cpp compilation above).

### Test CMakeLists.txt
The project includes a CMakeLists.txt in `src/tests/` that builds 3 test executables:
- `integer2string_test` - Tests the fixed `integer2string()` utility
- `isFloat_test` - Tests `isFloat()` / `isInteger()` type checks
- `field_destructor_test` - Tests field class destructors