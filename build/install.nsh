Function func
  nsProcessW::_FindProcess "ClassSystem.exe" $R0
  Pop $0
  StrCmp $0 "0" 0 +5
  nsProcessW::_KillProcess "ClassSystem.exe"
  Sleep 2000
  Call func
  
FunctionEnd

!macro customHeader
  SilentInstall silent
!macroend


!macro customInit
  Call func
  StrCpy $INSTDIR "D:\ClassSystem\"

!macroend

Function .onInstSuccess
Sleep 5000
  ExecWait "D:\ClassSystem\ClassSystem.exe"
FunctionEnd