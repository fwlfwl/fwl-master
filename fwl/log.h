#ifndef __H_FWL_LOG__
#define __H_FWL_LOG__

/**
* @file log.h
* @brief 日志模块封装
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
* @ brief 以流方式写入日志
* */
#define FWL_LOG_LEVEL(logger,level) \
    fwl::LogEventWarp(fwl::LogEvent::ptr(new fwl::LogEvent(logger,level,__FILE__,__LINE__,0,fwl::getThreadId(),fwl::getThreadName(),fwl::getFiberId(),fwl::getFiberName(),time(NULL)))).getSS()

/**
 * @breif 以流方式写入INFO日志
 * */
#define FWL_LOG_INFO(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::INFO)

/**
 * @breif 以流方式写入DEBUG日志
 * */
#define FWL_LOG_DEBUG(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::DEBUG)

/**
 * @breif 以流方式写入WARN日志
 * */
#define FWL_LOG_WARN(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::WARN)


/**
 * @breif 以流方式写入ERROR日志
 * */
#define FWL_LOG_ERROR(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::ERROR)


/**
 * @breif 以流方式写入FATAL日志
 * */
#define FWL_LOG_FATAL(logger) \
    FWL_LOG_LEVEL(logger,fwl::LogLevel::FATAL)

/**
 * @brief 默认获取name的日志器
 * */
#define FWL_LOG_NAME(name) fwl::loggerMgr::getInstance()->getLogger(name)

/**
 * @brief 默认获取name的日志器
 * */
#define FWL_LOG_ROOT() fwl::loggerMgr::getInstance()->getRoot()

namespace fwl{

class Logger;

/**
* @brief 日志级别
*/
class LogLevel {
public:
	/**
	* 日志级别枚举
	*/
	enum Level {
		//未知级别
		UNKNOWN = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5,
	    CLOSED = 100 //表示不再使用
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
* @brief 日志时间
*/

class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	/**
	* @brief 构造函数
	* @param[in] logger 日志器
	* @param[in] level 日志级别
	* @param[in] file 文件名
	* @param[in] line 文件行号
	* @param[in] elapse 程序启动依赖的耗时(毫秒)
	* @param[in] thread_id 线程id
	* @param[in] fiber_id 协程id
	* @param[in] time 日志时间(秒)
	* @param[in] thread_name 线程名称
    * @param[in] log_name 日志名
	*/
	LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, int32_t runtime, uint32_t threadId,const std::string &threadName, uint32_t fiberId,const std::string& fiberName,uint64_t time);

	/**
	* @breif 返回文件名
	*/
	const char* getFile() const{
		return m_file;
	}

	/**
	* @breif 返回行号
	*/
	int32_t getLine() const{
		return m_line;
	}

	/**
	* @breif 返回程序启动耗时
	*/
	int32_t getRuntime() const {
		return m_runtime;
	}

	/**
	* @breif 返回协程ID
	*/
	uint32_t getFiberId() const {
		return m_fiberId;
	}
		
    /**
        * @brief 返回协程名
        */
    std::string getFiberName() const {
        return m_fiberName;
    }

	/**
	* @breif 返回线程ID
	*/
	uint32_t getThreadId() const {
		return m_threadId;
	}

    /**
	* @breif 返回线程名称
	*/
	std::string getThreadName() const {
		return m_threadName;
	}

	/**
	* @breif 返回日志时间
	*/
	uint64_t getTime() const {
		return m_time;
	}

	/**
        *@brief 返回日志内容
        **/
    std::string getContent() const {
        return m_ss.str();
    }

	/**
	* @breif 返回日志级别
	*/
	LogLevel::Level getLogLevel() const {
		return m_level;
	}

	/**
	* @breif 返回日志器
	*/
    std::shared_ptr<Logger> getLogger() const {
		return m_logger;
	}
        
    /**
        * @brief 返回字符串流
        * */
    std::stringstream& getSS(){
        return m_ss;
    } 
    
private:
	//文件名
	const char * m_file = nullptr;
	//文件行号
	int32_t m_line;
	//程序启动依赖的耗时
	int32_t m_runtime = 0;
	//线程id
	uint32_t m_threadId = 0;
   	//线程名称
	std::string m_threadName;
    //协程id
	uint32_t m_fiberId = 0;
	//协程名
    std::string m_fiberName;
    //日志时间
    uint64_t m_time = 0;
	//日志器
	std::shared_ptr<Logger> m_logger;
	//日志级别
	LogLevel::Level m_level;
    //日志内容
    std::string m_content;
	//字符串流
    std::stringstream m_ss;
};

/**
 * @brief 日志包装类
 * */
class LogEventWarp{
public:
    /**
     * @brief 构造函数
     * @param[in] 日志事件
     * */
    LogEventWarp(LogEvent::ptr e);

