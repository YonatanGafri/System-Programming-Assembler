# System Programming Laboratory - Assembler Project (Course 20465)

This repository contains the final project for the "System Programming Laboratory" (20465) course at the Open University of Israel, Spring 2024 semester.

**Grade received: 100** 🎉

## Authors
- Yonatan Gafri
- Nechemia Rosenberg

## About the Project
This project is a custom Assembler implemented in C. It parses assembly language source files (with macros), translates them into machine code, and outputs appropriate object, entry, and external files as defined by the imaginary computer architecture in the course specifications.

## Features
- **Macro Deployment:** Pre-assembler stage that identifies and expands macros.
- **Two-Pass Assembly:** 
  - First pass builds the symbol table and data image.
  - Second pass encodes the instructions and resolves symbols.
- **Error Handling:** Robust syntax and semantic error checking with informative messages.
- **Output Files:** Generates `.ob` (Object), `.ent` (Entries), and `.ext` (Externals) files upon successful assembly.

## How to Compile and Run
Ensure you have `gcc` and `make` installed.

1. **Compile:**
   Use the provided `makefile` to compile the project.
   ```bash
   make
   ```
2. **Run:**
   Pass the names of the assembly files (without the `.as` extension) as arguments.
   ```bash
   ./assembler file1 file2
   ```

## Repository Structure
- `source_code/`: Contains all `.c` source files.
- `header_files/`: Contains all `.h` header files.
- `makefile`: For building the project.
- `tester*`: Various test files used to validate the assembler.
