# Scalable Client-Server File Management System

## Overview
This project is a C-based client-server application for Linux that allows clients to request files from a server. It includes auto-scaling with a mirror server, file search capabilities by size, date, and extension, and command syntax validation.

## Prerequisites
- Linux environment
- GCC (GNU Compiler Collection)

## Installation
1. Clone the Repository:
   ```bash
   git clone https://github.com/yourusername/your-repo.git
   cd your-repo
   ```
2. Compile the Server and Client:
   ```bash
   gcc -o server server.c
   gcc -o client client.c
   ```

## Usage
1. Run the Server:
   ```bash
   ./server
   ```
   The server will start and listen for client connections.

2. Run the Client:
   ```bash
   ./client
   ```
   Follow the prompts to request files or perform searches.

## Features
- Auto-scaling with a mirror server
- File search by size, date, and extension
- Command syntax validation

## Notes
- Ensure that the server is running before starting the client.
- Adjust configurations in the source code if necessary to match your environment.
