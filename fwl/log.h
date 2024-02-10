#ifndef __H_FWL_LOG__
#define __H_FWL_LOG__

/**
* @file log.h
* @brief ��־ģ���װ
* @author fwl
* @email 1822363660@qq.com
* @data 2023-10-06
*/


#include"base_head.h"
#include"unit.h"
#include"singletion.h"
#include"thread.h"
#include"mutex.h"

#define PATTERN_SYMBLE '%'
#define ERROR_FORMAT_PATTERN "Error fromat pattern!"
#define DEFAULT_TIME_PATTERN "%d{%Y-%m-%d %H:%M:%S}[%p][%t %N %F][%c %l]%m%n"
#define FILETYPESTR "filelogappender"
#define STDOUTTYPESTR "stdoutlogappender"
#define DEFAULT_LOG_LEVEL LogLevel::DEBUG

/**
* @ brief ������ʽд����־
* */
#define FWL_LOG_LEVEL(logger,level) \
    fwl::LogEventWarp(fwl::LogEvent::ptr(new fwl::LogEvent(logger,level,__FILE__,__LINE__,0,fwl::getThreadId(),fwl::getThreadName(),fwl::getFiberId(),fwl::getFiberName(),time(NULL)))).getSS()

/**
 * @breif ������ʽд��INFO��־
 * */
#define FWL_LOG_INFO(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::INFO)

/**
 * @breif ������ʽд��DEBUG��־
 * */
#define FWL_LOG_DEBUG(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::DEBUG)

/**
 * @breif ������ʽд��WARN��־
 * */
#define FWL_LOG_WARN(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::WARN)


/**
 * @breif ������ʽд��ERROR��־
 * */
#define FWL_LOG_ERROR(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::ERROR)


/**
 * @breif ������ʽд��FATAL��־
 * */
#define FWL_LOG_FATAL(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::FATAL)

/**
 * @brief Ĭ�ϻ�ȡname����־��
 * */
#define FWL_LOG_NAME(name) fwl::loggerMgr::getInstance()->getLogger(name)

/**
 * @brief Ĭ�ϻ�ȡname����־��
 * */
#define FWL_LOG_ROOT() fwl::loggerMgr::getInstance()->getRoot()

namespace fwl{

class Logger;

/**
* @brief ��־����
*/
class LogLevel {
public:
	/**
	* ��־����ö��
	*/
	enum Level {
		//δ֪����
		UNKNOWN = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5,
	    CLOSED = 100 //��ʾ����ʹ��
    };
    
    /**
     * @brief Level to string
     * */
    static const char * ToString(LogLevel::Level level);
    
    /**
     * @brief Level from string
     * */
    static LogLevel::Level FromString(const std::string& level_str);
};

/**
* @brief ��־ʱ��
*/

class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	/**
	* @brief ���캯��
	* @param[in] logger ��־��
	* @param[in] level ��־����
	* @param[in] file �ļ���
	* @param[in] line �ļ��к�
	* @param[in] elapse �������������ĺ�ʱ(����)
	* @param[in] thread_id �߳�id
	* @param[in] fiber_id Э��id
	* @param[in] time ��־ʱ��(��)
	* @param[in] thread_name �߳�����
    * @param[in] log_name ��־��
	*/
	LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, int32_t runtime, uint32_t threadId,const std::string &threadName, uint32_t fiberId,const std::string& fiberName,uint64_t time);

	/**
	* @breif �����ļ���
	*/
	const char* getFile() const{
		return m_file;
	}

	/**
	* @breif �����к�
	*/
	int32_t getLine() const{
		return m_line;
	}

	/**
	* @breif ���س���������ʱ
	*/
	int32_t getRuntime() const {
		return m_runtime;
	}

	/**
	* @breif ����Э��ID
	*/
	uint32_t getFiberId() const {
		return m_fiberId;
	}
		
    /**
        * @brief ����Э����
        */
    std::string getFiberName() const {
        return m_fiberName;
    }

	/**
	* @breif �����߳�ID
	*/
	uint32_t getThreadId() const {
		return m_threadId;
	}

    /**
	* @breif �����߳�����
	*/
	std::string getThreadName() const {
		return m_threadName;
	}

	/**
	* @breif ������־ʱ��
	*/
	uint64_t getTime() const {
		return m_time;
	}

	/**
        *@brief ������־����
        **/
    std::string getContent() const {
        return m_ss.str();
    }

	/**
	* @breif ������־����
	*/
	LogLevel::Level getLogLevel() const {
		return m_level;
	}

	/**
	* @breif ������־��
	*/
    std::shared_ptr<Logger> getLogger() const {
		return m_logger;
	}
        
    /**
        * @brief �����ַ�����
        * */
    std::stringstream& getSS(){
        return m_ss;
    } 
    
