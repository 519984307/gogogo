#ifndef __JXON__
#define __JXON__

#include "txvalue.hpp"
#include <string>
#include <stack>
#include <deque>

namespace Jxon {

	struct Reader 
	{
		typedef char Char;
		typedef const Char *Location;
		enum TokenType
		{
			tokenEndOfStream = 0,
			tokenObjectBegin,
			tokenObjectEnd,
			tokenArrayBegin,
			tokenArrayEnd,
			tokenString,
			tokenNumber,
			tokenTrue,
			tokenFalse,
			tokenNull,
			tokenArraySeparator,
			tokenMemberSeparator,
			tokenComment,
			tokenError
		};

		class Token
		{
		public:
			TokenType type_;
			Location start_;
			Location end_;
		};

		class ErrorInfo
		{
		public:
			Token token_;
			std::string message_;
			Location extra_;
		};

		typedef std::deque<ErrorInfo> Errors;

		typedef std::stack<xv::xvalue_t*> Nodes;
		Nodes nodes_;
		Errors errors_;
		//std::string document_;
		Location begin_;
		Location end_;
		Location current_;

		static inline bool 
			in( Char c, Char c1, Char c2, Char c3, Char c4 )
		{
			return c == c1  ||  c == c2  ||  c == c3  ||  c == c4;
		}

		static inline bool 
			in( Char c, Char c1, Char c2, Char c3, Char c4, Char c5 )
		{
			return c == c1  ||  c == c2  ||  c == c3  ||  c == c4  ||  c == c5;
		}


		static bool 
			containsNewLine( Location begin, 
			Location end )
		{
			for ( ;begin < end; ++begin )
				if ( *begin == '\n'  ||  *begin == '\r' )
					return true;
			return false;
		}

		static std::string codePointToUTF8(unsigned int cp)
		{
			std::string result;

			// based on description from http://en.wikipedia.org/wiki/UTF-8

			if (cp <= 0x7f) 
			{
				result.resize(1);
				result[0] = static_cast<char>(cp);
			} 
			else if (cp <= 0x7FF) 
			{
				result.resize(2);
				result[1] = static_cast<char>(0x80 | (0x3f & cp));
				result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
			} 
			else if (cp <= 0xFFFF) 
			{
				result.resize(3);
				result[2] = static_cast<char>(0x80 | (0x3f & cp));
				result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
				result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
			}
			else if (cp <= 0x10FFFF) 
			{
				result.resize(4);
				result[3] = static_cast<char>(0x80 | (0x3f & cp));
				result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
				result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
				result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
			}

			return result;
		}

		bool
			parse( const std::string &document, xv::xvalue_t &root )
		{
			const char *begin = document.c_str();
			const char *end = begin + document.length();
			return parse( begin, end, root );
		}


		bool
			parse( std::istream& sin, xv::xvalue_t &root )
		{
			//std::istream_iterator<char> begin(sin);
			//std::istream_iterator<char> end;
			// Those would allow streamed input from a file, if parse() were a
			// template function.

			// Since std::string is reference-counted, this at least does not
			// create an extra copy.
			std::string doc;
			std::getline(sin, doc, (char)EOF);
			return parse( doc, root );
		}

		bool 
			parse( const char *beginDoc, const char *endDoc, xv::xvalue_t & root )
		{
			begin_ = beginDoc;
			end_ = endDoc;
			current_ = begin_;
			//lastValueEnd_ = 0;
			//lastValue_ = 0;
			//commentsBefore_ = "";
			errors_.clear();
			while ( !nodes_.empty() )
				nodes_.pop();

			nodes_.push( &root );

			bool successful = readValue();

			Token token;
			skipCommentTokens( token );
			return successful;
		}


		bool
			readValue()
		{
			Token token;
			skipCommentTokens( token );
			bool successful = true;

			switch ( token.type_ )
			{
			case tokenObjectBegin:
				successful = readObject( token );
				break;
			case tokenArrayBegin:
				successful = readArray( token );
				break;
			case tokenNumber:
				successful = decodeNumber( token );
				break;
			case tokenString:
				successful = decodeString( token );
				break;
			case tokenTrue:
				currentValue() = true;
				break;
			case tokenFalse:
				currentValue() = false;
				break;
			case tokenNull:
				currentValue() = xv::xvalue_t();
				break;
			default:
				return addError( "Syntax error: xval, object or array expected.", token );
			}

			return successful;
		}


