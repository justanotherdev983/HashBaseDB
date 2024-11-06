# HashbaseDB: Custom NoSQL Database Engine

A high-performance key-value store with an easy-to-use SDL interface, written for Linux systems.

## Features

- **O(1) lookups**: Fast search times for keys.
- **Multiple values per key**: Store multiple values for a single key using a vector-based structure.
- **Simple SDL interface**: A custom user interface built using SDL for easy interaction.
- **Efficient storage**: Binary storage format for fast data retrieval.

## Hashbase Project Structure

## Database Files

- `database/`
  - `index.idx` - Index file for O(1) lookups
  - `winkel.db` - Main database storage

## Documentation

- `docs/`
  - `DESIGN.md` - Design decisions

## Header Files

- `include/`
  - **Core Database Functionality**
    - `core/`
      - `database.hpp` - Database operations
      - `hash.hpp` - Hashing functions
      - `trim.hpp` - String utilities
  - **UI-related Headers**
    - `frontend/`
      - **Components**
        - `components/`
          - `add_record.hpp`
          - `main_menu.hpp`
          - `remove_record.hpp`
          - `search_record.hpp`
      - `types.hpp` - UI type definitions
      - `window.hpp` - Window management
  - **Service Layer Headers**
    - `services/`
      - `db_service.hpp` - Database service
      - `ui_service.hpp` - UI service
  - **Test Headers**
    - `test/`
      - `unit_tests.hpp` - Unit test definitions

## Build Output

- `output/`
  - `hashmap.h` - Generated hash map header

## Source Files

- `src/`
  - **Core Implementations**
    - `core/`
      - `database.cpp` - Database operations
      - `hash.cpp` - Hash functions
  - **UI Implementations**
    - `frontend/`
      - **Components**
        - `components/`
          - `add_record.cpp`
          - `main_menu.cpp`
          - `remove_record.cpp`
          - `search_record.cpp`
      - `window.cpp` - Window management
  - **Service Implementations**
    - `services/`
      - `db_service.cpp` - Database service
      - `ui_service.cpp` - UI service
  - **Test Implementations**
    - `test/`
      - `unit_tests.cpp` - Unit test suite
  - `main.cpp` - Application entry point

# Dependencies

    cmake, clang, SDL2

# How to run

    $cmake
    $cd build && make
    $./hashbase

## Program Flow Diagram

```mermaid
flowchart TD
    A[main.cpp] --> B[Database_service::run]
    B --> D[UI_service::initialize]
    D --> E[Window::initialize]
    E --> F[Window::switch_screen]
    F --> G[Main Menu Screen]
    F --> H[Add Record Screen]
    F --> I[Remove Record Screen]
    F --> J[Search Record Screen]
    G --> K[UI_service::main_menu]
    H --> L[UI_service::add_record]
    I --> M[UI_service::remove_record]
    J --> N[UI_service::search_record]
    K --> O[Database_service::add_record]
    L --> P[Database_service::search_database]
    M --> Q[Database_service::insert_entry]
    P --> R[Database::search_functionality]
    Q --> S[Database::insert_entry]
    O --> T[Database::use_hash]
    P --> U[Database::use_hash]
    R --> V[Database::use_types]
    S --> W[Database::use_types]
    B --> X[Database::init_db]
    B --> Y[Database::cleanup_db]
    B --> Z[UI_service::cleanup]
    Z --> AA[Window::cleanup]
    Y --> AB[Database::cleanup]
    AA --> AC[UI_service::cleanup]
```