private:
	//�ļ���
	const char * m_file = nullptr;
	//�ļ��к�
	int32_t m_line;
	//�������������ĺ�ʱ
	int32_t m_runtime = 0;
	//�߳�id
	uint32_t m_threadId = 0;
   	//�߳�����
	std::string m_threadName;
    //Э��id
	uint32_t m_fiberId = 0;
	//Э����
    std::string m_fiberName;
    //��־ʱ��
    uint64_t m_time = 0;
	//��־��
	std::shared_ptr<Logger> m_logger;
	//��־����
	LogLevel::Level m_level;
    //��־����
    std::string m_content;
	//�ַ�����
    std::stringstream m_ss;
};

/**
 * @brief ��־��װ��
 * */
class LogEventWarp{
public:
    /**
     * @brief ���캯��
     * @param[in] ��־�¼�
     * */
    LogEventWarp(LogEvent::ptr e);

    /**
     *  @brief ��������
     * */
    ~LogEventWarp();
    
    /**
     * @brief ����stringstream
     * */
    std::stringstream& getSS();

    /**
     * @brief ����event
     * */
    LogEvent::ptr getEvent() const;

private:
    //��־�¼�
    LogEvent::ptr m_event;
};

/**
* @brief ��־��ʽ����
*/
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	/**
	* @brief ���캯��
    * @param[in] pattern ��ʽģ��
    * @details
    *  %m ��Ϣ
    *  %p ��־����
    *  %r �ۼƺ�����
    *  %c ��־����
    *  %t �߳�id
    *  %n ����
    *  %d ʱ��
    *  %f �ļ���
    *  %l �к�
    *  %T �Ʊ��
    *  %F Э��id
    *  %N �߳�����
	*/
	LogFormatter(const std::string&);
        
    /**
    * @brief isError
    * @param[out] �Ƿ��д���
    * */
    bool isError(){ return m_error; }

    /**
    * @brief ����format��ʽ�ַ���
    * @param[out] �����ַ���
    * */
    std::string getPattern(){ return m_pattern; }

	/**
	* @brief format����
	* @param[in,out] os
	* @param[in] logger
	* @param[in] level
	* @param[in] event
    */
    std::ostream& format(std::ostream& ofs,std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);
    std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);

public:
	/**
	* @brief ��־�ڲ����ʽ��-����ÿ������ʽ��
	*/
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem> ptr;
            
        /**
		* @brief ��������
		*/
		virtual ~FormatItem() {};

		/**
		* @brief format����
		* @param[in,out] os
		* @param[in] logger
		* @param[in] level
		* @param[in] event
		*/
		virtual void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};
private:
    //��ʼ������
    void init();

	//��־��ʽģ��
	std::string m_pattern;
	//��־�������ʽ
	std::vector<FormatItem::ptr> m_items;
	//�Ƿ��д���
	bool m_error = false;
    //
};

/**
* @brief ��־���Ŀ��
*/
class LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
    //��������
    //typedef Mutex MutexType;
	
    //������
    typedef SpinMutex MutexType;

	/**
	* ��������
	*/
	virtual ~LogAppender() {};

	/**
	* ����fomatter
	*/
	void setFormatter(LogFormatter::ptr formatter);

	/**
	* ��ȡfomatter
	*/
	LogFormatter::ptr getFormatter() { return m_formatter; };

	/**
	* ��ȡlevel
	*/
	LogLevel::Level getLevel() { return m_level; };

	/**
	* ����level
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* �Ƿ�����־��ʽ����
	*/
	bool hasFormatter() { return m_has_fomatter; }

	/**
	* ��־д��
	*/
	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level loglevel, LogEvent::ptr event) = 0;

    /**
	* @breif toYAMLString()
	*/
    virtual std::string toYAMLString() =  0;

protected:
	//��¼�Ƿ�����־��ʽ����
	bool m_has_fomatter = false;
	//��־��ʽ�����Ա
	std::shared_ptr<LogFormatter> m_formatter = nullptr;
	//��־�ȼ�
	LogLevel::Level m_level = DEFAULT_LOG_LEVEL;
    //��
    MutexType m_mutex;
};


/**
* ��־���Ŀ��-�ļ�
* �̳�LogAppender
*/
class FileLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<FileLogAppender> ptr;

	/**
	* @breif ���캯��
	*/
	FileLogAppender(const std::string & filename);

	/**
	* @breif ���´��ļ�
	*/
	bool reopen();

	/**
	* @breif д��־
	* @param[in] logger ��־��
	* @param[in] level ��־�ȼ�
	* @param[in] event ��־�¼�
	*/
	void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
	
    /**
	* @breif toYAMLString()
	*/
    std::string toYAMLString() override;

private:
	std::string m_filename;
	std::ofstream m_filestream;
	//�ϴ����´�ʱ��
	uint64_t m_lastTime = 0;
};

/**
* @brief��־���Ŀ��-��׼���
* @details �̳�LogAppender
*/
class StdoutLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;

	void log(std::shared_ptr<Logger> logger, LogLevel::Level loglevel, LogEvent::ptr event) override;

    /**
	* @breif toYAMLString()
	*/
    std::string toYAMLString() override;
};

