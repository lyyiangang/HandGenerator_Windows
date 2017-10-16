COMPILE GUIDE FOR HANDGENERATOR - WINDOWS SYSTEMS

Alvise Memo, 20/10/2015
Multimedia Technology and Telecommunications Laboratory, University of Padova

HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.


0) DEPENDENCIES REQUIRED:

	To use the library, is required to LINK the following libraries
		FreeGLUT	3.0.0		http://freeglut.sourceforge.net/
		Assimp		3.1.1		http://assimp.sourceforge.net/
		OpenGL		
	The compiled libraries are also provided with the code, so NO NEED for any action if you dont need to modify them.


1) USE COMPILED LIBRARY
	


	1.1) USE COMPILED TEST

		To run the test simply run

		HandGenerator_Test.exe

		located in the bin/ folder
		(To use line-parameters read the guide provided with the software)



	1.2) COMPILE CUSTOM TEST

		Link the library .dll and it's .lib for the symbols.
	

2) COMPILE CUSTOM LIBRARY


	With CMake and the provided CMakeList.txt, compilation is already setup, no need for additional setups.
