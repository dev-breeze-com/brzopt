// ----- BEGIN LICENSE BLOCK ----- //
//
// (c) Copyright 1996-2013, Glyph.Cloud, All Rights Reserved.
//
// GLYPH.CLOUD MAKES NO REPRESENTATIONS AND EXTENDS NO WARRANTIES, EXPRESS OR
// IMPLIED, WITH RESPECT TO THE SOFTWARE, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY PARTICULAR
// PURPOSE, AND THE WARRANTY AGAINST INFRINGEMENT OF PATENTS OR OTHER
// INTELLECTUAL PROPERTY RIGHTS. THE SOFTWARE IS PROVIDED "AS IS", AND IN NO
// EVENT SHALL GLYPH.CLOUD OR ANY OF ITS AFFILIATES BE LIABLE FOR ANY DAMAGES,
// INCLUDING ANY LOST PROFITS OR OTHER INCIDENTAL OR CONSEQUENTIAL DAMAGES
// RELATING TO THE USE OF THIS SOFTWARE.
//
// ----- END LICENSE BLOCK ----- //

#ifndef _glyph_brzopts_h_
#define _glyph_brzopts_h_

#include <list>
#include <map>
#include <cstring>
#include <iostream>
#include <string>

//--------------------------------- Classes -----------------------------------

namespace glyph_cloud {

namespace brzopts {

	enum ArgType {
		No_arg	= 0x0000,
		Ok_arg	= 0x0001,
		Exit_arg		= 0x0002,
		Required_arg	= 0x0004,
		Optional_arg	= 0x0008,

		ArgFail_arg 	= 0x0010,
		UrlFail_arg 	= 0x0020,
		InitFail_arg	= 0x0040,

		MissingRequired_arg	= 0x0100,
		InvalidRequired_arg	= 0x0200,
		InvalidOptional_arg	= 0x0400,
		InvalidOption_arg	= 0x0800
	};

	typedef struct Option {
		const char* name;
		ArgType flags;
		const char chr;
		const char* descr;
	} Option;

	class Options {
	protected:
		std::string _version;
		std::string _copyright;
		std::string _progname;
		std::list<std::string> _args;
		std::list< std::pair<std::string, std::string> > _kvstore; 
		unsigned int _flags;

		const ArgType getFlags() const { return static_cast<brzopts::ArgType>(_flags); }

		const Option* getOption(const Option args[], const char* opt, bool);

		int checkKV(const char* name, const char* key, const char* value,
			bool long_option, bool optional);

	public:
		Options(const std::string& version=std::string(),
			const std::string& copyright=std::string());
		virtual ~Options();

		void setVersion(const std::string& v) { _version = v; }
		void setCopyright(const std::string& v) { _copyright = v; }

	public:
		bool parse(int& argc, char **argv, const Option args[]);

		const std::list<std::string>& getArgs() const { return _args; }

		const std::list< std::pair<std::string, std::string> >& getKV() const { return _kvstore; }

		void addKV(const std::string& key, const std::string& value);

		void printUsage(const Option args[]);

	public:
		bool isnumber(const std::string& value) const;
		bool getBool(const std::string& key, bool flag=false) const;
		int getInt(const std::string& key, int val=0) const;
		unsigned int getUInt(const std::string& key, unsigned int val=0) const;
		float getFloat(const std::string& key, float val=0) const;
		double getDouble(const std::string& key, double val=0) const;

		bool hasOption(const std::string& key) const;

		const std::string& get(const std::string& key,
			const std::string& value=std::string()) const;

	public:
		bool isOk() const;
		bool exitNow() const;
		bool urlFailed() const;
		bool argsFailed() const;
		bool initFailed() const;

	};
};

};

#endif // _glyph_brzopts_h_

//==================================== EOF ====================================
