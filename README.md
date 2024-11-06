# HashbaseDB: Custom NoSQL Database Engine

A high-performance key-value store with an easy-to-use SDL interface.

## Features

- **O(1) lookups**: Fast search times for keys.
- **Multiple values per key**: Store multiple values for a single key using a vector-based structure.
- **Simple SDL interface**: A custom user interface built using SDL for easy interaction.
- **Efficient storage**: Binary storage format for fast data retrieval.

## Project Structure

── database
│ ├── index.idx
│ └── winkel.db
├── docs
│ ├── API.md
│ ├── DESIGN.md
│ └── notes.md
├── include
│ ├── core
│ │ ├── database.hpp
│ │ ├── hash.hpp
│ │ └── trim.hpp
│ ├── frontend
│ │ ├── components
│ │ │ ├── add_record.hpp
│ │ │ ├── main_menu.hpp
│ │ │ ├── remove_record.hpp
│ │ │ └── search_record.hpp
│ │ ├── types.hpp
│ │ └── window.hpp
│ ├── services
│ │ ├── db_service.hpp
│ │ └── ui_service.hpp
│ └── test
│ └── unit_tests.hpp
├── output
│ └── hashmap.h
└── src
├── core
│ ├── database.cpp
│ └── hash.cpp
├── frontend
│ ├── components
│ │ ├── add_record.cpp
│ │ ├── main_menu.cpp
│ │ ├── remove_record.cpp
│ │ └── search_record.cpp
│ └── window.cpp
├── main.cpp
├── services
│ ├── db_service.cpp
│ └── ui_service.cpp
└── test
└── unit_tests.cpp

# Dependencies

    SDL2, cmake, clang

# Program flow diagram

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
