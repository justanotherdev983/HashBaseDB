TODO:

- make simple ui with sdl
- add cleanup button to ui, so it deletes the deleted marked entries
- rename entire project to "FilemanDB"

- add tests to github
- make the storage binary based
- simple cashing
- use LSM-tree

IDEAS:

// PROJECT STRUCTURE

2 Week Deadline (Nov 7)

Core Features:

Clean up current code into proper files
O(1) search with proper hash table
Multiple values per key (vector based)
Basic cleanup function
Binary storage format
Simple error handling

Project Quality:

Unit tests
Benchmarks
Good documentation
Example programs

Folder Structure (2 weeks):
Copyproject/
├── src/
│ ├── main.cpp
│ ├── database.cpp
│ ├── hash.cpp
│ └── storage.cpp # Binary storage handling
├── include/
│ ├── database.hpp
│ ├── hash.hpp
│ └── storage.hpp
├── tests/
│ ├── unit_tests.cpp
│ └── benchmarks.cpp
├── examples/
│ └── usage.cpp
└── docs/
├── DESIGN.md # Explain your technical choices
└── API.md # How to use the database
Final Project (2-3 months)

Advanced Features:

LSM tree implementation
Proper caching system
SDL UI with:

Add/remove entries
Search functionality
View multiple values
Statistics display

Concurrent read support
Range queries
Compression

Professional Touches:

Performance tuning
Memory-mapped files
Crash recovery
Full documentation
Integration tests

Final Folder Structure:
Copyproject/
├── src/
│ ├── core/
│ │ ├── database.cpp
│ │ ├── hash.cpp
│ │ └── storage.cpp
│ ├── lsm/
│ │ ├── tree.cpp
│ │ └── compaction.cpp
│ ├── cache/
│ │ └── lru_cache.cpp
│ └── ui/
│ ├── window.cpp
│ ├── widgets.cpp
│ └── stats.cpp
├── include/
│ ├── core/
│ ├── lsm/
│ ├── cache/
│ └── ui/
├── tests/
│ ├── unit/
│ ├── integration/
│ └── benchmarks/
├── tools/
│ ├── db_inspect # Debug tool
│ └── stress_test # Load testing
├── examples/
│ ├── basic.cpp
│ ├── advanced.cpp
│ └── ui_demo.cpp
└── docs/
├── design/
│ ├── lsm.md
│ └── storage.md
├── benchmarks/
└── images/ # Screenshots, diagrams
This progression lets you:

Get a solid working base in 2 weeks
Add impressive features over the next months
End up with something really substantial

The end result would be way beyond a normal school project - more like a real database engine!

// UI in sdl
+------------------+
| Add Entry |
| Key: [ ] |
| Value: [ ] |
| [Add] [Search] |
+------------------+
| Current Entries |
| ID Value |
| 123 John |
| 456 Sarah |
| [Delete] [Clean]|
+------------------+
| Stats |
| Items: 50 |
| Cache hits: 30 |
+------------------+

// README in github

# Custom NoSQL Database Engine

- High-performance key-value store
- O(1) lookups
- Multiple values per key
- Custom SDL interface

[Screenshot of UI]

## Performance

- O(1) read complexity
- Efficient storage

BUGS:

- cant insert and delete multiple keys at once // no biggie, in ui will delete entries once

// TEST
Hashmap hashmap("../database/winkel.db", "../database/index.idx");

    uint64_t key = hashmap.hashify("12345");
    hashmap.insert(key, "example_value");

    // Try to get the value
    std::string result = hashmap.get_val_from_key("12345");
    std::cout << "Found value: " << result << std::endl;

    // Try a non-existent key
    result = hashmap.get_val_from_key("99999");
    std::cout << "Non-existent key result: " << result << std::endl;

    // Remove and try to get again
    hashmap.remove(key);
    result = hashmap.get_val_from_key("12345");
    std::cout << "After removal: " << result << std::endl;

    //!!!!!! second


    Hashmap hashmap("../database/winkel.db", "../database/index.idx");

    // Clear files first (optional)
    std::ofstream clear_db("../database/winkel.db", std::ios::trunc);
    std::ofstream clear_idx("../database/index.idx", std::ios::trunc);
    clear_db.close();
    clear_idx.close();

    uint64_t key1 = hashmap.hashify("test1");
    hashmap.insert(key1, "value1");
    hashmap.insert(key1, "value2");
    hashmap.insert(key1, "value3");

    uint64_t key2 = hashmap.hashify("test2");
    hashmap.insert(key2, "other_value");

    std::cout << "\nBefore deletion:\n";
    hashmap.debug_print_files();

    auto values1 = hashmap.get_val_from_key("test1");
    std::cout << "Values for test1:\n";
    for (const auto& value : values1) {
        std::cout << "  " << value << '\n';
    }

    hashmap.remove(key1);

    std::cout << "\nAfter deletion:\n";
    hashmap.debug_print_files();

// NEW IDEA

project/
├── database/
│ ├── index.idx
│ └── data.db
│
├── include/
│ ├── core/
│ │ ├── hash.hpp
│ │ └── database.hpp
│ └── ui/
│ ├── window.hpp
│ └── components.hpp
│
├── src/
│ ├── core/
│ │ ├── hash.cpp # Hash functions & bucket logic
│ │ └── database.cpp # File operations & data management
│ │
│ ├── service/
│ │ └── db_service.cpp # Bridge between UI and core
│ │
│ └── ui/
│ ├── window.cpp # Main SDL window management
│ └── components/
│ ├── table.cpp # Data display
│ ├── input.cpp # Input handling
│ └── buttons.cpp # UI controls
│
├── tests/
│ ├── core_tests.cpp
│ └── ui_tests.cpp
│
└── docs/
├── API.md
└── DESIGN.md

Data Flow:

    1. User Request → Database Module
    2. Database Module → Hash Module (for calculations)
    3. Hash Module → Database Module (returns locations)
    4. Database Module → Files (performs I/O)
    5. Database Module → User (returns results)

flowchart TD
A[main.cpp] --> B[Database_service::run]
B --> C[UI_service::initialize]
B --> D[Database::init_db]

    B --> E[UI_service::run]
    E --> F[Event Loop]
    F --> G[Handle Input]
    F --> H[Render Screen]
    G --> I[User Actions]
    I --> J[Add Record]
    I --> K[Search Records]
    I --> L[View Data]

    J --> M[Database_service::add_record]
    K --> N[Database_service::search_database]

    M --> O[Database::insert_entry]
    N --> P[Database File Operations]

    B --> Q[Cleanup]
    Q --> R[Database::cleanup_db]
    Q --> S[UI_service::cleanup]
