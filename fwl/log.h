#ifndef __H_FWL_LOG__
#define __H_FWL_LOG__

/**
* @file log.h
* @brief ¿¿¿
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
* @ brief ¿¿¿¿
* */
#define FWL_LOG_LEVEL(logger,level) \
    fwl::LogEventWarp(fwl::LogEvent::ptr(new fwl::LogEvent(logger,level,__FILE__,__LINE__,0,fwl::getThreadId(),fwl::getThreadName(),fwl::getFiberId(),fwl::getFiberName(),time(NULL)))).getSS()

/**
 * @breif ¿¿¿¿-info¿¿
 * */
#define FWL_LOG_INFO(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::INFO)

/**
 * @breif ¿¿¿¿-debug¿¿
 * */
#define FWL_LOG_DEBUG(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::DEBUG)

/**
 * @breif ¿¿¿¿-warn¿¿
 * */
#define FWL_LOG_WARN(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::WARN)


/**
 * @breif ¿¿¿¿-error¿¿
 * */
#define FWL_LOG_ERROR(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::ERROR)


/**
 * @breif ¿¿¿¿-fatal¿¿
 * */
#define FWL_LOG_FATAL(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::FATAL)

/**
 * @brief ¿¿¿¿¿-¿¿¿¿¿
 * */
#define FWL_LOG_NAME(name) fwl::loggerMgr::getInstance()->getLogger(name)

/**
 * @brief ¿¿¿¿¿-root
 * */
#define FWL_LOG_ROOT() fwl::loggerMgr::getInstance()->getRoot()

namespace fwl{

class Logger;

/**
* @brief ÈÕÖ¾¼¶±ğ
*/
class LogLevel {
public:
	/**
	* ÈÕÖ¾¼¶±ğÃ¶¾Ù
	*/
	enum Level {
		//Î´Öª¼¶±ğ
		UNKNOWN = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5,
	    CLOSED = 100 //±íÊ¾²»ÔÙÊ¹ÓÃ
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
* @brief ÈÕÖ¾Ê±¼ä
*/

class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	/**
	* @brief ¹¹Ôìº¯Êı
	* @param[in] logger ÈÕÖ¾Æ÷
	* @param[in] level ÈÕÖ¾¼¶±ğ
	* @param[in] file ÎÄ¼şÃû
	* @param[in] line ÎÄ¼şĞĞºÅ
	* @param[in] elapse ³ÌĞòÆô¶¯ÒÀÀµµÄºÄÊ±(ºÁÃë)
	* @param[in] thread_id Ïß³Ìid
	* @param[in] fiber_id Ğ­³Ìid
	* @param[in] time ÈÕÖ¾Ê±¼ä(Ãë)
	* @param[in] thread_name Ïß³ÌÃû³Æ
    * @param[in] log_name ÈÕÖ¾Ãû
	*/
	LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, int32_t runtime, uint32_t threadId,const std::string &threadName, uint32_t fiberId,const std::string& fiberName,uint64_t time);

	/**
	* @breif ·µ»ØÎÄ¼şÃû
	*/
	const char* getFile() const{
		return m_file;
	}

	/**
	* @breif ·µ»ØĞĞºÅ
	*/
	int32_t getLine() const{
		return m_line;
	}

	/**
	* @breif ·µ»Ø³ÌĞòÆô¶¯ºÄÊ±
	*/
	int32_t getRuntime() const {
		return m_runtime;
	}

	/**
	* @breif ·µ»ØĞ­³ÌID
	*/
	uint32_t getFiberId() const {
		return m_fiberId;
	}
		
    /**
        * @brief ·µ»ØĞ­³ÌÃû
        */
    std::string getFiberName() const {
        return m_fiberName;
    }

	/**
	* @breif ·µ»ØÏß³ÌID
	*/
	uint32_t getThreadId() const {
		return m_threadId;
	}

    /**
	* @breif ·µ»ØÏß³ÌÃû³Æ
	*/
	std::string getThreadName() const {
		return m_threadName;
	}

	/**
	* @breif ·µ»ØÈÕÖ¾Ê±¼ä
	*/
	uint64_t getTime() const {
		return m_time;
	}

	/**
        *@brief ·µ»ØÈÕÖ¾ÄÚÈİ
        **/
    std::string getContent() const {
        return m_ss.str();
    }

