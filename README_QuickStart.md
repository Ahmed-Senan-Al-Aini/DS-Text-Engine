# Simple C++ Text Editor - Quick Start Guide

This guide will help you quickly get the C++ Text Editor up and running. It's designed for beginners who want to compile and run the project as a single executable.

## Prerequisites

To compile and run this project, you need a C++ compiler. We recommend `g++`, which is commonly available on Linux, macOS, and Windows (via MinGW or Cygwin).

## Step-by-Step Guide

### 1. Download the Project Files

First, you need to get the project files. If you have Git installed, you can clone the repository:

```bash
git clone https://github.com/Ahmed-Senan-Al-Aini/Text-Editor.git
cd Text-Editor
```

If you don't have Git, you can download the project as a ZIP file from GitHub and extract it to a folder on your computer.

### 2. Compile the Project

Open your terminal or command prompt, navigate to the project directory (where `main.cpp`, `editor.cpp`, and `editor.h` are located), and use the `g++` compiler to create a single executable file:

```bash
g++ main.cpp editor.cpp -o text_editor
```

*   `g++`: This is the C++ compiler command.
*   `main.cpp editor.cpp`: These are the source code files that the compiler will process.
*   `-o text_editor`: This tells the compiler to create an output file named `text_editor` (or `text_editor.exe` on Windows).

If the compilation is successful, you will see a new file named `text_editor` (or `text_editor.exe`) in your project directory.

### 3. Run the Text Editor

Now that you have the executable file, you can run the text editor:

```bash
./text_editor
```

On Windows, you might need to type:

```bash
.\text_editor.exe
```

## Basic Usage

When the editor starts, you'll see a menu. Here are some common commands:

*   Type `1` and press Enter to insert text.
*   Type `16` and press Enter to save your work to a file.
*   Type `18` and press Enter to exit the editor.

Enjoy using your simple C++ Text Editor!

---
