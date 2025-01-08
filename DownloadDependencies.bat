@echo off
color 0F
setlocal enabledelayedexpansion

where git >nul 2>nul
if %errorlevel% neq 0 goto :GitNotInstalled

REM Check if Python and pip are installed
where python >nul 2>nul
if %errorlevel% neq 0 goto :PythonNotInstalled

where pip >nul 2>nul
if %errorlevel% neq 0 goto :PipNotInstalled

REM Check if gdown is installed
python -c "import gdown" >nul 2>nul
if %errorlevel% neq 0 goto :GdownNotInstalled

REM All dependencies are installed, proceed with the remaining commands
set "repoURL=https://github.com/SleepToFightAnotherDay/Libraries.git"
set "tempFolder=%TEMP%\tempRepo"

REM Check if AeonCore\dependencies exists, and remove it if it does
powershell -command "Write-Host 'Checking if AeonCore\dependencies exists' -ForegroundColor Cyan"
echo.
IF EXIST "AeonCore\dependencies" (
	powershell -command "Write-Host 'File exists, clearing directory...' -ForegroundColor Cyan"
	echo.
    rd /s /q "AeonCore\dependencies"
)

REM Create a temporary directory for cloning
mkdir "%tempFolder%\Dep"
mkdir "%tempFolder%\Debug"
mkdir "%tempFolder%\Release"

REM Clone the repository into the temporary folder
powershell -command "Write-Host 'Downloading Dependencies...' -ForegroundColor Yellow"
echo.
git clone %repoURL% "%tempFolder%\Dep"

REM Move the contents from the temp folder to the destination folder
xcopy "%tempFolder%\Dep" "AeonCore\" /E /Y

REM Download the physxGPULib using gdown
echo.
powershell -command "Write-Host 'Downloading PhysXGpu_64.dll...' -ForegroundColor Yellow"
gdown --id 16hN5f2YfNAAciYvPkwGlsidLmrrl2mbf -O "%tempFolder%\Debug\PhysXGpu_64.dll"
gdown --id 1N6LEsaTV53hZAZgA4JYPOt_6MuGbNTkD -O "%tempFolder%\Release\PhysXGpu_64.dll"

REM Move the downloaded file to the destination folder
move "%tempFolder%\Debug\PhysXGpu_64.dll" "AeonCore\dependencies\physx\bin\Debug"
move "%tempFolder%\Release\PhysXGpu_64.dll" "AeonCore\dependencies\physx\bin\Release"

REM Clean up temporary directory
rmdir /S /Q "%tempFolder%\Dep"
rmdir /S /Q "%tempFolder%\Debug"
rmdir /S /Q "%tempFolder%\Release"

echo Done.
pause
exit /b

:GitNotInstalled
powershell -command "Write-Host 'Git is not installed. Please download and install Git from https://git-scm.com/' -ForegroundColor Red"
pause
exit /b

:PythonNotInstalled
powershell -command "Write-Host 'Python is not installed. Please download and install Python from https://www.python.org/' -ForegroundColor Red"
pause
exit /b

:PipNotInstalled
powershell -command "Write-Host 'pip is not installed. Please install pip before proceeding. To download, run install pip.' -ForegroundColor Red"
pause
exit /b

:GdownNotInstalled
powershell -command "Write-Host 'gdown is not installed. Please run ''pip install gdown'' to install it.' -ForegroundColor Red"
pause
exit /b
