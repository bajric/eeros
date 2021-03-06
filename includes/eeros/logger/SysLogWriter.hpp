#ifndef ORG_EEROS_LOGGER_SYSLOGWRITER_HPP_
#define ORG_EEROS_LOGGER_SYSLOGWRITER_HPP_

#include <eeros/logger/LogWriter.hpp>
#include <sstream>
#include <mutex>

namespace eeros {
	namespace logger {
		class SysLogWriter : public LogWriter {
		public:
			SysLogWriter(const std::string name);
			virtual ~SysLogWriter();
			
			virtual void show(unsigned level = ~0);
			
			virtual void begin(unsigned level, unsigned category);
			virtual void end();
			
			virtual void endl();
			
			virtual LogWriter& operator<<(int value);
			virtual LogWriter& operator<<(unsigned int value);
			virtual LogWriter& operator<<(long value);
			virtual LogWriter& operator<<(double value);
			virtual LogWriter& operator<<(const std::string& value);
			virtual LogWriter& operator<<(std::ostream& os);
			virtual LogWriter& operator<<(void (*f)(LogWriter&));
			
		private:
			std::string name;
			std::ostringstream out;
			unsigned level;
			std::mutex mtx;
			std::unique_lock<std::mutex> lck;
			unsigned visibleLevel;
			bool enabled;
		};
	}
}

#endif /* ORG_EEROS_LOGGER_SYSLOGWRITER_HPP_ */
