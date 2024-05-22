#include"base_head.h"
#include"log.h"
#include"config.h"

namespace fwl{

//LogLevel类实现
const char * LogLevel::ToString(LogLevel::Level level){
    switch(level){
#define FWL_XX(name) \
    case LogLevel::name: \
        return #name; \
        break;
    
    FWL_XX(DEBUG);
    FWL_XX(INFO);
    FWL_XX(WARN);
    FWL_XX(ERROR);
    FWL_XX(FATAL);
#undef FWL_XX
    default:
        return "UNKOWN";
    }
    return "UNKOWN";
}

LogLevel::Level LogLevel::FromString(const std::string & level_str){
#define FWL_XX(level,str) \
    if(level_str ==  #str){ \
        return LogLevel::level; \
    }

    FWL_XX(DEBUG,debug)
    FWL_XX(INFO,info)
    FWL_XX(WARN,warn)
    FWL_XX(ERROR,error)
    FWL_XX(FATAL,fatal)

    FWL_XX(DEBUG,DEBUG)
    FWL_XX(INFO,INFO)
    FWL_XX(WARN,WARN)
    FWL_XX(ERROR,ERROR)
    FWL_XX(FATAL,FATAL)
#undef FWL_XX
    return LogLevel::UNKNOWN;
}

//LogEvent类实现
LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, int32_t runtime, uint32_t threadId, const std::string & threadName,uint32_t fiberId,const std::string& fiberName,uint64_t time):	\
	m_file(file),	\
	m_line(line),	\
	m_runtime(runtime),	\
	m_threadId(threadId),	\
	m_threadName(threadName), \
    m_fiberId(fiberId),	\
    m_fiberName(fiberName), \
	m_time(time),	\
    m_logger(logger), \
    m_level(level)
    {}

//LogEvent封装类LogEventWarp实现
LogEventWarp::LogEventWarp(LogEvent::ptr e):m_event(e){}

LogEventWarp::~LogEventWarp(){
	m_event -> getLogger() -> log(m_event -> getLogLevel(),m_event);
};

std::stringstream& LogEventWarp::getSS(){
    return m_event -> getSS();
}

LogEvent::ptr LogEventWarp::getEvent() const{
    return m_event;
}

//Logger类函数实现
Logger::Logger(const std::string& str):m_name(str),m_level(LogLevel::DEBUG){
    m_formatter.reset(new LogFormatter(DEFAULT_TIME_PATTERN));
};

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
		auto self = shared_from_this();
		MutexType::Lock lock(m_mutex);
		if (!m_appenders.empty()) {
			for (auto& it : m_appenders) {
				it->log(self,level,event);
			}
		}
		else if (m_root) {
			m_root->log(level,event);
		}
	}
};

void Logger::fatal(LogEvent::ptr event) {
	log(LogLevel::FATAL,event);
}

void Logger::error(LogEvent::ptr event) {
	log(LogLevel::ERROR,event);
}

void Logger::info(LogEvent::ptr event) {
	log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
	log(LogLevel::WARN, event);
}

void Logger::debug(LogEvent::ptr event) {
	log(LogLevel::DEBUG, event);
}

void Logger::addLogAppender(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
	if (!appender->getFormatter()) {
		appender->setFormatter(m_formatter);
	}
	m_appenders.push_back(appender);
}

void Logger::delLogAppenders(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
	for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
		if (*it == appender) {
			m_appenders.erase(it);
			break;
		}
	}
}

void Logger::clearLogAppenders() {
    MutexType::Lock lock(m_mutex);
	m_appenders.clear();
}

void Logger::setLogFormatter(LogFormatter::ptr formatter) {
    MutexType::Lock lock(m_mutex);
	m_formatter = formatter;
	if (m_formatter) {
		for (auto& it : m_appenders) {
			if(!it->hasFormatter())
				it->setFormatter(m_formatter);
		}
	}
}

void Logger::setLogFormatter(const std::string& m_pattern) {
    LogFormatter::ptr formatter;
    if(!m_pattern.empty()){
        formatter = std::make_shared<LogFormatter>(m_pattern);
        setLogFormatter(formatter);
    }
};

