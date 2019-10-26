BRZOPTS
=======

 1. Summary
 2. Prerequisites
 3. Building
 4. Installing
 5. Examples
 6. Copyright
 

SUMMARY
=======

This is a C++ library for command line argument parsing. It sticks to the  
view that a command line is made up of a list of key/value pairs and an   
optional list of singletons. The key in the K/V pair is always prefixed by  
either one dash or two dashes.


PREREQUISITES
=============

 This project requires:
  * Cross-platform Make (CMake) v2.6.2+
  * GNU Make or equivalent.
  * GCC or an alternative, reasonably conformant C++ compiler.


BUILDING
========
 
This project uses the Cross-platform Make (CMake) build system. However, we  
have conveniently provided a wrapper configure script and Makefile so that  
the typical build invocation of "./configure" followed by "make" will work.  
For a list of all possible build targets, use the command "make help".


INSTALLING 
==========

 Once the project has been built, execute "sudo make install".


EXAMPLES
========

    // See the include file brzopts.h for the API  
	//  
	void App::parse(int argc, char **argv, const Option otpions[])
	{  
	    brzopts::Options cmdargs;

	#if defined( _GTK_ )  
		Application::init(   argc, &argv );
	#endif  

		if (cmdargs.parse( argc, argv, options )) {  
			const std::list< std::pair<std::string, std::string> >& kv = cmdargs.getKV();  
			const std::list<std::string>& arglist = cmdargs.getArgs();  
		}  
	}  


COPYRIGHT
=========

  Brzopts is copyright (c) 2019 Pierre Innocent, All rights reserved  
  Licensed through the GNU General Public License V3.0.