		void 
			skipCommentTokens( Token &token )
		{
			do
			{
				readToken( token );
			}
			while ( token.type_ == tokenComment );
		}


		bool 
			expectToken( TokenType type, Token &token, const char *message )
		{
			readToken( token );
			if ( token.type_ != type )
				return addError( message, token );
			return true;
		}


		bool 
			readToken( Token &token )
		{
			skipSpaces();
			token.start_ = current_;
			Char c = getNextChar();
			bool ok = true;
			switch ( c )
			{
			case '{':
				token.type_ = tokenObjectBegin;
				break;
			case '}':
				token.type_ = tokenObjectEnd;
				break;
			case '[':
				token.type_ = tokenArrayBegin;
				break;
			case ']':
				token.type_ = tokenArrayEnd;
				break;
			case '"':
				token.type_ = tokenString;
				ok = readString();
				break;
			case '/':
				token.type_ = tokenComment;
				ok = readComment();
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				token.type_ = tokenNumber;
				readNumber();
				break;
			case 't':
				token.type_ = tokenTrue;
				ok = match( "rue", 3 );
				break;
			case 'f':
				token.type_ = tokenFalse;
				ok = match( "alse", 4 );
				break;
			case 'n':
				token.type_ = tokenNull;
				ok = match( "ull", 3 );
				break;
			case ',':
				token.type_ = tokenArraySeparator;
				break;
			case ':':
				token.type_ = tokenMemberSeparator;
				break;
			case 0:
				token.type_ = tokenEndOfStream;
				break;
			default:
				ok = false;
				break;
			}
			if ( !ok )
				token.type_ = tokenError;
			token.end_ = current_;
			return true;
		}


		void 
			skipSpaces()
		{
			while ( current_ != end_ )
			{
				Char c = *current_;
				if ( c == ' '  ||  c == '\t'  ||  c == '\r'  ||  c == '\n' )
					++current_;
				else
					break;
			}
		}


		bool 
			match( Location pattern, 
			int patternLength )
		{
			if ( end_ - current_ < patternLength )
				return false;
			int index = patternLength;
			while ( index-- )
				if ( current_[index] != pattern[index] )
					return false;
			current_ += patternLength;
			return true;
		}


		bool
			readComment()
		{
			Location commentBegin = current_ - 1;
			Char c = getNextChar();
			bool successful = false;
			if ( c == '*' )
				successful = readCStyleComment();
			else if ( c == '/' )
				successful = readCppStyleComment();
			if ( !successful )
				return false;

			return true;
		}


		//void 
		//	addComment( Location begin, 
		//	Location end, 
		//	CommentPlacement placement )
		//{
		//	return ;
		//	//assert( false );
		//	//if ( placement == commentAfterOnSameLine )
		//	//{
		//	//	assert( lastValue_ != 0 );
		//	//	lastValue_->setComment( std::string( begin, end ), placement );
		//	//}
		//	//else
		//	//{
		//	//	if ( !commentsBefore_.empty() )
		//	//		commentsBefore_ += "\n";
		//	//	commentsBefore_ += std::string( begin, end );
		//	//}
		//}


		bool 
			readCStyleComment()
		{
			while ( current_ != end_ )
			{
				Char c = getNextChar();
				if ( c == '*'  &&  *current_ == '/' )
					break;
			}
			return getNextChar() == '/';
		}


		bool 
			readCppStyleComment()
		{
			while ( current_ != end_ )
			{
				Char c = getNextChar();
				if (  c == '\r'  ||  c == '\n' )
					break;
			}
			return true;
		}


		void 
			readNumber()
		{
			while ( current_ != end_ )
			{
				if ( !(*current_ >= '0'  &&  *current_ <= '9')  &&
					!in( *current_, '.', 'e', 'E', '+', '-' ) )
					break;
				++current_;
			}
		}

		bool
			readString()
		{
			Char c = 0;
			while ( current_ != end_ )
			{
				c = getNextChar();
				if ( c == '\\' )
					getNextChar();
				else if ( c == '"' )
					break;
			}
			return c == '"';
		}