std::string Logger::toYAMLString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["name"] = m_name;
    node["level"] = LogLevel::ToString(m_level);
    if(m_formatter){
        node["formatter"] = m_formatter -> getPattern(); 
    }
    for(auto & it : m_appenders){
       node["appenders"].push_back(YAML::Load(it -> toYAMLString())); 
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

//LogAppender类函数实现
void LogAppender::setFormatter(LogFormatter::ptr formatter) {
    MutexType::Lock lock(m_mutex);
	m_formatter = formatter;
	if (formatter) {
		m_has_fomatter = true;
	}
	else {
		m_has_fomatter = false;
	}
}

//FileLogAppender类函数实现
FileLogAppender::FileLogAppender(const std::string& filename):m_filename(filename){
	reopen();
}

bool FileLogAppender::reopen() {
    if(m_filestream){
        m_filestream.close();
    }	
    return FSUnit::OpenForWrite(m_filestream,m_filename);
}


void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        uint64_t now = event -> getTime();
        if(now >= 3 + m_lastTime){
            reopen();
            m_lastTime = now;
        }
		MutexType::Lock lock(m_mutex);
        if(!m_formatter -> format(m_filestream,logger,level,event)){
            std::cout << "error in write log!" << std::endl;           
        }
    }
};

std::string FileLogAppender::toYAMLString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = FILETYPESTR;
    node["level"] = LogLevel::ToString(m_level);
    if(hasFormatter()){
        node["formatter"] = m_formatter -> getPattern();
    }
    node["file"] = m_filename;
    std::stringstream ss;
    ss << node;
    return ss.str();
}

//StdoutLogAppender类实现
void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level){
    	MutexType::Lock lock(m_mutex);
        m_formatter -> format(std::cout, logger, level, event);
    }
}

std::string StdoutLogAppender::toYAMLString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = STDOUTTYPESTR;
    node["level"] = LogLevel::ToString(m_level);
    if(hasFormatter()){
        node["formatter"] = m_formatter -> getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}


//LogFormatter类实现
LogFormatter::LogFormatter(const std::string & pattern):m_pattern(pattern){
    init();
}

std::ostream& LogFormatter::format(std::ostream& ofs,std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event){
	for(auto &it : m_items){
        it -> format(ofs,logger,level,event);
    }
    return ofs;
}


std::string LogFormatter::format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    std::ostringstream oss;
    for(auto &it : m_items){
        it -> format(oss,logger,level,event);
    }
    return oss.str();
}

/**
 * @brief 日志等级打印器
 * */
class LevelFormatItem:public LogFormatter::FormatItem{
public:
    LevelFormatItem(const std::string & in = ""){}
    void format(std::ostream & ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << LogLevel::ToString(event -> getLogLevel());
    }
};

/**
 * @brief 时间打印器
 * */
class TimeFormatItem: public LogFormatter::FormatItem{
public:
    TimeFormatItem(const std::string & in = DEFAULT_TIME_PATTERN):time_pattern(in){
        if(time_pattern.empty())
            time_pattern = DEFAULT_TIME_PATTERN;
    }
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        time_t time = event -> getTime();
        struct tm tm;
        localtime_r(&time,&tm);
        char buf[64];
        strftime(buf,sizeof(buf),time_pattern.c_str(),&tm);
        ofs << buf;
    }
private:
    std::string time_pattern;
};

/**
 * @brief 运行时间打印器
 * */
class RuntimeFormatItem : public LogFormatter::FormatItem{
public:    
    RuntimeFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getRuntime();
    }
};

/**
 * @brief 日志名打印器
 * */
class LogNameFormatItem : public LogFormatter::FormatItem{
public:    
    LogNameFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFile();
    }
};

/**
 * @brief 线程ID打印器
 * */
class ThreadIdFormatItem : public LogFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getThreadId();
    }
};

/**
 * @brief 线程名打印器
 * */
class ThreadNameFormatItem : public LogFormatter::FormatItem{
public:
    ThreadNameFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getThreadName();
    }
};

/**
 * @brief 文件名打印器
 * */
class FileFormatItem : public LogFormatter::FormatItem{
public:    
    FileFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFile();
    }
};