    /**
     *  @brief 析构函数
     * */
    ~LogEventWarp();
    
    /**
     * @brief 返回stringstream
     * */
    std::stringstream& getSS();

    /**
     * @brief 返回event
     * */
    LogEvent::ptr getEvent() const;

private:
    //日志事件
    LogEvent::ptr m_event;
};

/**
* @brief 日志格式化类
*/
class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	/**
	* @brief 构造函数
    * @param[in] pattern 格式模板
    * @details
    *  %m 消息
    *  %p 日志级别
    *  %r 累计毫秒数
    *  %c 日志名称
    *  %t 线程id
    *  %n 换行
    *  %d 时间
    *  %f 文件名
    *  %l 行号
    *  %T 制表符
    *  %F 协程id
    *  %N 线程名称
	*/
	LogFormatter(const std::string&);
        
    /**
    * @brief isError
    * @param[out] 是否有错误
    * */
    bool isError(){ return m_error; }

    /**
    * @brief 返回format格式字符串
    * @param[out] 返回字符串
    * */
    std::string getPattern(){ return m_pattern; }

	/**
	* @brief format函数
	* @param[in,out] os
	* @param[in] logger
	* @param[in] level
	* @param[in] event
    */
    std::ostream& format(std::ostream& ofs,std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);
    std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);

public:
	/**
	* @brief 日志内部项格式化-根据每个类别格式化
	*/
	class FormatItem {
	public:
		typedef std::shared_ptr<FormatItem> ptr;
            
        /**
		* @brief 析构函数
		*/
		virtual ~FormatItem() {};

		/**
		* @brief format函数
		* @param[in,out] os
		* @param[in] logger
		* @param[in] level
		* @param[in] event
		*/
		virtual void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};
private:
    //初始化函数
    void init();

	//日志格式模板
	std::string m_pattern;
	//日志解析后格式
	std::vector<FormatItem::ptr> m_items;
	//是否有错误
	bool m_error = false;
    //
};

/**
* @brief 日志输出目标
*/
class LogAppender {
public:
	typedef std::shared_ptr<LogAppender> ptr;
    //互斥量锁
    //typedef Mutex MutexType;
	
    //自旋锁
    typedef SpinMutex MutexType;

	/**
	* 析构函数
	*/
	virtual ~LogAppender() {};

	/**
	* 设置fomatter
	*/
	void setFormatter(LogFormatter::ptr formatter);

	/**
	* 获取fomatter
	*/
	LogFormatter::ptr getFormatter() { return m_formatter; };

	/**
	* 获取level
	*/
	LogLevel::Level getLevel() { return m_level; };

	/**
	* 设置level
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* 是否有日志格式化器
	*/
	bool hasFormatter() { return m_has_fomatter; }

	/**
	* 日志写入
	*/
	virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level loglevel, LogEvent::ptr event) = 0;

    /**
	* @breif toYAMLString()
	*/
    virtual std::string toYAMLString() =  0;

protected:
	//记录是否有日志格式化器
	bool m_has_fomatter = false;
	//日志格式化类成员
	std::shared_ptr<LogFormatter> m_formatter = nullptr;
	//日志等级
	LogLevel::Level m_level = DEFAULT_LOG_LEVEL;
    //锁
    MutexType m_mutex;
};


/**
* 日志输出目标-文件
* 继承LogAppender
*/
class FileLogAppender : public LogAppender {
public:
	typedef std::shared_ptr<FileLogAppender> ptr;

	/**
	* @breif 构造函数
	*/
	FileLogAppender(const std::string & filename);

	/**
	* @breif 重新打开文件
	*/
	bool reopen();

	/**
	* @breif 写日志
	* @param[in] logger 日志器
	* @param[in] level 日志等级
	* @param[in] event 日志事件
	*/
	void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
	
    /**
	* @breif toYAMLString()
	*/
    std::string toYAMLString() override;

private:
	std::string m_filename;
	std::ofstream m_filestream;
	//上次重新打开时间
	uint64_t m_lastTime = 0;
};

