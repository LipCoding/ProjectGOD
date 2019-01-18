cd..
cd..

xcopy /s /y /d .\Engine\Include\*.h .\EngineRef\Include\
xcopy /s /y /d .\Engine\Include\*.inl .\EngineRef\Include\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin_Debug\*.* .\EngineRef\Bin_Debug\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Texture\*.* .\MonsterForest\Bin\Texture\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Texture\*.* .\MonsterForest\Bin64\Texture\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Texture\*.* .\MonsterForest\Bin64_Debug\Texture\

xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Mesh\*.* .\MonsterForest\Bin\Mesh\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Mesh\*.* .\MonsterForest\Bin64\Mesh\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Mesh\*.* .\MonsterForest\Bin64_Debug\Mesh\

xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Data\*.* .\MonsterForest\Bin\Data\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Data\*.* .\MonsterForest\Bin64\Data\
xcopy /s /y /d /exclude:Out.txt .\MonsterForest\Bin_Debug\Data\*.* .\MonsterForest\Bin64_Debug\Data\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin_Debug\Shader\*.* .\Engine\Bin\Shader\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin_Debug\Shader\*.* .\Engine\Bin64\Shader\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin_Debug\Shader\*.* .\Engine\Bin64_Debug\Shader\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin\*.* .\EngineRef\Bin\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64_Debug\*.* .\EngineRef\Bin64_Debug\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64\*.* .\EngineRef\Bin64\

xcopy /s /y /d .\EngineRef\Bin_Debug\*.* .\MonsterForest\Bin_Debug\
xcopy /s /y /d .\EngineRef\Bin\*.* .\MonsterForest\Bin\
xcopy /s /y /d .\EngineRef\Bin64\*.* .\MonsterForest\Bin64\
xcopy /s /y /d .\EngineRef\Bin64_Debug\*.* .\MonsterForest\Bin64_Debug\