/**
 * @brief 行号打印器
 * */
class LineFormatItem : public LogFormatter::FormatItem{
public:
    LineFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getLine();
    }
};

/**
 * @brief 协程ID打印器
 * */
class FiberIdFormatItem : public LogFormatter::FormatItem{
public:    
    FiberIdFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFiberId();
    }
};


/**
 * @brief 协程名打印器
 * */
class FiberNameFormatItem : public LogFormatter::FormatItem{
public:
    FiberNameFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFiberName();
    }
};

/**
 * @brief TAB打印器
 * */
class TabFormatItem : public LogFormatter::FormatItem{
public:
    TabFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << "\t";
    }
};

/**
 * @brief 换行打印器
 * */
class NewLineFormatItem : public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
		ofs << std::endl;
	}
};

/**
 * @brief 调用信息打印器
 * */
class MsgFormatItem : public LogFormatter::FormatItem{
public:
    MsgFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
        ofs << event -> getContent();
    }
};

/**
 * @brief 收入字符串打印器
 * */
class StringFormatItem : public LogFormatter::FormatItem{
public:
    StringFormatItem(const std::string & in):m_str(in){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
        ofs << m_str;
    }
private:
    std::string m_str;
};

void LogFormatter::init() {
    /**
     * @格式：%x{xxx}或者%x或者%%，其他非法
     * @tuple<str,fmt,int>
     * @details
     *  str:%后格式字段
     *  fmt:大括号内格式内容
     *  int:解析格式0,1;其中0为普通字符(即不解析，按照原格式输出)，1为Event中标定格式(小于0统一表示格式错误)
     */
    std::vector<std::tuple<std::string, std::string, int>> fmt_tuple_vec;
    //记录普通字符
    std::string nstr;
    //m_pattern大小
    int pattern_size = m_pattern.size();
    //记录访问位置，从0开始
    int r_index = 0;
    while (r_index <= pattern_size) {
        //判断非Event格式字符，保存至nstr
        if (PATTERN_SYMBLE != m_pattern[r_index]) {
            nstr.append(1, m_pattern[r_index]);
            ++r_index;
            continue;
        }
        ++r_index;
        if (r_index == pattern_size)
            break;
        if ('%' == m_pattern[r_index]) {
            nstr.append(1, m_pattern[r_index]);
            ++r_index;
            continue;
        }

        //记录格式字段位置
        int r_fmt_index = r_index;
        int fmt_status = 0;
        int fmt_begin;
        std::string str;
        std::string fmt;
        while (r_fmt_index <= pattern_size) {
            if (0 == fmt_status) {
                if ('{' == m_pattern[r_fmt_index]) {
                    str = m_pattern.substr(r_index, r_fmt_index - r_index);
                    //状态切换
                    fmt_status = 1;
                    fmt_begin = r_fmt_index +  1;
                }
                else if (!isalpha(m_pattern[r_fmt_index])) {
                    str = m_pattern.substr(r_index, r_fmt_index - r_index);
                    break;
                }
            }
            if (1 == fmt_status && '}' == m_pattern[r_fmt_index]) {
                //字段格式读取完毕，状态切换
                fmt_status = 0;
                fmt = m_pattern.substr(fmt_begin, r_fmt_index - fmt_begin);
                ++r_fmt_index;               break;
            }
            ++r_fmt_index;
        }

        //有Event定义格式后生成新的tuple存放
        if (!nstr.empty()) {
            fmt_tuple_vec.push_back(make_tuple(nstr, std::string(""),0));
            nstr.clear();
        }
        if (!fmt_status) {
            fmt_tuple_vec.push_back(make_tuple(str, fmt, 1));
        }
        else {
            m_error = true;
            //加入tuple中，当前不使用
            fmt_tuple_vec.push_back(make_tuple(ERROR_FORMAT_PATTERN, std::string(""), -1));
        }
        r_index = r_fmt_index;
    }
    //宏定义生成通用的FormatItem
    static std::map<std::string, std::function<FormatItem::ptr(const std::string&)> > m_static_items = {
    #define FWL_XXX(str,func) \
        {#str,[](const std::string & fmt){return FormatItem::ptr(new func(fmt));}}
        FWL_XXX(p,LevelFormatItem), //日志等级
        FWL_XXX(m,MsgFormatItem), //日志信息
        FWL_XXX(d,TimeFormatItem), //当前等级时间
        FWL_XXX(r,RuntimeFormatItem), //累计运行时间
        FWL_XXX(c,LogNameFormatItem), //日志名称
        FWL_XXX(t,ThreadIdFormatItem), //线程ID，很少使用
        FWL_XXX(N,ThreadNameFormatItem), //线程名称
        FWL_XXX(f,FileFormatItem), //文件
        FWL_XXX(l,LineFormatItem), //行号
        FWL_XXX(F,FiberIdFormatItem), //协程ID
        //FWL_XXX(N,FiberNameFormatItem), //协程名
        FWL_XXX(T,TabFormatItem), //制表
        FWL_XXX(n,NewLineFormatItem) //换行
    };
#undef FWL_XXX
    for (auto& it : fmt_tuple_vec) {
        if (0 == std::get<2>(it)) {
            //将文本内容存入
            m_items.push_back(StringFormatItem::ptr(new StringFormatItem(std::get<0>(it))));
            }
        else {
            auto it_static_item = m_static_items.find(std::get<0>(it));
            if (m_static_items.end() == it_static_item) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(ERROR_FORMAT_PATTERN)));
                m_error = true;
            }
            else{
                m_items.push_back(it_static_item -> second(std::get<1>(it)));
            }
        
        }
    }
}

