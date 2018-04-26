##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lsm-project
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/BEST/Documents/lsm_phase2
ProjectPath            :=C:/cygwin64/home/BEST/cs265/project/lsm-project
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=BEST
Date                   :=27/04/2018
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="lsm-project.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/test_range.c$(ObjectSuffix) $(IntermediateDirectory)/fileutil.c$(ObjectSuffix) $(IntermediateDirectory)/lsm.c$(ObjectSuffix) $(IntermediateDirectory)/node.c$(ObjectSuffix) $(IntermediateDirectory)/hasht.c$(ObjectSuffix) $(IntermediateDirectory)/bloom.c$(ObjectSuffix) $(IntermediateDirectory)/test.c$(ObjectSuffix) $(IntermediateDirectory)/hashlib.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/test_range.c$(ObjectSuffix): test_range.c $(IntermediateDirectory)/test_range.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/test_range.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_range.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_range.c$(DependSuffix): test_range.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_range.c$(ObjectSuffix) -MF$(IntermediateDirectory)/test_range.c$(DependSuffix) -MM test_range.c

$(IntermediateDirectory)/test_range.c$(PreprocessSuffix): test_range.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_range.c$(PreprocessSuffix) test_range.c

$(IntermediateDirectory)/fileutil.c$(ObjectSuffix): fileutil.c $(IntermediateDirectory)/fileutil.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/fileutil.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fileutil.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fileutil.c$(DependSuffix): fileutil.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fileutil.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fileutil.c$(DependSuffix) -MM fileutil.c

$(IntermediateDirectory)/fileutil.c$(PreprocessSuffix): fileutil.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fileutil.c$(PreprocessSuffix) fileutil.c

$(IntermediateDirectory)/lsm.c$(ObjectSuffix): lsm.c $(IntermediateDirectory)/lsm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/lsm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lsm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lsm.c$(DependSuffix): lsm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lsm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lsm.c$(DependSuffix) -MM lsm.c

$(IntermediateDirectory)/lsm.c$(PreprocessSuffix): lsm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lsm.c$(PreprocessSuffix) lsm.c

$(IntermediateDirectory)/node.c$(ObjectSuffix): node.c $(IntermediateDirectory)/node.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/node.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/node.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/node.c$(DependSuffix): node.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/node.c$(ObjectSuffix) -MF$(IntermediateDirectory)/node.c$(DependSuffix) -MM node.c

$(IntermediateDirectory)/node.c$(PreprocessSuffix): node.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/node.c$(PreprocessSuffix) node.c

$(IntermediateDirectory)/hasht.c$(ObjectSuffix): hasht.c $(IntermediateDirectory)/hasht.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/hasht.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hasht.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hasht.c$(DependSuffix): hasht.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hasht.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hasht.c$(DependSuffix) -MM hasht.c

$(IntermediateDirectory)/hasht.c$(PreprocessSuffix): hasht.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hasht.c$(PreprocessSuffix) hasht.c

$(IntermediateDirectory)/bloom.c$(ObjectSuffix): bloom.c $(IntermediateDirectory)/bloom.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/bloom.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bloom.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bloom.c$(DependSuffix): bloom.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bloom.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bloom.c$(DependSuffix) -MM bloom.c

$(IntermediateDirectory)/bloom.c$(PreprocessSuffix): bloom.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bloom.c$(PreprocessSuffix) bloom.c

$(IntermediateDirectory)/test.c$(ObjectSuffix): test.c $(IntermediateDirectory)/test.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/test.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test.c$(DependSuffix): test.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test.c$(ObjectSuffix) -MF$(IntermediateDirectory)/test.c$(DependSuffix) -MM test.c

$(IntermediateDirectory)/test.c$(PreprocessSuffix): test.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test.c$(PreprocessSuffix) test.c

$(IntermediateDirectory)/hashlib.c$(ObjectSuffix): hashlib.c $(IntermediateDirectory)/hashlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/hashlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hashlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hashlib.c$(DependSuffix): hashlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hashlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hashlib.c$(DependSuffix) -MM hashlib.c

$(IntermediateDirectory)/hashlib.c$(PreprocessSuffix): hashlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hashlib.c$(PreprocessSuffix) hashlib.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/cygwin64/home/BEST/cs265/project/lsm-project/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


