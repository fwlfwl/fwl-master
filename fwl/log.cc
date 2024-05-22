#include"base_head.h"
#include"log.h"
#include"config.h"

namespace fwl{

//LogLevel��ʵ��
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

//LogEvent��ʵ��
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

//LogEvent��װ��LogEventWarpʵ��
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

//Logger�ຯ��ʵ��
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

//LogAppender�ຯ��ʵ��
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

//FileLogAppender�ຯ��ʵ��
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

//StdoutLogAppender��ʵ��
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


//LogFormatter��ʵ��
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
 * @brief ��־�ȼ���ӡ��
 * */
class LevelFormatItem:public LogFormatter::FormatItem{
public:
    LevelFormatItem(const std::string & in = ""){}
    void format(std::ostream & ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << LogLevel::ToString(event -> getLogLevel());
    }
};

/**
 * @brief ʱ���ӡ��
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
 * @brief ����ʱ���ӡ��
 * */
class RuntimeFormatItem : public LogFormatter::FormatItem{
public:    
    RuntimeFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getRuntime();
    }
};

/**
 * @brief ��־����ӡ��
 * */
class LogNameFormatItem : public LogFormatter::FormatItem{
public:    
    LogNameFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFile();
    }
};

/**
 * @brief �߳�ID��ӡ��
 * */
class ThreadIdFormatItem : public LogFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getThreadId();
    }
};

/**
 * @brief �߳�����ӡ��
 * */
class ThreadNameFormatItem : public LogFormatter::FormatItem{
public:
    ThreadNameFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getThreadName();
    }
};

/**
 * @brief �ļ�����ӡ��
 * */
class FileFormatItem : public LogFormatter::FormatItem{
public:    
    FileFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFile();
    }
};

/**
 * @brief �кŴ�ӡ��
 * */
class LineFormatItem : public LogFormatter::FormatItem{
public:
    LineFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getLine();
    }
};

/**
 * @brief Э��ID��ӡ��
 * */
class FiberIdFormatItem : public LogFormatter::FormatItem{
public:    
    FiberIdFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFiberId();
    }
};


/**
 * @brief Э������ӡ��
 * */
class FiberNameFormatItem : public LogFormatter::FormatItem{
public:
    FiberNameFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << event -> getFiberName();
    }
};

/**
 * @brief TAB��ӡ��
 * */
class TabFormatItem : public LogFormatter::FormatItem{
public:
    TabFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
        ofs << "\t";
    }
};

/**
 * @brief ���д�ӡ��
 * */
class NewLineFormatItem : public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string &in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override{
		ofs << std::endl;
	}
};

/**
 * @brief ������Ϣ��ӡ��
 * */
class MsgFormatItem : public LogFormatter::FormatItem{
public:
    MsgFormatItem(const std::string & in = ""){}
    void format(std::ostream &ofs,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
        ofs << event -> getContent();
    }
};

