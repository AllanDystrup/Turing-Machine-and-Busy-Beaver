# ===========================Make.ps1 =================================
#  Developer:	Allan Dystrup, Feb. 2026.
#
#  Content:   	A small PowerShell "batch file" to build 
#	      	the Turing and Beaver C-Sharp source files, including
#		their Console and Windows test drivers/applications.
#
#    	The build can be done in the "Developer PowerShell" from MS
#    	Visual Studio (csc in not a recognized compiler in the std console),
#    	-- or you can of course create a project/solution in the VS IDE 
#   	to edit and build the C# applications (with some config bloat).
# ======================================================================

# Change to project dir
cd C:\Users\allan\CLionProjects\TURING\CS


# Clean bin directory
rm ./bin/*.exe
ls


# Build executables 
csc Turing.cs DBG.cs TuringConUI.cs

csc Turing.cs DBG.cs TuringWinUI.cs

csc Turing.cs DBG.cs Beaver.cs


# Xfer to bin directory
cp ./*.exe ./bin/
rm ./*.exe
ls .
ls ./bin

# END C-Sharp build script ==============================================
