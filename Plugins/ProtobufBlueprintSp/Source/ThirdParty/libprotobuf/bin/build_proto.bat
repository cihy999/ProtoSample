@echo off
set PROTOC_EXE=protoc.exe
set IN_DIR=%~dp0..\..\..\..\Example
set OUT_DIR_CPP=%~dp0..\..\..\ProtobufBlueprintSp\Private\ProtoGenExample

echo [Info] 使用 protoc 編譯 .proto 成 C++ 檔案...

for %%F in (%IN_DIR%\*.proto) do (
    echo [編譯中] %%~nxF
    %PROTOC_EXE% --proto_path=%IN_DIR% --cpp_out=%OUT_DIR_CPP% %%F
	
	if %errorlevel% neq 0 (
		echo [Error] 編譯失敗！
		exit /b 1
	)
)

echo [Success] 已輸出到 %OUT_DIR_CPP% 資料夾

pause
