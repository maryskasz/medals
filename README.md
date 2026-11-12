# Medals

## Overview

**Medals** is a C++ command-line application designed to process and query medal statistics across various competitions. 

---

## Terminology & Formatting Rules

| Concept | Description |
| :--- | :--- |
| **Word** | A non-empty string consisting of non-whitespace characters. |
| **Question Mark** | A `word` containing only a single question mark character: `?`. |
| **Medal** | A word containing a single digit `'1'`, `'2'`, or `'3'`, representing Gold, Silver, or Bronze medals respectively. |
| **Year** | A word representing a base-10 integer in the range `[1800, 59999]` with no leading zeros (e.g., `1996`, `2024`). |
| **Whitespace** | A non-empty sequence containing exclusively space (`' '`) or tab (`'\t'`) characters. |
| **Competition** | A non-empty sequence of `words` containing only uppercase or lowercase English letters (`[a-zA-Z]`), separated by `whitespace`. |
| **Line** | A sequence of words separated by whitespace and terminated by a newline character (`\n`). Whitespace may also occur at the beginning or end of a line. |

> **Note:** As an exception, if the final line of the input data does not end with a newline character (`\n`), it is still considered a valid line as long as its contents are valid.

---

## Program Description

The program reads data line-by-line from standard input (`std::cin`). Program execution for each line depends on whether the first word is a question mark (`?`).

### 1. Data Entry Line (First word is NOT `?`)
A valid data line must contain exactly three pieces of information in **any order**:
- **Medal** (`1`, `2`, or `3`)
- **Year** (`1800` – `59999`)
- **Competition** (Alphabetical string, can span multiple words)

**Action:** The program stores this valid record in memory.

### 2. Query Line (First word IS `?`)
A valid query line starts with `?` followed optionally by 1, 2, or 3 filtering criteria in **any order**:
- **Medal** (Optional)
- **Year** (Optional)
- **Competition** (Optional)

**Action:** The program counts how many previously stored medal entries match **all** provided criteria and prints the integer count followed by a newline (`\n`) to standard output (`std::cout`).

---

## Error Handling

If a line is invalid (incorrect format, invalid characters, out-of-range values, etc.):
1. The program prints the following string to standard error (`std::cerr`):
   ```
   ERROR <line_number>
   ```
   *(Line numbers start at 1).*
2. The invalid line is ignored and no data from it is saved.

---

## How to compile
- **Compilation Command:**
  ```bash
  g++ -Wall -Wextra -O2 -std=c++23 medals.cpp -o medals
  ```
- **Exit Code:** Program must terminate with exit status `0`.