@echo off

if "%1"=="" (  echo expected 1 argument. && exit )

if not exist latest ( echo "latest" not found. && exit )

if exist app.asar ( 
  taskkill /f /pid %1 >> debug.log 2>&1
  timeout /t 1 /nobreak > nul

  del /f app.asar >> debug.log 2>&1
)

ren latest app.asar >> debug.log 2>&1

timeout /t 1 /nobreak > nul
cd ..
ClassSystem.exe