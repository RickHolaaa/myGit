# myGit — Version Control System (Academic Project)

This project was developed as part of a university project simulating a basic version control system similar to Git.

## 👥 Authors

- **Essabri Youness** — Student ID: 28705780
- **Gao Rick** — Student ID: 21110625

## 📄 Project Description

The goal of this project was to implement, in the C programming language, a simplified version control system inspired by Git.  
It provides the following features:

- **Repository initialization**
- **File tracking (staging)**
- **Committing changes**
- **Branch creation and management**
- **Branch switching (checkout)**
- **Merging branches with manual conflict resolution**
- **Commit history browsing**

The project is modularized to ensure clean code organization and maintainability.

## 🛠 Technologies Used

- **Language**: C
- **Compiler**: GCC
- **IDE**: Visual Studio Code
- **Platform**: Linux
- **Standard Libraries**:
  - `dirent.h` (directory operations)
  - `stdio.h` (input/output)
  - `stdlib.h` (memory allocation, conversions, etc.)
  - `string.h` (string manipulation)
  - `unistd.h` (system calls)

## 🔧 Code Structure

The project is divided into multiple modules:

- **FonctionHash**: hashing functions used to compute hashes for blobs, worktrees, and commits.
- **listCell**: linked list implementation to handle collections of files.
- **ManipBase**: file and directory manipulation (creation, deletion, copying, conflict detection, etc.).
- **commit**: structures and functions to create and manage commits.
- **branchutils**: branch management functions (creation, listing, etc.).
- **checkout**: allows switching branches or restoring previous commits.
- **merge**: manages branch merging with manual conflict resolution.
- **myGit**: main entry point, simulating Git-like commands.
- **mainX.c**: test files for the various features.

## 🚀 Example Usage

### Initialize Repository
```bash
./myGit init
```

### Add files to staging
```bash
./myGit add file.txt file1.txt
```

### Commit
```bash
./myGit commit master -m "first commit"
```

### Create a Branch
```bash
./myGit branch Feature1
```
