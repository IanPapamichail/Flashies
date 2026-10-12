@echo off
REM 1) Compile the C++ code (optional if you already have an .exe)
g++ -std=c++17 -I . -o flashcards main.cpp Card.cpp Deck.cpp User.cpp

REM 2) Start the backend console app (hidden or not)
start flashcards.exe

REM 3) Start a Python HTTP server (this requires python installed and on PATH)
start cmd /k "python -m http.server 8080"

REM 4) Open the default browser to http://localhost:8080/index.html
start "" "http://localhost:8080/index.html"











