@ECHO OFF

SET CC=clang
SET FLAGS=-Wall -Wextra -Wpedantic -Werror -std=c11

%CC% -c %FLAGS% -o main.o Project\main.c
%CC% -o Project\main main.o -lm

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