@echo off
setlocal enabledelayedexpansion

:: === Settings ===
set QT_DIR=C:\Qt\6.5.3\msvc2019_64
set BUILD_DIR=cmake-build-release

:: === Get executable file name ===
if "%~1"=="" (
    set /p EXE_NAME=Enter the name of the .exe-file:
) else (
    set EXE_NAME=%~1
)

:: === Checking the file existance ===
set EXE_PATH=%BUILD_DIR%\%EXE_NAME%

if not exist "%EXE_PATH%" (
    echo [ERROR] File not found: %EXE_PATH%
    pause
    exit /b 1
)

:: === Start windeployqt ===
echo [INFO] Packing with windeployqt...
"%QT_DIR%\bin\windeployqt.exe" ^
  --release ^
  --no-translations ^
  --no-system-d3d-compiler ^
  --compiler-runtime ^
  "%EXE_PATH%"

echo [READY] Packing is complete.
pause
endlocal