/**
* @brief日志输出目标-标准输出
* @details 继承LogAppender
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
* @brief 日志器
*/
class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
	typedef std::shared_ptr<Logger> ptr;
    
    //互斥量锁
    //typedef Mutex MutexType;
	
    //自旋锁
    typedef SpinMutex MutexType;

	/**
	* @breif 构造函数
	*/
	Logger(const std::string & str = "root");

	/**
	* @breif 写FATAl级别日志
	* @param[in] event 日志事件
	*/
	void fatal(LogEvent::ptr event);

	/**
	* @breif 写ERROR级别日志
	* @param[in] event 日志事件
	*/
	void error(LogEvent::ptr event);

	/**
	* @breif 写WARN级别日志
	* @param[in] event 日志事件
	*/
	void warn(LogEvent::ptr event);

	/**
	* @breif 写INFO级别日志
	* @param[in] event 日志事件
	*/
	void info(LogEvent::ptr event);

	/**
	* @breif 写DEBUG级别日志
	* @param[in] event 日志事件
	*/
	void debug(LogEvent::ptr event);

	/**
	* @breif 设置日志等级 
	* @param[in] level 日志等级
	*/
	void setLevel(LogLevel::Level level) { m_level = level; }

	/**
	* @brief 获取日志等级
	*/
	LogLevel::Level getLevel() const { return m_level; };

	/**
	* @breif 追加日志目标
	* @param[in] logAppender 日志目标
	*/
	void addLogAppender(LogAppender::ptr logAppender);

	/**
	* @brief 删除目标目标
	*/
	void delLogAppenders(LogAppender::ptr logAppender);

	/**
	* @brief 清空日志目标集合
	*/
	void clearLogAppenders();

	/**
	* @brief 设置日志编码器-通过formatter类指针
	*/
	void setLogFormatter(LogFormatter::ptr formatter);

	/**
	* @brief 设置日志编码器-通过str格式
	*/
	void setLogFormatter(const std::string & name);

	/**
	* @brief 返回日志编码器
	*/
	LogFormatter::ptr GetLogFormatter() const{ return m_formatter;  };

	/**
	* @breif 写日志
	* @param[in] level 日志级别
	* @param[in] event 日志时间
	*/
	void log(LogLevel::Level level,LogEvent::ptr event);

	/**
	* @breif toYAMLString
	*/
    std::string toYAMLString();

private:
	//日志名称
	std::string m_name;
	//日志级别
	LogLevel::Level m_level;
	//日志目标集合
	std::vector<LogAppender::ptr> m_appenders;
	//日志编码器
    std::shared_ptr<LogFormatter> m_formatter;
	//主日志器
	std::shared_ptr<Logger> m_root;
    //锁
    MutexType m_mutex;
};

/**
 * @brief 
 * */
class LoggerManager{
public:
    //互斥量锁
    //typedef Mutex MutexType;
	
    //自旋锁
    typedef SpinMutex MutexType;
    
    /**
     * @brief 构造函数
     * */
    LoggerManager();

    /**
     * @brief 获取Logger
     * @param[in] logger名
     * @param[out] 查询的logger
     * */
    Logger::ptr getLogger(const std::string & name);


    /**
     * @brief 获取主日志器
     * @param[out] 查询的logger
     * */
    Logger::ptr getRoot() const { return m_root; };
    
    /**
     * @brief toYAMLString(按照yaml格式展示)
     * */
    std::string toYAMLString();

private:
    //logger集和
    std::map<std::string,Logger::ptr> m_loggers;
    //主日志器
    Logger::ptr m_root;
    //锁
    MutexType m_mutex;
};


/**
 * @brief appender类型，0表示文件，1表示stdout
 * */
enum AppenderType{
    FILE = 0,
    STDOUT =  1
};

/**
 * @brief LogAppenderDefine
 * @details 日志文件appender定义
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
    //日志等级
    LogLevel::Level level;
    //日志编码器
    std::string formatter;
    //appender类型
    AppenderType type;
    //文件名
    std::string file;
};


/**
 * @brief LogDefine
 * @details 日志字段文件类定义
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
    //日志名
    std::string m_name;
    //日志等级
    LogLevel::Level m_level = LogLevel::Level::UNKNOWN;
    //日志解码器
    std::string m_formatter;
    //appenders类型
    std::vector<LogAppenderDefine> m_appenders;
};

typedef fwl::SingletionPtr<LoggerManager> loggerMgr;
    
}
#endif

