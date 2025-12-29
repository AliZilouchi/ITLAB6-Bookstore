# ITLAB6-Bookstore — Arash Library (CLI)

GitHub repository: [https://github.com/AliZilouchi/ITLAB6-Bookstore](https://github.com/AliZilouchi/ITLAB6-Bookstore)

A **C++17** command-line library manager: add, list, search, and delete books with **persistent file storage**.

---

## Features

* **Add book** (title, author, year)
* **List books**
* **Search** by partial text (case-insensitive) in title/author
* **Delete** by **ID** or **exact title**
* **Persistent storage** in `books.json`

---

## Repository Structure

```
.
├── README.md
├── books.json
├── include
│   ├── Book.hpp
│   ├── CliUtil.hpp
│   ├── Library.hpp
│   ├── StorageJson.hpp
│   └── StringUtil.hpp
├── input.txt
└── src
    ├── CliUtil.cpp
    ├── Library.cpp
    ├── StorageJson.cpp
    ├── StringUtil.cpp
    └── main.cpp
```

---

## Prerequisites

* Linux / WSL (or any OS with a C++17 compiler)
* `g++` with C++17 support

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install g++
```

---

## Build

From the repository root (where `src/` and `include/` exist):

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/*.cpp -o library
```

---

## Run

Interactive:

```bash
./library
```

Run automatically using the provided input file:

```bash
./library < input.txt
```

---

## Data Persistence (`books.json`)

* The program **loads** from `books.json` on startup (if it exists).
* The program **saves** updates back to `books.json` (after add/delete and/or on exit).

Reset to an empty library:

```bash
rm -f books.json
```

---

## Git/GitHub Workflow Evidence

This repository includes:

* multiple branches (feature branches + docs conflict branches)
* pull requests
* issues linked to pull requests
* a controlled merge conflict that was created and resolved
* a project board for tracking work

### Branches used

* `AidinKazemi-main`
* `AidinKazemi-CliUtil`
* `chore/project-structure`
* `feature/storage-json`
* `feature/library-core`
* `docs/conflict-a`
* `docs/conflict-a-1`
* `docs/conflict-b`

### Repository links

* Branches: [https://github.com/AliZilouchi/ITLAB6-Bookstore/branches](https://github.com/AliZilouchi/ITLAB6-Bookstore/branches)
* Pull Requests: [https://github.com/AliZilouchi/ITLAB6-Bookstore/pulls](https://github.com/AliZilouchi/ITLAB6-Bookstore/pulls)
* Issues: [https://github.com/AliZilouchi/ITLAB6-Bookstore/issues](https://github.com/AliZilouchi/ITLAB6-Bookstore/issues)
* Projects (Project Board): [https://github.com/AliZilouchi/ITLAB6-Bookstore/projects](https://github.com/AliZilouchi/ITLAB6-Bookstore/projects)

---

## Team

* **Aidin Kazemi** — 81010561
* **Ali Zilouchi** — 810101560