//LoggerManager类
LoggerManager::LoggerManager(){
    m_root.reset(new Logger);
    m_root->addLogAppender(LogAppender::ptr(new StdoutLogAppender));
    //m_root->addLogAppender(LogAppender::ptr(new FileLogAppender("../log_file/root.txt")));
    m_loggers[m_root -> m_name] = m_root; 
}

Logger::ptr LoggerManager::getLogger(const std::string & name){
    MutexType::Lock lock(m_mutex);
    auto it = m_loggers.find(name);
    if(it != m_loggers.end()){
        return it -> second;
    }
    Logger::ptr logger(new Logger(name));
    m_loggers[name] = logger;
    logger->m_root = m_root;
    return logger;
}

std::string LoggerManager::toYAMLString(){
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    for(auto it : m_loggers){
        node.push_back(YAML::Load(it.second->toYAMLString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

//LexicalCast模板类-全特化string to LogDefine
template<>
class LexicalCast<std::string, LogDefine>{
public:
    LogDefine operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        LogDefine ld;
        //解析name，关键词，解析错误返回异常
        if(!node["name"].IsDefined()){
            std::cout << "config name is error!" << std::endl;
            throw std::logic_error("config name is error!");
        }
        ld.m_name = node["name"].as<std::string>();
        if(node["level"].IsDefined()){
            std::string level_str = node["level"].as<std::string>();
            ld.m_level = LogLevel::FromString(level_str);
        }
        if(node["formatter"].IsDefined()){
            ld.m_formatter = node["formatter"].as<std::string>();
        }
        //遍历appenders
        if(node["appenders"].IsDefined()){
            for(size_t i =  0; i < node["appenders"].size(); ++i){
                auto adNode = node["appenders"][i];
                /**
                 * @brief 解析type
                 * */
                if(!adNode["type"].IsDefined()){
                    std::cout << "config error,appender type is error!" << std::endl;
                    continue;
                }
                std::string str_type = adNode["type"].as<std::string>();
                TRANSFORM_TO_LOWER(str_type);
                LogAppenderDefine lad;
                //判断appender类型
                if(std::string(FILETYPESTR) == str_type){
                    //如果是文件，则需判断file是否存在以及是否非空
                    std::string in_file;
                    if(!adNode["file"].IsDefined() || (in_file = adNode["file"].as<std::string>()).empty()){
                        std::cout << "config error,appender file is error!" << std::endl;
                        continue;
                    }
                    lad.file = in_file;
                    lad.type = FILE;
                }else if(std::string(STDOUTTYPESTR) == str_type){
                    lad.type = STDOUT;
                }else{
                    std::cout << "config error,appender type is not find!" << std::endl;
                    continue;
                }
                /**
                 * @brief 解析其他元素,level和formatter
                 * */
                if(adNode["formatter"].IsDefined()){
                    lad.formatter = adNode["formatter"].as<std::string>();
                }
                if(adNode["level"].IsDefined()){
                    lad.level = LogLevel::FromString(adNode["level"].as<std::string>());
                }
                ld.m_appenders.push_back(lad);
            }
        }
        return ld;
    }
};

//LexicalCast模板类-全特化LogDefine to string
template<>
class LexicalCast<LogDefine,std::string>{
public:
    std::string operator()(const LogDefine & ld){
        YAML::Node node;
        //判断ld中name是否为空，为空视为无效
        if(!ld.m_name.empty()){
            node["name"] = ld.m_name;
            //如果m_level为UNKOWN，则不更新
            if(ld.m_level !=  LogLevel::Level::UNKNOWN){
                node["level"] = LogLevel::ToString(ld.m_level);
            }
            if(!ld.m_formatter.empty()){
                node["formatter"] = ld.m_formatter;
            }
            //存入appenders
            for(auto a : ld.m_appenders){
                YAML::Node n;
                //写入type
                if(a.type == FILE){
                    if(a.file.empty()){
                        std::cout << "logdefine appnder file empty"<< std::endl;
                        continue;
                    }
                    n["type"] = FILETYPESTR;
                    n["file"] = a.file;
                }else if(a.type == STDOUT){
                    n["type"] = STDOUTTYPESTR;
                }else{
                    std::cout << "logdefine appender type error" << std::endl;
                    continue;
                }
                //写入formatter
                if(!a.formatter.empty()){
                    n["formatter"] = a.formatter;
                }
                //写入level
                n["level"] = LogLevel::ToString(a.level);
                node["appenders"].push_back(n);
            }
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//日志文件
ConfigVar<std::set<LogDefine> >::ptr g_log_define = Config::lookUp<std::set<LogDefine>>("logs",std::set<LogDefine>(), "logs config");

struct LogInit{
    LogInit(){
        g_log_define -> addListenerCb(0xFBFB,[](std::set<LogDefine>& old_value,
                                               const std::set<LogDefine> & new_value){
            FWL_LOG_INFO(FWL_LOG_ROOT()) << "log config on change!";
            for(auto & i : new_value){
                auto it = old_value.find(i); 
                //增加
                Logger::ptr logger;
                if(it == old_value.end()){
                    logger = FWL_LOG_NAME(i.m_name);
                }else{
                    if(!(i == *it)){
                        //修改
                        logger = FWL_LOG_NAME(i.m_name);
                    }else{
                        continue;
                    }
                }
                logger->setLevel(i.m_level);
                if(!i.m_formatter.empty()){
                    logger->setLogFormatter(i.m_formatter);
                }
                logger -> clearLogAppenders();
                for(auto a : i.m_appenders){
                    LogAppender::ptr ad;
                    if(FILE ==  a.type){
                        if(a.file.empty()){
                            std::cout << "log.name:" << i.m_name << " has error in appender filename" << std::endl;
                            continue;
                        }
                        ad.reset(new FileLogAppender(a.file));
                    }else if(STDOUT == a.type){
                        ad.reset(new StdoutLogAppender());
                    }else{
                        continue;
                    }
                    ad -> setLevel(a.level); 
                    if(!a.formatter.empty()){
                        LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                        if(!fmt -> isError()){
                            ad -> setFormatter(fmt);
                        }else{
                            std::cout << "log.name:" << i.m_name << " appender.type"
                                << a.type << " formatter is error" << std::endl;
                            continue;
                        }
                    }
                    logger -> addLogAppender(ad);
                }
            }
            //删除old_value多余的Logger
            for(auto & i : old_value){
                auto it = new_value.find(i);
                if(it == new_value.end()){
                    //暂不删除，避免调用函数变量异常，将level设置为CLOSE表示不在使用
                    Logger::ptr logger = FWL_LOG_NAME(i.m_name);
                    logger -> setLevel(LogLevel::Level::CLOSED);
                    logger -> clearLogAppenders();
                }
            }
        });
    }
};

static LogInit log_init;

}
