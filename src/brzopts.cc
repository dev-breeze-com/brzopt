// ----- BEGIN LICENSE BLOCK ----- //
//
// (c) Copyright 1996-2015, Glyph.Cloud, All Rights Reserved.
//
// TSERT.COM MAKES NO REPRESENTATIONS AND EXTENDS NO WARRANTIES, EXPRESS OR
// IMPLIED, WITH RESPECT TO THE SOFTWARE, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY PARTICULAR
// PURPOSE, AND THE WARRANTY AGAINST INFRINGEMENT OF PATENTS OR OTHER
// INTELLECTUAL PROPERTY RIGHTS. THE SOFTWARE IS PROVIDED "AS IS", AND IN NO
// EVENT SHALL TSERT.COM OR ANY OF ITS AFFILIATES BE LIABLE FOR ANY DAMAGES,
// INCLUDING ANY LOST PROFITS OR OTHER INCIDENTAL OR CONSEQUENTIAL DAMAGES
// RELATING TO THE USE OF THIS SOFTWARE.
//
// ----- END LICENSE ----- //
//
#include "brzopts.h"

//------------------------------- Namespaces ----------------------------------

using namespace glyph_cloud;
using namespace brzopts;

//-------------------------------- Functions ---------------------------------

//-----------------------------------------------------------------------------
Options::Options(const std::string& version, const std::string& copyright)
//-----------------------------------------------------------------------------
{
	_version = version;
	_copyright = copyright;
}

