/*
 * @file logging.h
 * @author Thomas Roehr, thomas.roehr@dfki.de
 *
 * @brief Plain logging class
 */

#ifndef _BASE_LOGGING_H_
#define _BASE_LOGGING_H_

// Need to map to logging::Priority order
// ALlowing to set a log level via CFLAGS - method call will not be compiled into the system
// Setting the enviroment variable BASE_LOG_LEVEL does only have any effect if the compiled
// level is equal or higher (closer to FATAL) than the one request
//
#if defined(BASE_LOG_FATAL)
#define BASE_LOG_PRIORITY 1
#undef BASE_LOG_FATAL
#elif defined(BASE_LOG_ERROR)
#define BASE_LOG_PRIORITY 2
#undef BASE_LOG_ERROR
#elif defined(BASE_LOG_WARN)
#define BASE_LOG_PRIORITY 3
#undef BASE_LOG_WARN
#elif defined(BASE_LOG_INFO)
#define BASE_LOG_PRIORITY 4
#undef BASE_LOG_INFO
#elif defined(BASE_LOG_DEBUG)
#define BASE_LOG_PRIORITY 5
#undef BASE_LOG_DEBUG
#endif

// Default logging priority that is compiled in, i.e. all log levels 
// will be accessible at runtime
#ifndef BASE_LOG_PRIORITY
#define BASE_LOG_PRIORITY 6
#endif

#ifdef BASE_LONG_NAMES
// Empty definition of debug statement
#define BASE_LOG_DEBUG(FORMAT, ARGS...)
#define BASE_LOG_INFO(FORMAT, ARGS...)
#define BASE_LOG_WARN(FORMAT, ARGS...)
#define BASE_LOG_ERROR(FORMAT, ARGS...)
#define BASE_LOG_FATAL(FORMAT, ARGS...)
#define BASE_LOG_CONFIGURE(PRIO, STREAM)
#else
#define LOG_DEBUG(FORMAT, ARGS...)
#define LOG_INFO(FORMAT, ARGS...)
#define LOG_WARN(FORMAT, ARGS...)
#define LOG_ERROR(FORMAT, ARGS...)
#define LOG_FATAL(FORMAT, ARGS...)
#define LOG_CONFIGURE(PRIO, STREAM)
#endif // BASE_LONG_NAMES

#ifndef Release

#ifndef BASE_LOG_NAMESPACE
#warning "BASE_LOG_NAMESPACE is not set - will be using empty namespaces"
#define BASE_LOG_NAMESPACE ""
#endif // BASE_LOG_NAMESPACE

// The debug flag __BASE_LOG_NAMESPACE__ needs to be converted to a string
// Stringify element
#define __STRINGIFY_(X) #X
// expand x before being stringified
#define __STRINGIFY(X) __STRINGIFY_(X)

// Depending on the globally set log level insert log statements by preprocessor
//
// The namespace represents the library name and should be set via definitions, e.g. in
// your CMakeLists.txt -D__BASE_LOG_NAMESPACE__=yournamespace
//
#define __LOG(PRIO, FORMAT, ARGS ...) { using namespace base::logging; Logger::getInstance()->log(PRIO, __FILE__, __LINE__, "%s::" FORMAT, __STRINGIFY(BASE_LOG_NAMESPACE), ## ARGS); }

#ifdef BASE_LONG_NAMES

#if BASE_LOG_PRIORITY >= 1 
#undef BASE_LOG_FATAL
#define BASE_LOG_FATAL(FORMAT, ARGS...) __LOG(FATAL, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 2
#undef BASE_LOG_ERROR
#define BASE_LOG_ERROR(FORMAT, ARGS...) __LOG(ERROR, FORMAT, ## ARGS)
#endif
 
#if BASE_LOG_PRIORITY >= 3
#undef BASE_LOG_WARN
#define BASE_LOG_WARN(FORMAT, ARGS...) __LOG(WARN, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 4 
#undef BASE_LOG_INFO
#define BASE_LOG_INFO(FORMAT, ARGS...) __LOG(INFO, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 5
#undef BASE_LOG_DEBUG
#define BASE_LOG_DEBUG(FORMAT, ARGS...) __LOG(DEBUG, FORMAT, ## ARGS)  
#endif

#undef BASE_LOG_CONFIGURE
#define BASE_LOG_CONFIGURE(PRIO,STREAM) { using namespace base::logging; Logger::getInstance()->configure(PRIO, STREAM); }

#else // #ifdef BASE_LONG_NAMES

// If there should be conflicts with other libraries, switch to long names
// Other wise short names will be available as only
#undef LOG_CONFIGURE
#define LOG_CONFIGURE(PRIO,STREAM) { using namespace base::logging; Logger::getInstance()->configure(PRIO, STREAM); }

#if BASE_LOG_PRIORITY >= 1 
#undef LOG_FATAL
#define LOG_FATAL(FORMAT, ARGS...) __LOG(FATAL, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 2
#undef LOG_ERROR
#define LOG_ERROR(FORMAT, ARGS...) __LOG(ERROR, FORMAT, ## ARGS)
#endif
 
#if BASE_LOG_PRIORITY >= 3
#undef LOG_WARN
#define LOG_WARN(FORMAT, ARGS...) __LOG(WARN, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 4 
#undef LOG_INFO
#define LOG_INFO(FORMAT, ARGS...) __LOG(INFO, FORMAT, ## ARGS)
#endif

#if BASE_LOG_PRIORITY >= 5
#undef LOG_DEBUG
#define LOG_DEBUG(FORMAT, ARGS...) __LOG(DEBUG, FORMAT, ## ARGS)  
#endif

#endif // BASE_LONG_NAMES

#endif // Release


#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <base/singleton.h>

using namespace std;

namespace base {

namespace logging {

	enum Priority	{ UNKNOWN = 0, FATAL , ERROR, WARN, INFO, DEBUG  };

	/**
	 * @class Logger
	 * @brief Logger is a logger that allows priority based logging
         * with minimal impact on performance and minimal configuration
         * requirements
         * 
         * The logger will be only active in an application compiled 
         * not with Release Flag 
         *
         * Use the enviroment variable BASE_LOG_LEVEL to define the 
         * requested logging level. 
         * 
         * A library designer can decide using the BASE_LOG_xxx flag at compile time which log level
         * should be available. 
         * 
         * If a different output stream is requested BASE_LOG_CONFIGURE(priority,ostream)
         * can be used.
         * Ostream request a FILE* ptr 
         * 
	 */
	class Logger : public Singleton<Logger>
	{
            friend class Singleton<Logger>;

        protected:
		/**
		 * Construct the logger
		 */
		Logger();
	public:

		virtual ~Logger();

                /** 
                * Configure logger - this is for the library developer so he can set a maximum log
                * level, which cannot be further limited to higher log priorities via setting BASE_LOG_LEVEL
                * If no previous configuration is given, no output logging will be done
                */
                void configure(Priority priority, FILE* outputStream);
		
		/**
 		* Logs a message with a given priority, can be used with printf style format
 		* @param priority priority level
                * @param ns namespace to be used
                * @param filename Filename
                * @param line Linenumber
 		* @param format printf like format string
 		* @param ... variable argument list
 		*/
		void log(Priority priority, const char* filename, int line, const char* format, ...);

	private:
                /**
                * Retrieve the log level from the enviroment
                */
                Priority getLogLevelFromEnv();

                FILE* mStream;
                std::vector<std::string> mPriorityNames;
                Priority mPriority;

	};

} // end namespace
} // end namespace

#endif /* _BASE_LOGGER_H_ */

