@echo off
setlocal enabledelayedexpansion

:: === Settings ===
set QT_DIR=C:\Qt\6.5.3\msvc2019_64
set BUILD_DIR=cmake-build-release
set DEPLOY_DIR=dist

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

:: === Create folder  ===
if not exist "%DEPLOY_DIR%" (
    mkdir "%DEPLOY_DIR%"
)

:: === Copy EXE in dist ===
echo [INFO] Copy %EXE_NAME% to %DEPLOY_DIR%
copy /Y "%EXE_PATH%" "%DEPLOY_DIR%"

:: === Start windeployqt in folder dist ===
echo [INFO] Packing with windeployqt...
pushd "%DEPLOY_DIR%"
"%QT_DIR%\bin\windeployqt.exe" ^
  --release ^
  --no-translations ^
  --no-system-d3d-compiler ^
  --compiler-runtime ^
  "%EXE_NAME%"
popd

echo [READY] The file is packaged in %DEPLOY_DIR%
pause
endlocal