//-----------------------------------------------------------------------------
Options::~Options() {}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::isOk() const { return (_flags & brzopts::Ok_arg) > 0; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::exitNow() const { return (_flags & brzopts::Exit_arg) > 0; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::urlFailed() const { return (_flags & brzopts::UrlFail_arg) > 0; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::argsFailed() const { return (_flags & brzopts::ArgFail_arg) > 0; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::initFailed() const { return (_flags & brzopts::InitFail_arg) > 0; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool Options::isnumber(const std::string& value) const
//-----------------------------------------------------------------------------
{
	for (const auto& chr: value) {
		if (std::iscntrl( chr ))
			return false;

		if (!std::isdigit( chr ) &&
			!std::isxdigit( chr ) &&
			chr != '.' && chr != ',' && chr != '+' && chr != '-')
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
void Options::addKV(const std::string& key, const std::string& value)
//-----------------------------------------------------------------------------
{
	_kvstore.push_back( std::make_pair( key, value ));
}

//-----------------------------------------------------------------------------
bool Options::hasOption(const std::string& key) const
//-----------------------------------------------------------------------------
{
	for (const auto& kv: _kvstore) {
		if (key == kv.first)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
bool Options::getBool(const std::string& key, bool value) const
//-----------------------------------------------------------------------------
{
	std::string valstr( Options::get( key ));

	if ( !valstr.empty() ) {
		try {
			value = std::stoi( valstr, 0L );
		} catch( std::exception& err) {
			value = valstr == "true" ? true : 
				(valstr == "yes" ? true : 
				(valstr == "oui" ? true : 
				(valstr == "on" ? true : 
				(valstr == "enabled" ? true : 
				(valstr == "enabled" ? true : false)))));
		}
	}

	return value;
}

//-----------------------------------------------------------------------------
int Options::getInt(const std::string& key, int value) const
//-----------------------------------------------------------------------------
{
	std::string valstr( Options::get( key ));

	if ( !valstr.empty() ) {
		if (!isnumber( valstr ))
			return 0;

		try {
			value = std::stoi( valstr, 0L );
		} catch( std::exception& err) {
		}
	}

	return value;
}

//-----------------------------------------------------------------------------
unsigned int Options::getUInt(const std::string& key, unsigned int value) const
//-----------------------------------------------------------------------------
{
	std::string valstr( Options::get( key ));

	if ( !valstr.empty() ) {
		if (!isnumber( valstr ))
			return 0;

		try {
			value = std::stoul( valstr, 0L );
		} catch( std::exception& err) {
		}
	}

	return value;
}

//-----------------------------------------------------------------------------
float Options::getFloat(const std::string& key, float value) const
//-----------------------------------------------------------------------------
{
	std::string valstr( Options::get( key ));

	if ( !valstr.empty() ) {
		if (!isnumber( valstr ))
			return 0;

		try {
			value = std::stof( valstr, 0L );
		} catch( std::exception& err) {
		}
	}

	return value;
}

//-----------------------------------------------------------------------------
double Options::getDouble(const std::string& key, double value) const
//-----------------------------------------------------------------------------
{
	std::string valstr( Options::get( key ));

	if ( !valstr.empty() ) {
		if (!isnumber( valstr ))
			return 0;

		try {
			value = std::stof( valstr, 0L );
		} catch( std::exception& err) {
		}
	}

	return value;
}

//-----------------------------------------------------------------------------
const std::string& Options::get(const std::string& key, const std::string& value) const
//-----------------------------------------------------------------------------
{
	for (const auto& kv: _kvstore) {
		if (key == kv.first)
			return kv.second;
	}

	return value;
}

//-----------------------------------------------------------------------------
void Options::printUsage(const brzopts::Option args[])
//-----------------------------------------------------------------------------
{
	if ( ! _copyright.empty() ) {
		std::cout << _copyright << "\n\n";
	}

	std::cout << "Usage: " << _progname << " [ options ]" << "\n";

	for (int i=0; args[i].name; i++) {
		fprintf(
			stdout, "  -%c, %8s -- %s\n",
			args[i].chr, args[i].name, args[i].descr
		);
	}

	std::cout << std::endl;
	std::cout << "See man page for complete information.";
	std::cout << std::endl;
	std::cout.flush();
}

//-----------------------------------------------------------------------------
int Options::checkKV(const char* name, const char* key, const char* value, bool long_option, bool optional)
//-----------------------------------------------------------------------------
{
	if (( !value || !value[0] ) && !optional) {
		std::cerr << _progname << ": Missing argument for option '" << name << "'" << std::endl;
		_flags = MissingRequired_arg|ArgFail_arg;
		return (-1);
	}

	if ( value ) {
		if ( !optional && !std::strncmp( value, "-", 1 )) {
			std::cerr << _progname << ": Invalid argument '" << value << "' for option '" << name << "'" << std::endl;
			_flags = InvalidRequired_arg|ArgFail_arg;
			return (-1);
		}
		return (1);
	}

	return optional ? (0) : (-1);
}

//-----------------------------------------------------------------------------
const brzopts::Option* Options::getOption(const brzopts::Option args[], const char* key, bool long_option)
//-----------------------------------------------------------------------------
{
	for (int i=0; args[i].name; i++) {

		const brzopts::Option *option = &(args[i]);
		const char* sptr = std::strchr( key, '=' );
		int klen = sptr ? (sptr-key) : std::strlen( key );

		if ( long_option ) {
			if (!std::strncmp( option->name, key, klen )) {
				return option;
			}
		} else if (option->chr == key[0]) {

			if ( sptr )
				return (klen == 1) ? option : (brzopts::Option*) 0L;

			if (klen != 1 && (option->flags & brzopts::Required_arg))
				return (brzopts::Option*) 0L;

			return option;
		}
	}

	return (brzopts::Option*) 0L;
}

//-----------------------------------------------------------------------------
bool Options::parse(int& argc, char **argv, const brzopts::Option args[])
//-----------------------------------------------------------------------------
{
	const brzopts::Option *opt = 0L;
	bool long_option = false;
	char* sptr = 0L;
	size_t offset;

	_progname = argv[0];

	offset = _progname.find_last_of( '/' );

	if (offset != std::string::npos)
		_progname.erase( 0, offset+1 );

	for (int i=1; i < argc; i++) {

		char *key = argv[i];
		char *value = 0L;

		//fprintf( stderr, "argv[%d]='%s'\n", i, key );
		//fflush( stderr );

		if (!std::strncmp( key, "--", 2 )) {
			opt = getOption( args, key += 2, long_option=true );
		} else if (!std::strncmp( key, "-", 1 )) {
			opt = getOption( args, key += 1, long_option=false );
		} else {
			_args.push_back( key );
			continue;
		}

		if ( !opt ) {
			std::cerr << _progname << ": Option '" << key << "' is invalid !\n";
			std::cerr.flush();
			_flags = InvalidOption_arg|ArgFail_arg;
			return false;
		}

		std::string keyname( opt->name );

		if (keyname == "version") {
			if ( ! _version.empty() ) {
				std::cout << _version << "\n";
			} else if ( ! _copyright.empty() ) {
				std::cout << _copyright << "\n";
			} else {
				std::cout << argv[0] << ":\n";
			}

			_flags = Exit_arg;
			return true;
		}

		if (keyname == "help") {
			printUsage( args );
			_flags = Exit_arg;
			return true;
		}

		if ((opt->flags & brzopts::Required_arg)) {

			if ( long_option ) {
				if ((sptr = std::strchr( key, '=' ))) {
					(*sptr) = '\0'; value = sptr+1;
				} else {
					value = i < argc-1 ? argv[++i] : 0L;
				}
			} else if (std::strlen( key ) > 1) {
				value = key+1;

				if ((sptr = std::strchr( value, '=' ))) {
					(*sptr) = '\0'; value = sptr+1;
				} else {
					value = i < argc-1 ? argv[++i] : 0L;
				}
			} else {
				value = i < argc-1 ? argv[++i] : 0L;
			}

			if (checkKV( opt->name, key, value, long_option, false ) < 0)
				return false;

			addKV( keyname, value );

		} else if ((opt->flags & brzopts::Optional_arg)) {

			if ( long_option ) {
				if ((sptr = std::strchr( key, '=' ))) {
					(*sptr) = '\0'; value = sptr+1;
				} else if (argv[i+1][0] != '-') {
					value = i < argc-1 ? argv[++i] : 0L;
				}
			} else if (std::strlen( key ) > 1) {
				if ((sptr = std::strchr( key, '=' ))) {
					(*sptr) = '\0'; value = sptr+1;
				} else {
					value = key+1;
				}
			}

			if (checkKV( opt->name, key, value, long_option, true ) < 0)
				return false;

			if ( value ) {
				addKV( keyname, value );
			} else {
				addKV( keyname, "true" );
			}
		} else {
			addKV( keyname, "true" );
		}
	}

	_flags = Ok_arg;
	return true;
}

//==================================== EOF =====================================