		bool 
			readObject( Token &tokenStart )
		{
			Token tokenName;
			std::string name;
			currentValue() = xv::xvalue_t( xv::dObject );
			while ( readToken( tokenName ) )
			{
				bool initialTokenOk = true;
				while ( tokenName.type_ == tokenComment  &&  initialTokenOk )
					initialTokenOk = readToken( tokenName );
				if  ( !initialTokenOk )
					break;
				if ( tokenName.type_ == tokenObjectEnd  &&  name.empty() )  // empty object
					return true;
				if ( tokenName.type_ != tokenString )
					break;

				name = "";
				if ( !decodeString( tokenName, name ) )
					return recoverFromError( tokenObjectEnd );

				Token colon;
				if ( !readToken( colon ) ||  colon.type_ != tokenMemberSeparator )
				{
					return addErrorAndRecover( "Missing ':' after object member name", 
						colon, 
						tokenObjectEnd );
				}
				xv::xvalue_t &xval = currentValue()[ name ];
				nodes_.push( &xval );
				bool ok = readValue();
				nodes_.pop();
				if ( !ok ) // error already set
					return recoverFromError( tokenObjectEnd );

				Token comma;
				if ( !readToken( comma )
					||  ( comma.type_ != tokenObjectEnd  &&  
					comma.type_ != tokenArraySeparator &&
					comma.type_ != tokenComment ) )
				{
					return addErrorAndRecover( "Missing ',' or '}' in object declaration", 
						comma, 
						tokenObjectEnd );
				}
				bool finalizeTokenOk = true;
				while ( comma.type_ == tokenComment &&
					finalizeTokenOk )
					finalizeTokenOk = readToken( comma );
				if ( comma.type_ == tokenObjectEnd )
					return true;
			}
			return addErrorAndRecover( "Missing '}' or object member name", 
				tokenName, 
				tokenObjectEnd );
		}


		bool 
			readArray( Token &tokenStart )
		{
			currentValue() = xv::xvalue_t( xv::dArray );
			skipSpaces();
			if ( *current_ == ']' ) // empty array
			{
				Token endArray;
				readToken( endArray );
				return true;
			}
			int index = 0;
			while ( true )
			{
				xv::xvalue_t &xval = currentValue()[ index++ ];
				nodes_.push( &xval );
				bool ok = readValue();
				nodes_.pop();
				if ( !ok ) // error already set
					return recoverFromError( tokenArrayEnd );

				Token token;
				// Accept Comment after last item in the array.
				ok = readToken( token );
				while ( token.type_ == tokenComment  &&  ok )
				{
					ok = readToken( token );
				}
				bool badTokenType = ( token.type_ == tokenArraySeparator  &&  
					token.type_ == tokenArrayEnd );
				if ( !ok  ||  badTokenType )
				{
					return addErrorAndRecover( "Missing ',' or ']' in array declaration", 
						token, 
						tokenArrayEnd );
				}
				if ( token.type_ == tokenArrayEnd )
					break;
			}
			return true;
		}


		bool 
			decodeNumber( Token &token )
		{
			bool isDouble = false;
			for ( Location inspect = token.start_; inspect != token.end_; ++inspect )
			{
				isDouble = isDouble  
					||  in( *inspect, '.', 'e', 'E', '+' )  
					||  ( *inspect == '-'  &&  inspect != token.start_ );
			}
			if ( isDouble )
				return decodeDouble( token );

			Location current = token.start_;

			bool isNegative = *current == '-';

			if ( isNegative ) ++ current;

			//uint64_t threshold = (isNegative ? uint64_t(-xv::xvalue_t::minInt) : xv::xvalue_t::maxUInt) / 10;

			uint64_t xval = 0;
			while ( current < token.end_ )
			{
				Char c = *current++;
				if ( c < '0'  ||  c > '9' )
					return addError( "'" + std::string( token.start_, token.end_ ) + "' is not a number.", token );

				//if ( xval >= threshold )
				//	return decodeDouble( token );
				xval = xval * 10 + uint64_t(c - '0');
			}
			if ( isNegative )
				currentValue() = (int64_t)(0-xval);
			else
				currentValue() = (int64_t)xval;
			return true;
		}


		bool 
			decodeDouble( Token &token )
		{
			double xval = 0;
			const int bufferSize = 32;
			int count;
			int length = int(token.end_ - token.start_);
			if ( length < bufferSize )
			{
				Char buffer[bufferSize];
				memcpy( buffer, token.start_, length );
				buffer[length] = 0;
				count = sscanf( buffer, "%lf", &xval );
			}
			else
			{
				std::string buffer( token.start_, token.end_ );
				count = sscanf( buffer.c_str(), "%lf", &xval );
			}

			if ( count != 1 )
				return addError( "'" + std::string( token.start_, token.end_ ) + "' is not a number.", token );
			currentValue() = xval;
			return true;
		}