	/**
	* @breif ·µ»ØÈÕÖ¾¼¶±ğ
	*/
	LogLevel::Level getLogLevel() const {
		return m_level;
	}

	/**
	* @breif ·µ»ØÈÕÖ¾Æ÷
	*/
    std::shared_ptr<Logger> getLogger() const {
		return m_logger;
	}
        
    /**
        * @brief ·µ»Ø×Ö·û´®Á÷
        * */
    std::stringstream& getSS(){
        return m_ss;
    } 
    
private:
	//ÎÄ¼şÃû
	const char * m_file = nullptr;
	//ÎÄ¼şĞĞºÅ
	int32_t m_line;
	//³ÌĞòÆô¶¯ÒÀÀµµÄºÄÊ±
	int32_t m_runtime = 0;
	//Ïß³Ìid
	uint32_t m_threadId = 0;
   	//Ïß³ÌÃû³Æ
	std::string m_threadName;
    //Ğ­³Ìid
	uint32_t m_fiberId = 0;
	//Ğ­³ÌÃû
    std::string m_fiberName;
    //ÈÕÖ¾Ê±¼ä
    uint64_t m_time = 0;
	//ÈÕÖ¾Æ÷
	std::shared_ptr<Logger> m_logger;
	//ÈÕÖ¾¼¶±ğ
	LogLevel::Level m_level;
    //ÈÕÖ¾ÄÚÈİ
    std::string m_content;
	//×Ö·û´®Á÷
    std::stringstream m_ss;
};

/**
 * @brief ÈÕÖ¾°ü×°Àà
 * */
class LogEventWarp{
public:
    /**
     * @brief ¹¹Ôìº¯Êı
     * @param[in] ÈÕÖ¾ÊÂ¼ş
     * */
    LogEventWarp(LogEvent::ptr e);

    /**
     *  @brief Îö¹¹º¯Êı
     * */
    ~LogEventWarp();
    
    /**
     * @brief ·µ»Østringstream
     * */
    std::stringstream& getSS();

    /**
     * @brief ·µ»Øevent
     * */
    LogEvent::ptr getEvent() const;

private:
    //ÈÕÖ¾ÊÂ¼ş
    LogEvent::ptr m_event;
};

/**
* @brief ÈÕÖ¾¸ñÊ½»¯Àà
*/
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	/**
	* @brief ¹¹Ôìº¯Êı
    * @param[in] pattern ¸ñÊ½Ä£°å
    * @details
    *  %m ÏûÏ¢
    *  %p ÈÕÖ¾¼¶±ğ
    *  %r ÀÛ¼ÆºÁÃëÊı
    *  %c ÈÕÖ¾Ãû³Æ
    *  %t Ïß³Ìid
    *  %n »»ĞĞ
    *  %d Ê±¼ä
    *  %f ÎÄ¼şÃû
    *  %l ĞĞºÅ
    *  %T ÖÆ±í·û
    *  %F Ğ­³Ìid
    *  %N Ïß³ÌÃû³Æ
	*/
	LogFormatter(const std::string&);
        
    /**
    * @brief isError
    * @param[out] ÊÇ·ñÓĞ´íÎó
    * */
    bool isError(){ return m_error; }

    /**
    * @brief ·µ»Øformat¸ñÊ½×Ö·û´®
    * @param[out] ·µ»Ø×Ö·û´®
    * */
    std::string getPattern(){ return m_pattern; }

	/**
	* @brief formatº¯Êı
	* @param[in,out] os
	* @param[in] logger
	* @param[in] level
	* @param[in] event
    */
    std::ostream& format(std::ostream& ofs,std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);
    std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);

public:
	/**
	* @brief ÈÕÖ¾ÄÚ²¿Ïî¸ñÊ½»¯-¸ù¾İÃ¿¸öÀà±ğ¸ñÊ½»¯
	*/
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem> ptr;
            
        /**
		* @brief Îö¹¹º¯Êı
		*/
		virtual ~FormatItem() {};

		/**
		* @brief formatº¯Êı
		* @param[in,out] os
		* @param[in] logger
		* @param[in] level
		* @param[in] event
		*/
		virtual void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};
private:
    //³õÊ¼»¯º¯Êı
    void init();

	//ÈÕÖ¾¸ñÊ½Ä£°å
	std::string m_pattern;
	//ÈÕÖ¾½âÎöºó¸ñÊ½
	std::vector<FormatItem::ptr> m_items;
	//ÊÇ·ñÓĞ´íÎó
	bool m_error = false;
    //
};

