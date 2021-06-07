//------------------------------------------------------------------------
//  Readme.txt
//------------------------------------------------------------------------
This project stationery is designed to get you up and running
quickly with CodeWarrior for MC9S12XS128.
It is set up for the selected CPU and target connection,
but can be easily modified.

Sample code for the following language(s) is at your disposal:

The wizard has prepared CodeWarrior target(s) with the connection methods
of your choice:

Additional connections can be chosen in the simulator/debugger,
use the menu Component > Set Target.

//------------------------------------------------------------------------
//  Getting Started
//------------------------------------------------------------------------
To build/debug your project, use the menu Project > Debug or press F5.
This will open the simulator/debugger.
Press again F5 in the debugger (or menu Run > Start/Continue) to start
the application. The menu Run > Halt or F6 stops the application.
In the debugger menu Component > Open you can load additional components.

//------------------------------------------------------------------------
// Project structure
//------------------------------------------------------------------------
The project generated contains various files/folders:
- readme.txt: this file
- Sources: folder with the application source code
- Prm:
   - burner.bbl file to generate S-Records
- Libraries: needed library files (ANSI, derivative header/implementation
  files)
- Debugger Project File: contains a .ini file for the debugger for each
  connection
- Debugger Cmd Files: contains sub-folders for each connection with
  command files

//------------------------------------------------------------------------
//  Adding your own code
//------------------------------------------------------------------------
Once everything is working as expected, you can begin adding your own code
to the project. Keep in mind that we provide this as an example of how to
get up and running quickly with CodeWarrior. There are certainly other
ways to handle interrupts and set up your linker command file. Feel free
to modify any of the source files provided.

//  Simulator/Debugger: Additional components
//------------------------------------------------------------------------
In the simulator/debugger, you can load additional components. Try the
menu Component > Open.

//------------------------------------------------------------------------
//  Additional documentation
//------------------------------------------------------------------------
Read the online documentation provided. Use in CodeWarrior IDE the menu 
Help > CodeWarrior Help.

//------------------------------------------------------------------------
//  Contacting Metrowerks
//------------------------------------------------------------------------
For bug reports, technical questions, and suggestions, please use the
forms installed in the Release_Notes folder and send them to:
cw_support@freescale.com
