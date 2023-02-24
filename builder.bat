@echo OFF

set BUILD_DIR=%~dp0build
set C_STD=c2x
set SOURCE_FILE=%1
set BUILT_FILE=%BUILD_DIR:"=%\%SOURCE_FILE:.c=.exe%

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

clang ^
  -Wall -Werror -Wpedantic -Wdeprecated -std=%C_STD% ^
  "%~dp0%SOURCE_FILE%" -o "%BUILT_FILE%" ^
  ".\src\*.c" ^
  -I ".\src\headers" ^
  -lcrypto -lssl -lws2_32

@echo Build finished. Executable located at "%BUILT_FILE%".
@pause
