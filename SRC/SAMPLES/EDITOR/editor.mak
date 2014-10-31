.SUFFIXES: .cpp .obj .rc .res

.all: Editor.exe

.cpp.obj:
	@echo " Compile::C++ Compiler "
	icc.exe /I. /Iinclude /I..\..\pmclass\include /I..\..\nvclass\include /Fi /Si /O /Gm /Fo"%|dpfF.obj" /C %s

.rc.res:
	@echo " Compile::Resource Compiler "
	rc.exe -r %s %|dpfF.RES

Editor.exe: \
	code\prodinfo.obj \
	code\tbartop.obj \
	code\editor.obj \
	code\finddlg.obj \
	editor.res \
	..\..\pmclass\pmclass.lib \
	..\..\nvclass\nvclass.lib \
	code\editor.def
	@echo " Link::Linker "
	@echo " Bind::Resource Bind "
	icc.exe @<<
	 /B" /pmtype:pm"
	 /FeEditor.exe
	 ..\..\pmclass\pmclass.lib
	 ..\..\nvclass\nvclass.lib
	 code\editor.def
	 code\prodinfo.obj
	 code\tbartop.obj
	 code\editor.obj
	 code\finddlg.obj
<<
	rc.exe editor.res Editor.exe

code\prodinfo.obj: code\prodinfo.cpp

code\finddlg.obj: code\finddlg.cpp

code\editor.obj: code\editor.cpp

code\tbartop.obj: code\tbartop.cpp

editor.res: editor.rc rc.hpp