/**
* @brief ��־��
*/
class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
	typedef std::shared_ptr<Logger> ptr;
    
    //��������
    //typedef Mutex MutexType;
	
    //������
    typedef SpinMutex MutexType;

	/**
	* @breif ���캯��
	*/
	Logger(const std::string & str = "root");

	/**
	* @breif дFATAl������־
	* @param[in] event ��־�¼�
	*/
	void fatal(LogEvent::ptr event);

	/**
	* @breif дERROR������־
	* @param[in] event ��־�¼�
	*/
	void error(LogEvent::ptr event);

	/**
	* @breif дWARN������־
	* @param[in] event ��־�¼�
	*/
	void warn(LogEvent::ptr event);

	/**
	* @breif дINFO������־
	* @param[in] event ��־�¼�
	*/
	void info(LogEvent::ptr event);

	/**
	* @breif дDEBUG������־
	* @param[in] event ��־�¼�
	*/
	void debug(LogEvent::ptr event);

	/**
	* @breif ������־�ȼ� 
	* @param[in] level ��־�ȼ�
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* @brief ��ȡ��־�ȼ�
	*/
	LogLevel::Level getLevel() const { return m_level; };

	/**
	* @breif ׷����־Ŀ��
	* @param[in] logAppender ��־Ŀ��
	*/
	void addLogAppender(LogAppender::ptr logAppender);

	/**
	* @brief ɾ��Ŀ��Ŀ��
	*/
	void delLogAppenders(LogAppender::ptr logAppender);

	/**
	* @brief �����־Ŀ�꼯��
	*/
	void clearLogAppenders();

	/**
	* @brief ������־������-ͨ��formatter��ָ��
	*/
	void setLogFormatter(LogFormatter::ptr formatter);

	/**
	* @brief ������־������-ͨ��str��ʽ
	*/
	void setLogFormatter(const std::string & name);

	/**
	* @brief ������־������
	*/
	LogFormatter::ptr GetLogFormatter() const{ return m_formatter;  };

	/**
	* @breif д��־
	* @param[in] level ��־����
	* @param[in] event ��־ʱ��
	*/
	void log(LogLevel::Level level,LogEvent::ptr event);

	/**
	* @breif toYAMLString
	*/
    std::string toYAMLString();

private:
	//��־����
	std::string m_name;
	//��־����
	LogLevel::Level m_level;
	//��־Ŀ�꼯��
	std::vector<LogAppender::ptr> m_appenders;
	//��־������
    std::shared_ptr<LogFormatter> m_formatter;
	//����־��
	std::shared_ptr<Logger> m_root;
    //��
    MutexType m_mutex;
};

/**
 * @brief 
 * */
class LoggerManager{
public:
    //��������
    //typedef Mutex MutexType;
	
    //������
    typedef SpinMutex MutexType;
    
    /**
     * @brief ���캯��
     * */
    LoggerManager();

    /**
     * @brief ��ȡLogger
     * @param[in] logger��
     * @param[out] ��ѯ��logger
     * */
    Logger::ptr getLogger(const std::string & name);


    /**
     * @brief ��ȡ����־��
     * @param[out] ��ѯ��logger
     * */
    Logger::ptr getRoot() const { return m_root; };
    
    /**
     * @brief toYAMLString(����yaml��ʽչʾ)
     * */
    std::string toYAMLString();

private:
    //logger����
    std::map<std::string,Logger::ptr> m_loggers;
    //����־��
    Logger::ptr m_root;
    //��
    MutexType m_mutex;
};


/**
 * @brief appender���ͣ�0��ʾ�ļ���1��ʾstdout
 * */
enum AppenderType{
    FILE = 0,
    STDOUT =  1
};

/**
 * @brief LogAppenderDefine
 * @details ��־�ļ�appender����
 * */
class LogAppenderDefine{
public:
    LogAppenderDefine& operator=(LogAppenderDefine & in){
        file = in.file;
        level = in.level;
        formatter = in.formatter;
        type = in.type;
        return *this;
    }

    bool operator==(const LogAppenderDefine & in) const{
        return file == in.file 
            && level == in.level
            && formatter == in.formatter
            && type == in.type;
    }
public:
    //��־�ȼ�
    LogLevel::Level level;
    //��־������
    std::string formatter;
    //appender����
    AppenderType type;
    //�ļ���
    std::string file;
};


/**
 * @brief LogDefine
 * @details ��־�ֶ��ļ��ඨ��
 * */
class LogDefine{
public:
    typedef std::shared_ptr<LogDefine> ptr;
    
    bool operator==(const LogDefine & in) const{
        return m_name == in.m_name
            && m_level == in.m_level
            && m_formatter == in.m_formatter
            && m_appenders == in.m_appenders;
    }

    bool operator<(const LogDefine & in) const{
        return m_name < in.m_name;
    }

public:
    //��־��
    std::string m_name;
    //��־�ȼ�
    LogLevel::Level m_level = LogLevel::Level::UNKNOWN;
    //��־������
    std::string m_formatter;
    //appenders����
    std::vector<LogAppenderDefine> m_appenders;
};

typedef fwl::SingletionPtr<LoggerManager> loggerMgr;
    
}
#endif