		bool 
			decodeString( Token &token )
		{
			std::string decoded;
			if ( !decodeString( token, decoded ) )
				return false;
			currentValue() = decoded;
			return true;
		}


		bool decodeString( Token &token, std::string &decoded )
		{
			decoded.reserve( token.end_ - token.start_ - 2 );
			Location current = token.start_ + 1; // skip '"'
			Location end = token.end_ - 1;      // do not include '"'
			while ( current != end )
			{
				Char c = *current++;
				if ( c == '"' )
					break;
				else if ( c == '\\' )
				{
					if ( current == end )
						return addError( "Empty escape sequence in string", token, current );
					Char escape = *current++;
					switch ( escape )
					{
					case '"': decoded += '"'; break;
					case '/': decoded += '/'; break;
					case '\\': decoded += '\\'; break;
					case 'b': decoded += '\b'; break;
					case 'f': decoded += '\f'; break;
					case 'n': decoded += '\n'; break;
					case 'r': decoded += '\r'; break;
					case 't': decoded += '\t'; break;
					case 'u':
						{
							unsigned int unicode;
							if ( !decodeUnicodeCodePoint( token, current, end, unicode ) )
								return false;
							decoded += codePointToUTF8(unicode);
						}
						break;
					default:
						return addError( "Bad escape sequence in string", token, current );
					}
				}
				else
				{
					decoded += c;
				}
			}
			return true;
		}

		bool
			decodeUnicodeCodePoint( Token &token, 
			Location &current, 
			Location end, 
			unsigned int &unicode )
		{

			if ( !decodeUnicodeEscapeSequence( token, current, end, unicode ) )
				return false;
			if (unicode >= 0xD800 && unicode <= 0xDBFF)
			{
				// surrogate pairs
				if (end - current < 6)
					return addError( "additional six characters expected to parse unicode surrogate pair.", token, current );
				unsigned int surrogatePair;
				if (*(current++) == '\\' && *(current++)== 'u')
				{
					if (decodeUnicodeEscapeSequence( token, current, end, surrogatePair ))
					{
						unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
					} 
					else
						return false;
				} 
				else
					return addError( "expecting another \\u token to begin the second half of a unicode surrogate pair", token, current );
			}
			return true;
		}

		bool 
			decodeUnicodeEscapeSequence( Token &token, 
			Location &current, 
			Location end, 
			unsigned int &unicode )
		{
			if ( end - current < 4 )
				return addError( "Bad unicode escape sequence in string: four digits expected.", token, current );
			unicode = 0;
			for ( int index =0; index < 4; ++index )
			{
				Char c = *current++;
				unicode *= 16;
				if ( c >= '0'  &&  c <= '9' )
					unicode += c - '0';
				else if ( c >= 'a'  &&  c <= 'f' )
					unicode += c - 'a' + 10;
				else if ( c >= 'A'  &&  c <= 'F' )
					unicode += c - 'A' + 10;
				else
					return addError( "Bad unicode escape sequence in string: hexadecimal digit expected.", token, current );
			}
			return true;
		}


		bool 
			addError( const std::string &message, 
			Token &token,
			Location extra = 0 )
		{
			ErrorInfo info;
			info.token_ = token;
			info.message_ = message;
			info.extra_ = extra;
			errors_.push_back( info );
			return false;
		}


		bool 
			recoverFromError( TokenType skipUntilToken )
		{
			int errorCount = int(errors_.size());
			Token skip;
			while ( true )
			{
				if ( !readToken(skip) )
					errors_.resize( errorCount ); // discard errors caused by recovery
				if ( skip.type_ == skipUntilToken  ||  skip.type_ == tokenEndOfStream )
					break;
			}
			errors_.resize( errorCount );
			return false;
		}


		bool 
			addErrorAndRecover( const std::string &message, 
			Token &token,
			TokenType skipUntilToken )
		{
			addError( message, token );
			return recoverFromError( skipUntilToken );
		}


		xv::xvalue_t & currentValue()
		{
			return *(nodes_.top());
		}