/**
* @brief ÈÕÖ¾Êä³öÄ¿±ê
*/
class LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
    //»¥³âÁ¿Ëø
    //typedef Mutex MutexType;
	
    //×ÔĞıËø
    typedef SpinMutex MutexType;

	/**
	* Îö¹¹º¯Êı
	*/
	virtual ~LogAppender() {};

	/**
	* ÉèÖÃfomatter
	*/
	void setFormatter(LogFormatter::ptr formatter);

	/**
	* »ñÈ¡fomatter
	*/
	LogFormatter::ptr getFormatter() { return m_formatter; };

	/**
	* »ñÈ¡level
	*/
	LogLevel::Level getLevel() { return m_level; };

	/**
	* ÉèÖÃlevel
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* ÊÇ·ñÓĞÈÕÖ¾¸ñÊ½»¯Æ÷
	*/
	bool hasFormatter() { return m_has_fomatter; }

	/**
	* ÈÕÖ¾Ğ´Èë
	*/
	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level loglevel, LogEvent::ptr event) = 0;

    /**
	* @breif toYAMLString()
	*/
    virtual std::string toYAMLString() =  0;

protected:
	//¼ÇÂ¼ÊÇ·ñÓĞÈÕÖ¾¸ñÊ½»¯Æ÷
	bool m_has_fomatter = false;
	//ÈÕÖ¾¸ñÊ½»¯Àà³ÉÔ±
	std::shared_ptr<LogFormatter> m_formatter = nullptr;
	//ÈÕÖ¾µÈ¼¶
	LogLevel::Level m_level = DEFAULT_LOG_LEVEL;
    //Ëø
    MutexType m_mutex;
};


/**
* ÈÕÖ¾Êä³öÄ¿±ê-ÎÄ¼ş
* ¼Ì³ĞLogAppender
*/
class FileLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<FileLogAppender> ptr;

	/**
	* @breif ¹¹Ôìº¯Êı
	*/
	FileLogAppender(const std::string & filename);

	/**
	* @breif ÖØĞÂ´ò¿ªÎÄ¼ş
	*/
	bool reopen();

	/**
	* @breif Ğ´ÈÕÖ¾
	* @param[in] logger ÈÕÖ¾Æ÷
	* @param[in] level ÈÕÖ¾µÈ¼¶
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
	
    /**
	* @breif toYAMLString()
	*/
    std::string toYAMLString() override;

private:
	std::string m_filename;
	std::ofstream m_filestream;
	//ÉÏ´ÎÖØĞÂ´ò¿ªÊ±¼ä
	uint64_t m_lastTime = 0;
};

/**
* @briefÈÕÖ¾Êä³öÄ¿±ê-±ê×¼Êä³ö
* @details ¼Ì³ĞLogAppender
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
* @brief ÈÕÖ¾Æ÷
*/
class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
	typedef std::shared_ptr<Logger> ptr;
    
    //»¥³âÁ¿Ëø
    //typedef Mutex MutexType;
	
    //×ÔĞıËø
    typedef SpinMutex MutexType;

	/**
	* @breif ¹¹Ôìº¯Êı
	*/
	Logger(const std::string & str = "root");

	/**
	* @breif Ğ´FATAl¼¶±ğÈÕÖ¾
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void fatal(LogEvent::ptr event);

	/**
	* @breif Ğ´ERROR¼¶±ğÈÕÖ¾
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void error(LogEvent::ptr event);

	/**
	* @breif Ğ´WARN¼¶±ğÈÕÖ¾
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void warn(LogEvent::ptr event);

	/**
	* @breif Ğ´INFO¼¶±ğÈÕÖ¾
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void info(LogEvent::ptr event);

	/**
	* @breif Ğ´DEBUG¼¶±ğÈÕÖ¾
	* @param[in] event ÈÕÖ¾ÊÂ¼ş
	*/
	void debug(LogEvent::ptr event);

	/**
	* @breif ÉèÖÃÈÕÖ¾µÈ¼¶ 
	* @param[in] level ÈÕÖ¾µÈ¼¶
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* @brief »ñÈ¡ÈÕÖ¾µÈ¼¶
	*/
	LogLevel::Level getLevel() const { return m_level; };

	/**
	* @breif ×·¼ÓÈÕÖ¾Ä¿±ê
	* @param[in] logAppender ÈÕÖ¾Ä¿±ê
	*/
	void addLogAppender(LogAppender::ptr logAppender);

	/**
	* @brief É¾³ıÄ¿±êÄ¿±ê
	*/
	void delLogAppenders(LogAppender::ptr logAppender);

	/**
	* @brief Çå¿ÕÈÕÖ¾Ä¿±ê¼¯ºÏ
	*/
	void clearLogAppenders();

	/**
	* @brief ÉèÖÃÈÕÖ¾±àÂëÆ÷-Í¨¹ıformatterÀàÖ¸Õë
	*/
	void setLogFormatter(LogFormatter::ptr formatter);

	/**
	* @brief ÉèÖÃÈÕÖ¾±àÂëÆ÷-Í¨¹ıstr¸ñÊ½
	*/
	void setLogFormatter(const std::string & name);

	/**
	* @brief ·µ»ØÈÕÖ¾±àÂëÆ÷
	*/
	LogFormatter::ptr GetLogFormatter() const{ return m_formatter;  };

	/**
	* @breif Ğ´ÈÕÖ¾
	* @param[in] level ÈÕÖ¾¼¶±ğ
	* @param[in] event ÈÕÖ¾Ê±¼ä
	*/
	void log(LogLevel::Level level,LogEvent::ptr event);

	/**
	* @breif toYAMLString
	*/
    std::string toYAMLString();

