# Memory-Efficient Versioned File Indexer

## Overview of the System
This application is a **C++ command-line tool** designed to analyze log or text files by building a **versioned word-frequency index**. The program is implemented using **Object-Oriented Programming (OOP)** principles and processes files efficiently using **buffered file reading**. All interactions with the application are performed through **terminal commands using command-line arguments**.

## Basic Structure of the Application

### Buffered Reader
The system reads files in chunks using a configurable **buffer size (256–1024 KB)**. This improves performance when processing large log files.

### Tokenizer
The tokenizer processes the file content and extracts **alphanumeric words**, converting them to **lowercase** to ensure case-insensitive word counting.

### Versioned Index
The application maintains a **version-based word frequency index** where each version corresponds to a specific file. This allows efficient querying and comparison between different versions.

## Features and Functionalities
- **Word Query** – Finds the frequency of a specific word in a file version.
- **Top-K Query** – Displays the  K most frequent words in a file.
- **Diff Query** – Compares the frequency of a word between two file versions.
- Efficient file processing using **buffered reading**.
- Case-insensitive word matching.

## Usage

Compile the program using a C++ compiler:

    g++ -std=c++17 main.cpp -o 240650_devarsh

Run the program using command-line arguments:

### Word Query

    .\240650_devarsh.exe --file <file> --version <version> --buffer <size> --query word --word <word>

### Top-K Query

    .\240650_devarsh.exe --file <file> --version <version> --buffer <size> --query top --top <K>

### Diff Query

    .\240650_devarsh.exe --file1 <file1> --version1 <v1> --file2 <file2> --version2 <v2> --buffer <size> --query diff --word <word>