		Char getNextChar()
		{
			if ( current_ == end_ )
				return 0;
			return *current_++;
		}


		void getLocationLineAndColumn( Location location,
			int &line,
			int &column ) const
		{
			Location current = begin_;
			Location lastLineStart = current;
			line = 0;
			while ( current < location  &&  current != end_ )
			{
				Char c = *current++;
				if ( c == '\r' )
				{
					if ( *current == '\n' )
						++current;
					lastLineStart = current;
					++line;
				}
				else if ( c == '\n' )
				{
					lastLineStart = current;
					++line;
				}
			}
			// column & line start at 1
			column = int(location - lastLineStart) + 1;
			++line;
		}


		std::string getLocationLineAndColumn( Location location ) const
		{
			int line, column;
			getLocationLineAndColumn( location, line, column );
			char buffer[18+16+16+1];
			sprintf( buffer, "Line %d, Column %d", line, column );
			return buffer;
		}


		std::string getFormatedErrorMessages() const
		{
			std::string formattedMessage;
			for ( Errors::const_iterator itError = errors_.begin();
				itError != errors_.end();
				++itError )
			{
				const ErrorInfo &error = *itError;
				formattedMessage += "* " + getLocationLineAndColumn( error.token_.start_ ) + "\n";
				formattedMessage += "  " + error.message_ + "\n";
				if ( error.extra_ )
					formattedMessage += "See " + getLocationLineAndColumn( error.extra_ ) + " for detail.\n";
			}
			return formattedMessage;
		}
	};

	struct Writer 
	{
		Writer() {
		}
		static bool isControlCharacter(char ch)
		{
			return ch > 0 && ch <= 0x1F;
		}
		static bool containsControlCharacter( const char* str )
		{
			while ( *str ) 
			{
				if ( isControlCharacter( *(str++) ) )
					return true;
			}
			return false;
		}
		static void uintToString( uint32_t xval, char *&current )
		{
			*--current = 0;
			do
			{
				*--current = (xval % 10) + '0';
				xval /= 10;
			}
			while ( xval != 0 );
		}
		static std::string valueToString( int32_t xval )
		{
			char buffer[32];
			char *current = buffer + sizeof(buffer);
			bool isNegative = xval < 0;
			if ( isNegative ) xval = -xval;
			uintToString( uint32_t(xval), current );
			if ( isNegative ) *--current = '-';
			return current;
		}
		static std::string valueToString( double xval )
		{
			char buffer[32];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__) // Use secure version with visual studio 2005 to avoid warning. 
			sprintf_s(buffer, sizeof(buffer), "%#.16g", xval); 
#else	
			sprintf(buffer, "%#.16g", xval); 
#endif
			char* ch = buffer + strlen(buffer) - 1;
			if (*ch != '0') return buffer; // nothing to truncate, so save time
			while(ch > buffer && *ch == '0'){
				--ch;
			}
			char* last_nonzero = ch;
			while(ch >= buffer){
				switch(*ch){
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					--ch;
					continue;
				case '.':
					// Truncate zeroes to save bytes in output, but keep one.
					*(last_nonzero+2) = '\0';
					return buffer;
				default:
					return buffer;
				}
			}
			return buffer;
		}
		static std::string valueToString( bool xval )
		{
			return xval ? "true" : "false";
		}
		static std::string valueToQuotedString( const char *xval )
		{
			// Not sure how to handle unicode...
			if (strpbrk(xval, "\"\\\b\f\n\r\t") == NULL && !containsControlCharacter( xval ))
				return std::string("\"") + xval + "\"";
			// We have to walk xval and escape any special characters.
			// Appending to std::string is not efficient, but this should be rare.
			// (Note: forward slashes are *not* rare, but I am not escaping them.)
			size_t maxsize = strlen(xval)*2 + 3; // allescaped+quotes+NULL
			std::string result;
			result.reserve(maxsize); // to avoid lots of mallocs
			result += "\"";
			for (const char* c=xval; *c != 0; ++c)
			{
				switch(*c)
				{
				case '\"':
					result += "\\\"";
					break;
				case '\\':
					result += "\\\\";
					break;
				case '\b':
					result += "\\b";
					break;
				case '\f':
					result += "\\f";
					break;
				case '\n':
					result += "\\n";
					break;
				case '\r':
					result += "\\r";
					break;
				case '\t':
					result += "\\t";
					break;
					//case '/':
					// Even though \/ is considered a legal escape in JSON, a bare
					// slash is also legal, so I see no reason to escape it.
					// (I hope I am not misunderstanding something.
					// blep notes: actually escaping \/ may be useful in javascript to avoid </ 
					// sequence.
					// Should add a flag to allow this compatibility mode and prevent this 
					// sequence from occurring.
				default:
					if ( isControlCharacter( *c ) )
					{
						char tmp[8] = {};
						sprintf( tmp, "%04X", (UINT16)*c );
						//std::ostringstream oss;
						//oss << "\\u" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << static_cast<int>(*c);
						result += tmp;
					}
					else
					{
						result += *c;
					}
					break;
				}
			}
			result += "\"";
			return result;
		}

