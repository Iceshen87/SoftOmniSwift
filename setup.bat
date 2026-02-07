&commat;echo off

setlocal EnableExtensions EnableDelayedExpansion

for /f "delims=" %%A in ('echo prompt $E ^| cmd') do set "ESC=%%A"
set "RED=%ESC%[31m"
set "YELLOW=%ESC%[33m"
set "GREEN=%ESC%[32m"
set "CYAN=%ESC%[36m"
set "GRAY=%ESC%[90m"
set "NO_COLOR=%ESC%[0m"

if "%ESC%"=="" (
  set "RED="
  set "YELLOW="
  set "GREEN="
  set "CYAN="
  set "GRAY="
  set "NO_COLOR="
)

set "SCRIPT_DIR=%~dp0"
set "SUBPROJECTS_DIR=%SCRIPT_DIR%subprojects"

call :info "CHECKING IF THE MESON BUILD TOOL IS INSTALLED"
where /q meson
if errorlevel 1 (
  call :info "MESON BUILD SYSTEM IS NOT INSTALLED"
  call :info "ATTEMPTING TO INSTALL MESON"
  call :info "CHECKING IF PYTHON'S PIP TOOL IS INSTALLED AS pip3"

  where /q pip3
  if errorlevel 1 (
    call :error "PYTHON'S PIP TOOL IS NOT INSTALLED AS pip3"
    call :error "ABORTING ALL"
    exit /b 10
  )

  call :ok "PYTHON'S PIP TOOL IS INSTALLED"
  call :info "INSTALLING MESON THROUGH PIP"
  pip3 install --user meson
  if errorlevel 1 (
    call :error "PIP FAILED TO INSTALL MESON...ABORTING"
    exit /b 11
  )

  where /q meson
  if errorlevel 1 (
    call :error "MESON FAILED TO INSTALL (MESON STILL NOT ON PATH)...ABORTING"
    call :warning "You may need to reopen the terminal or add your Python user scripts directory to PATH."
    exit /b 11
  ) else (
    call :ok "MESON IS INSTALLED SUCCESSFULLY"
  )
) else (
  call :ok "MESON IS INSTALLED ON THE SYSTEM"
)

call :debug "CALCULATING PATH TO SUBPROJECTS SUB-DIRECTORY..."
call :info "CHECKING IF SUBPROJECTS DIRECTORY '%SUBPROJECTS_DIR%' EXISTS..."
if not exist "%SUBPROJECTS_DIR%\" (
  call :info "SUBPROJECTS DIRECTORY DOES NOT EXIST"
  call :info "CREATING SUBPROJECTS DIRECTORY"
  mkdir "%SUBPROJECTS_DIR%" >nul 2>&1
  if not exist "%SUBPROJECTS_DIR%\" (
    call :error "COULD NOT CREATE '%SUBPROJECTS_DIR%' DIRECTORY...EXITING."
    exit /b 3
  )
  call :ok "SUBPROJECTS DIRECTORY CREATED"
) else (
  call :info "SUBPROJECTS DIRECTORY EXISTS"
)

call :info "SETTING UP GTEST..."
pushd "%SCRIPT_DIR%" >nul
meson wrap install gtest
if errorlevel 1 (
  popd >nul
  call :error "FAILED TO INSTALL GTEST WRAP"
  exit /b 4
)

call :info "CHECKING IF GTEST INSTALLED IN DIRECTORY"
set "FOUND=FALSE"
for /d %%D in ("%SUBPROJECTS_DIR%\*") do (
  echo %%~nxD | findstr /i "googletest" >nul
  if not errorlevel 1 (
    set "FOUND=TRUE"
    call :ok "GTEST FOUND"
    goto :gtest_done
  )
)

:gtest_done
if /i "!FOUND!"=="FALSE" (
  popd >nul
  call :error "GTEST NOT FOUND"
  call :error "ABORTING..."
  exit /b 4
)

call :info "INVOKING MESON CONFIGURATION"
meson setup buildDir --reconfigure
if not errorlevel 1 (
  popd >nul
  call :ok "ALL SETUP"
  call :ok "HAPPY CODING!"
  exit /b 0
)

popd >nul
call :error "SOMETHING FAILED :("
call :warning "MAYBE THE LOGS COULD PROVE USEFUL :)"
exit /b 20

:print
set "C=%~1"
set "L=%~2"
set "M=%~3"
echo %C% %L%: %M% %NO_COLOR%
exit /b 0

:error
call :print "%RED%" "ERROR" "%~1"
exit /b 0

:warning
call :print "%YELLOW%" "WARNING" "%~1"
exit /b 0

:info
call :print "%CYAN%" "INFO" "%~1"
exit /b 0

:debug
call :print "%GRAY%" "DEBUG" "%~1"
exit /b 0

:ok
call :print "%GREEN%" "OK" "%~1"
exit /b 0