@echo off
setlocal enabledelayedexpansion

:: Define source and output directories
set "SOURCE_DIR=shaders"
set "OUTPUT_DIR=shaders\spirv"

:: Create output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

:: Loop through all .hlsl files in the source directory
for /r "%SOURCE_DIR%" %%F in (*.hlsl) do (
    set "FILENAME=%%~nxF"
    set "FILEPATH=%%F"
    
    :: Skip files that are already inside the output directory
    echo "%%F" | findstr /i "%OUTPUT_DIR%" >nul
    if errorlevel 1 (
        
        set "STAGE="
        set "ENTRY=main"
        
        :: Detect shader stage precisely using the end of the filename
        echo !FILENAME! | findstr /i "\.vs\.hlsl$" >nul && set "STAGE=vs_6_0"
        echo !FILENAME! | findstr /i "\.ps\.hlsl$" >nul && set "STAGE=ps_6_0"
        echo !FILENAME! | findstr /i "\.cs\.hlsl$" >nul && set "STAGE=cs_6_0"
        
        :: Only compile if a valid shader extension was recognized
        if not "!STAGE!"=="" (
            echo Compiling: !FILENAME! [!STAGE!] -> %OUTPUT_DIR%\%%~nF.spv
            
            :: Run DXC compiler (strips the final .hlsl extension for the output name)
            dxc.exe -spirv -T !STAGE! -E !ENTRY! "%%F" -Fo "%OUTPUT_DIR%\%%~nF.spv"
            
            if !errorlevel! neq 0 (
                echo [ERROR] Failed to compile !FILENAME!
            )
        ) else (
            echo [SKIP] !FILENAME! does not match .vs.hlsl, .ps.hlsl, or .cs.hlsl
        )
    )
)

echo ----------------------------------------
echo Compilation complete.
pause
