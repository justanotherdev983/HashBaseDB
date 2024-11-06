# HashBaseDB Design Document

## Overview

HashBaseDB is a custom (unsafe) NoSQL file-based database engine built with C++ focusing on performance, scalability, and ease of use. The project is designed to handle key-value pairs stored in a file system, using hashing techniques for efficient search and retrieval, with further extensibility for an SDL-based user interface.

## Language en technologies

- **C++** I began this project in plain C, while at the time i thought this was a great choice, i switched during development because of the lack of features it provides and the unsafe nature of the language. So instead i settled on something i haven't used before, C++. C++ is an excellent language for programming that can handle its own memory. So it shall be used responsibly. I choose it, because you have alot of freedom in the language and it is also very fun. This is my first time using the language and i will use it further down the line.

- **SDL** I choose SDL for one 2 reasons. The first is that it is relativly simple to use while giving you alot of power. The second is that i wanted to try something new and primarly ued in C++.

- **CMake** For my build system i used CMake, because it is the industry standard and not that difficult to use. I had no problems with it.

## Architectural Decisions

### 1. **File-Based Storage**

- **Reasoning:** The database stores data in a file to ensure persistence across program executions. Using a file-based approach is simple and avoids the overhead of maintaining a separate database server.
- **Implementation:** Data is stored as key-value pairs within a binary file, making it easy to expand and optimize storage later, when we will be Log-Structured Merge (LSM) trees and compaction.

### 2. **Own hashmap-ish Implementation**

- **Reasoning:** A hashmap (or hash table) is used for fast lookups, insertions, and deletions, providing an average O(1) time complexity for these operations.
- **Implementation:** The hash table is implemented manually, avoiding the use of standard library data structures such as `std::unordered_map` to ensure full control over the implementation and its optimizations. The hashmap uses a simple bucket storage ystem.

### 3. **No real Object-Oriented Programming (OOP)**

- **Reasoning:** The project avoids the use of OOP principles and classes to maintain a simpler, more procedural codebase that focuses on performance. This decision makes the code less complex and easier to understand for a project focused on algorithms and file handling. This also avoids the memory overhead of classes. In hindsight i could have made the ui using classes.
- **Implementation:** Namespaces are used to organize the project into logical components (e.g., a namespace for the hashmap functions and another for the file I/O operations).

### 4. **Bucket file storage for optimization**

- **Reasoning:** Buckets are used to increase the performance of the application.
- **Implementation:** We calculate our key offsets for the index file using our bucket functions in the hash file.

### 5. **Index and Database file Storage Format**

- **Reasoning:** I choose this format so I could use technically 0(1) time complexity and it is easy to see.
- **Implementation:** I made it so that the index file holds the key and the offset, while the database file holds the key and the values.

### 6. **SDL-Based User Interface**

- **Reasoning:** SDL is used to create a cross-platform user interface for interacting with the database. SDL allows for a simple, efficient interface and ensures compatibility with various platforms.
- **Implementation:** The user interface is built using SDL to provide an intuitive front-end for adding, deleting, and searching for key-value pairs. It will also display statistics and status updates, such as the number of items in the database and cache hits.

### 7. **Minimal Main File**

- **Reasoning:** To maintain a clean and simple entry point, the `main.cpp` file will be minimal, delegating the majority of logic to specialized files such as `db_service.cpp` for handling the database functions and SDL initialization.
- **Implementation:** `main.cpp` will only call the db_service to run the operations.

### 8. **Testability and Modularity**

- **Reasoning:** The project is designed to be modular and testable to facilitate easier maintenance and debugging. Core functionality is separated into logical modules, such as the hashmap, file I/O, and UI components.
- **Implementation:** Each component of the project is isolated into separate files and functions, allowing for unit testing and more focused development.

### 9. **No External Dependencies**

- **Reasoning:** To maintain a lightweight project that focuses on core algorithms, no external libraries (except SDL for the UI) are used. This decision simplifies the build process and keeps the project portable. Some third part libraries tend to carry bloat with them.
- **Implementation:** The project only depends on the SDL library for the UI, while the rest of the functionality is implemented using standard C++ libraries.

## Future Improvements

- **LSM** Implement for performance optimization.
- **Concurrency:** Implement multi-threading or concurrency support to allow for faster operations, particularly for write-heavy workloads.
- **Crash Recovery:** Add robust crash recovery mechanisms to ensure data integrity in the event of a program or system crash.
- **Compression:** Implement data compression techniques to optimize storage space and reduce I/O overhead.
- **Binary format** Implement for permormance optimization.
