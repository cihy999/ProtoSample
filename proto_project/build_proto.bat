@echo off
set PROTOC_EXE=%~dp0..\Plugins\ProtobufBlueprintSp\Source\ThirdParty\libprotobuf\bin\protoc.exe
set IN_DIR=%~dp0protos
set OUT_DIR_CPP=%~dp0..\Source\ProtoSample\Message

echo [Info] �ϥ� protoc �sĶ .proto �� C++ �ɮ�...

for %%F in (%IN_DIR%\*.proto) do (
    echo [�sĶ��] %%~nxF
    %PROTOC_EXE% --proto_path=%IN_DIR% --cpp_out=%OUT_DIR_CPP% %%F
	
	if %errorlevel% neq 0 (
		echo [Error] �sĶ���ѡI
		exit /b 1
	)
)

echo [Success] �w��X�� %OUT_DIR_CPP% ��Ƨ�

pause