/**
 * @brief �����ַ�����ӡ��
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
     * @��ʽ��%x{xxx}����%x����%%�������Ƿ�
     * @tuple<str,fmt,int>
     * @details
     *  str:%���ʽ�ֶ�
     *  fmt:�������ڸ�ʽ����
     *  int:������ʽ0,1;����0Ϊ��ͨ�ַ�(��������������ԭ��ʽ���)��1ΪEvent�б궨��ʽ(С��0ͳһ��ʾ��ʽ����)
     */
    std::vector<std::tuple<std::string, std::string, int>> fmt_tuple_vec;
    //��¼��ͨ�ַ�
    std::string nstr;
    //m_pattern��С
    int pattern_size = m_pattern.size();
    //��¼����λ�ã���0��ʼ
    int r_index = 0;
    while (r_index <= pattern_size) {
        //�жϷ�Event��ʽ�ַ���������nstr
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

        //��¼��ʽ�ֶ�λ��
        int r_fmt_index = r_index;
        int fmt_status = 0;
        int fmt_begin;
        std::string str;
        std::string fmt;
        while (r_fmt_index <= pattern_size) {
            if (0 == fmt_status) {
                if ('{' == m_pattern[r_fmt_index]) {
                    str = m_pattern.substr(r_index, r_fmt_index - r_index);
                    //״̬�л�
                    fmt_status = 1;
                    fmt_begin = r_fmt_index +  1;
                }
                else if (!isalpha(m_pattern[r_fmt_index])) {
                    str = m_pattern.substr(r_index, r_fmt_index - r_index);
                    break;
                }
            }
            if (1 == fmt_status && '}' == m_pattern[r_fmt_index]) {
                //�ֶθ�ʽ��ȡ��ϣ�״̬�л�
                fmt_status = 0;
                fmt = m_pattern.substr(fmt_begin, r_fmt_index - fmt_begin);
                ++r_fmt_index;               break;
            }
            ++r_fmt_index;
        }

        //��Event�����ʽ�������µ�tuple���
        if (!nstr.empty()) {
            fmt_tuple_vec.push_back(make_tuple(nstr, std::string(""),0));
            nstr.clear();
        }
        if (!fmt_status) {
            fmt_tuple_vec.push_back(make_tuple(str, fmt, 1));
        }
        else {
            m_error = true;
            //����tuple�У���ǰ��ʹ��
            fmt_tuple_vec.push_back(make_tuple(ERROR_FORMAT_PATTERN, std::string(""), -1));
        }
        r_index = r_fmt_index;
    }
    //�궨������ͨ�õ�FormatItem
    static std::map<std::string, std::function<FormatItem::ptr(const std::string&)> > m_static_items = {
    #define FWL_XXX(str,func) \
        {#str,[](const std::string & fmt){return FormatItem::ptr(new func(fmt));}}
        FWL_XXX(p,LevelFormatItem), //��־�ȼ�
        FWL_XXX(m,MsgFormatItem), //��־��Ϣ
        FWL_XXX(d,TimeFormatItem), //��ǰ�ȼ�ʱ��
        FWL_XXX(r,RuntimeFormatItem), //�ۼ�����ʱ��
        FWL_XXX(c,LogNameFormatItem), //��־����
        FWL_XXX(t,ThreadIdFormatItem), //�߳�ID������ʹ��
        FWL_XXX(N,ThreadNameFormatItem), //�߳�����
        FWL_XXX(f,FileFormatItem), //�ļ�
        FWL_XXX(l,LineFormatItem), //�к�
        FWL_XXX(F,FiberIdFormatItem), //Э��ID
        //FWL_XXX(N,FiberNameFormatItem), //Э����
        FWL_XXX(T,TabFormatItem), //�Ʊ�
        FWL_XXX(n,NewLineFormatItem) //����
    };
#undef FWL_XXX
    for (auto& it : fmt_tuple_vec) {
        if (0 == std::get<2>(it)) {
            //���ı����ݴ���
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

//LoggerManager��
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

//LexicalCastģ����-ȫ�ػ�string to LogDefine
template<>
class LexicalCast<std::string, LogDefine>{
public:
    LogDefine operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        LogDefine ld;
        //����name���ؼ��ʣ��������󷵻��쳣
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
        //����appenders
        if(node["appenders"].IsDefined()){
            for(size_t i =  0; i < node["appenders"].size(); ++i){
                auto adNode = node["appenders"][i];
                /**
                 * @brief ����type
                 * */
                if(!adNode["type"].IsDefined()){
                    std::cout << "config error,appender type is error!" << std::endl;
                    continue;
                }
                std::string str_type = adNode["type"].as<std::string>();
                TRANSFORM_TO_LOWER(str_type);
                LogAppenderDefine lad;
                //�ж�appender����
                if(std::string(FILETYPESTR) == str_type){
                    //������ļ��������ж�file�Ƿ�����Լ��Ƿ�ǿ�
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
                 * @brief ��������Ԫ��,level��formatter
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

//LexicalCastģ����-ȫ�ػ�LogDefine to string
template<>
class LexicalCast<LogDefine,std::string>{
public:
    std::string operator()(const LogDefine & ld){
        YAML::Node node;
        //�ж�ld��name�Ƿ�Ϊ�գ�Ϊ����Ϊ��Ч
        if(!ld.m_name.empty()){
            node["name"] = ld.m_name;
            //���m_levelΪUNKOWN���򲻸���
            if(ld.m_level !=  LogLevel::Level::UNKNOWN){
                node["level"] = LogLevel::ToString(ld.m_level);
            }
            if(!ld.m_formatter.empty()){
                node["formatter"] = ld.m_formatter;
            }
            //����appenders
            for(auto a : ld.m_appenders){
                YAML::Node n;
                //д��type
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
                //д��formatter
                if(!a.formatter.empty()){
                    n["formatter"] = a.formatter;
                }
                //д��level
                n["level"] = LogLevel::ToString(a.level);
                node["appenders"].push_back(n);
            }
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//��־�ļ�
ConfigVar<std::set<LogDefine> >::ptr g_log_define = Config::lookUp<std::set<LogDefine>>("logs",std::set<LogDefine>(), "logs config");

struct LogInit{
    LogInit(){
        g_log_define -> addListenerCb(0xFBFB,[](std::set<LogDefine>& old_value,
                                               const std::set<LogDefine> & new_value){
            FWL_LOG_INFO(FWL_LOG_ROOT()) << "log config on change!";
            for(auto & i : new_value){
                auto it = old_value.find(i); 
                //����
                Logger::ptr logger;
                if(it == old_value.end()){
                    logger = FWL_LOG_NAME(i.m_name);
                }else{
                    if(!(i == *it)){
                        //�޸�
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
            //ɾ��old_value�����Logger
            for(auto & i : old_value){
                auto it = new_value.find(i);
                if(it == new_value.end()){
                    //�ݲ�ɾ����������ú��������쳣����level����ΪCLOSE��ʾ����ʹ��
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