private:
	//ÈÕÖ¾Ãû³Æ
	std::string m_name;
	//ÈÕÖ¾¼¶±ğ
	LogLevel::Level m_level;
	//ÈÕÖ¾Ä¿±ê¼¯ºÏ
	std::vector<LogAppender::ptr> m_appenders;
	//ÈÕÖ¾±àÂëÆ÷
    std::shared_ptr<LogFormatter> m_formatter;
	//Ö÷ÈÕÖ¾Æ÷
	std::shared_ptr<Logger> m_root;
    //Ëø
    MutexType m_mutex;
};

/**
 * @brief 
 * */
class LoggerManager{
public:
    //»¥³âÁ¿Ëø
    //typedef Mutex MutexType;
	
    //×ÔĞıËø
    typedef SpinMutex MutexType;
    
    /**
     * @brief ¹¹Ôìº¯Êı
     * */
    LoggerManager();

    /**
     * @brief »ñÈ¡Logger
     * @param[in] loggerÃû
     * @param[out] ²éÑ¯µÄlogger
     * */
    Logger::ptr getLogger(const std::string & name);


    /**
     * @brief »ñÈ¡Ö÷ÈÕÖ¾Æ÷
     * @param[out] ²éÑ¯µÄlogger
     * */
    Logger::ptr getRoot() const { return m_root; };
    
    /**
     * @brief toYAMLString(°´ÕÕyaml¸ñÊ½Õ¹Ê¾)
     * */
    std::string toYAMLString();

private:
    //logger¼¯ºÍ
    std::map<std::string,Logger::ptr> m_loggers;
    //Ö÷ÈÕÖ¾Æ÷
    Logger::ptr m_root;
    //Ëø
    MutexType m_mutex;
};


/**
 * @brief appenderÀàĞÍ£¬0±íÊ¾ÎÄ¼ş£¬1±íÊ¾stdout
 * */
enum AppenderType{
    FILE = 0,
    STDOUT =  1
};

/**
 * @brief LogAppenderDefine
 * @details ÈÕÖ¾ÎÄ¼şappender¶¨Òå
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
    //ÈÕÖ¾µÈ¼¶
    LogLevel::Level level;
    //ÈÕÖ¾±àÂëÆ÷
    std::string formatter;
    //appenderÀàĞÍ
    AppenderType type;
    //ÎÄ¼şÃû
    std::string file;
};


/**
 * @brief LogDefine
 * @details ÈÕÖ¾×Ö¶ÎÎÄ¼şÀà¶¨Òå
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
    //ÈÕÖ¾Ãû
    std::string m_name;
    //ÈÕÖ¾µÈ¼¶
    LogLevel::Level m_level = LogLevel::Level::UNKNOWN;
    //ÈÕÖ¾½âÂëÆ÷
    std::string m_formatter;
    //appendersÀàĞÍ
    std::vector<LogAppenderDefine> m_appenders;
};

typedef fwl::SingletionPtr<LoggerManager> loggerMgr;
    
}
#endif

