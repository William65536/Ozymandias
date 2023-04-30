@ECHO OFF

SET CC=clang
SET FLAGS=-Wall -Wextra -Wpedantic -Werror -std=c11

:: Incrementally compile this

%CC% -c %FLAGS% -o Image.o Project\includes\Image.c

%CC% -c %FLAGS% -o main.o Project\main.c
%CC% -o Project\main Image.o main.o -lm

DEL *.o

Project\main

ECHO: && ECHO: && ECHO:

IF ERRORLEVEL 1 (
    ECHO PROGRAM ERROR %ERRORLEVEL%
) ELSE IF ERRORLEVEL 0 (
    ECHO SUCCESS 0
) ELSE (
    ECHO SYSTEM ERROR %ERRORLEVEL%
)