		static size_t utf16_to_utf8(const wchar_t * src, size_t len, std::string & ret )
		{
			static const uint8_t kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
			size_t dpos = 0, spos = 0;
			for (;;)
			{
				unsigned num_add;

				uint32_t uc16;

				if (spos == len) break;

				uc16 = src[spos++];
				if (uc16 < 0x80)
				{
					ret.push_back(uc16);
					dpos++;
					continue;
				}
				if (uc16 >= 0xD800 && uc16 < 0xE000)
				{
					uint32_t c2;
					if (uc16 >= 0xDC00 || spos == len) break;
					c2 = src[spos++]; 
					if (c2 < 0xDC00 || c2 >= 0xE000) break;
					uc16 = (((uc16 - 0xD800) << 10) | (c2 - 0xDC00)) + 0x10000;
				}

				for (num_add = 1; num_add < 5; ++num_add)
					if (uc16 < (((uint32_t)1) << (num_add * 5 + 6)))
						break;

				ret.push_back( (char)(kUtf8Limits[num_add - 1] + (uc16 >> (6 * num_add))) );
				dpos++;

				do {
					num_add--;
					ret.push_back( (char)(0x80 + ((uc16 >> (6 * num_add)) & 0x3F)) );
					dpos++;
				}
				while (num_add != 0);
			}
			return (spos == len);
		}

		static bool append( const char * str, size_t len, xv::Output * o )
		{
			return o->write( str, len );
		}

		static bool append( std::string str, xv::Output * o )
		{
			return o->write( str.c_str(), str.size() );
		}

		static bool append( const char * str, xv::Output * o )
		{
			return append( str, strlen(str), o );
		}

		static bool write_null( xv::Output * o )
		{
			return append( STR_SIZE("null"), o );
		}

		static bool write_int( xv::xvalue_t & v, xv::Output * o )
		{
			int32_t x = 0;
			switch( v._type.bsiz )
			{
			case xv::_8:	x = (uint8_t)v.integer; break;
			case xv::_16:	x = (uint16_t)v.integer; break;
			case xv::_32:	x = (uint32_t)v.integer; break;
			case xv::_64:	x = (uint32_t)v.integer; break;
			}
			char tmp[16] = {};
			sprintf( tmp, "%d", x );
			return append( tmp, o );
		}

		static bool write_real( xv::xvalue_t & v, xv::Output * o )
		{
			char tmp[32] = {};
			sprintf( tmp, "%g", v.real );
			return append( tmp, o );
		}

		static bool write_bool(xv::xvalue_t & v,xv::Output * o )
		{
			return  v.vbool ? 
				append( STR_SIZE("true"), o ) : 
				append( STR_SIZE("false"), o );
		}

		static bool write_lstr(xv::xvalue_t& v,xv::Output * o )
		{
			return append( valueToQuotedString( v.asCString() ), o );
		}

		static bool write_utf8(xv::xvalue_t& v,xv::Output * o )
		{
			return append( valueToQuotedString( v.asCString() ), o );
		}

		static bool write_unicode(xv::xvalue_t& v,xv::Output * o )
		{
			refp<xv::vStringW> str = v.asStringW(false);
			if( !str ) return false;
			const wchar_t * text = str->c_str();
			size_t wlen = wcslen(text);
			std::string utf8;
			utf16_to_utf8( text, wlen, utf8 );
			return append( valueToQuotedString( utf8.c_str() ), o );
		}

		static void ascii_to_hex( uint8_t u, char * str ) {
			uint8_t v4 = u >> 4;
			if( v4 > 9 ) v4 = 'A' + v4 - 9;
			else v4 = '0' + v4;
			str[0] = v4;
			v4 = u & 0xf;
			if( v4 > 9 ) v4 = 'A' + v4 - 9;
			else v4 = '0' + v4;
			str[1] = v4;
		}

		static bool write_buffer(xv::xvalue_t& v,xv::Output * o )
		{
			refp<xv::vBuffer> buf = v.asBuffer();
			if( !buf ) return false;
			const uint8_t * ptr = buf->base();
			size_t bytes = buf->length();
			char temp[3] = {};
			if( !o->write( "\"", 1 ) ) 
				return false;
			for( size_t i = 0; i < bytes; ++ i ) {
				ascii_to_hex( ptr[i], temp );
				if( !o->write( temp, 2 ) ) 
					return false;
			}
			if( !o->write( "\"", 1 ) ) 
				return false;
			return true;
		}

		static bool write_array(xv::xvalue_t& v,xv::Output * o )
		{
			refp<xv::vArray> arr = v.asArray(false);
			if( !arr ) return false;
			size_t ce = arr->length();
			if( ce > 0x1000000 ) return false;
			struct dumper
			{
				size_t _a_index;
				xv::Output * _out;
				dumper(xv::Output * ao ) : _a_index(0), _out(ao) {
				}
				bool __dump_pair( size_t idx, xv::Value & item, bool last )
				{
					for( ; _a_index < idx; ++ _a_index ) 
					{
						if( _a_index ) 
							if( !append( ",", _out ) ) 
								return false;
						if( !write_null( _out ) )
							return false;
					}

					if( idx ) 
						if( !append( ",", _out ) ) 
							return false;

					xv::xvalue_t tmp(item);
					if( !write( tmp, _out ) )
						return false;

					_a_index = idx + 1;
					return true;
				}
				static bool dump_pair( size_t idx, xv::Value & item, bool last, void * p )
				{
					if( !p ) 
						return false;
					dumper * d = (dumper*)p;
					return d->__dump_pair(idx, item, last );
				}
			};
			dumper d(o);
			if( !append( "[", o ) ) return false;
			if( !arr->enum_it( dumper::dump_pair, &d ) ) return false;
			if( !append( "]", o ) ) return false;
			return true;
		};

		static bool write_dict(xv::xvalue_t& v,xv::Output * o )
		{
			refp<xv::vObject> obj = v.asObject(false);
			if( !obj ) return false;
			size_t cf = obj->length();
			if( cf > 0x10000 ) return false;
			struct dumper
			{
				static bool dump_pair( const char * name, xv::Value & item, bool last, void * p )
				{
					if( !name || !p ) 
						return false;
					xv::Output * o = (xv::Output *)p;
					append( valueToQuotedString(name), o );
					append( ":", o  );
					xv::xvalue_t tmp(item);
					if( !write( tmp, o ) ) 
						return false;
					if( !last ) 
						return append( ",", o );
					return true;
				}
			};
			if( !append( "{", o ) ) return false;
			if( !obj->enum_it( dumper::dump_pair, o ) )
				return false;
			if( !append( "}", o ) ) return false;
			return true;
		};
		static bool write(xv::xvalue_t & v, xv::Output * o )
		{
			switch( v._type.type )
			{
			case xv::dNull:		return write_null( o );
			case xv::dInt:		return write_int( v, o );
			case xv::dReal:		return write_real( v, o );
			case xv::dLocalStr:	return write_lstr( v, o );
			case xv::dUtf8Str:	return write_utf8( v, o );
			case xv::dUniStr:	return write_unicode( v, o );
			case xv::dArray:	return write_array( v, o );
			case xv::dObject:	return write_dict( v, o );
			case xv::dBuffer:	return write_buffer( v, o );
			case xv::dBoolean:	return write_bool( v, o );
			}
			return false;
		};

		bool dumps(xv::xvalue_t & v, std::string & str )
		{
			struct string_output : xv::Output { 
				std::string & output_;
				string_output( std::string & str ) : output_(str) {}
				virtual bool write( const void * p, size_t cb ) {
					output_.append( (const char*)p, cb );
					return true;
				}
			};
			//return write( v, &string_output(str) );
			string_output tmp(str);
			return write( v, &tmp );
		}
	};

} // namespace Jxon


#endif
