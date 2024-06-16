#ifndef __H_FWL_CONFIG__
#define __H_FWL_CONFIG__

class Person;

/**
* @file config.h
* @brief 配置文件类封装
* @author fwl
* @email 1822363660@qq.com
* @data 2023-10-21
**/

#include"base_head.h"
#include"log.h"
#include"mutex.h"
#include"singletion.h"
#include<yaml-cpp/yaml.h>
#include<boost/lexical_cast.hpp>

#define TRANSFORM_TO_LOWER(str) \
    std::transform(str.begin(),str.end(),str.begin(),::tolower)
#define CHECK_STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPWRSTUVWXYZ._1234567890"

namespace fwl{

/**
 * @brief ConfigVarBase类
 * @details
 *      ConfigVar基类
 * */
class ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    
    /**
     * @brief 构造函数
     * */
    ConfigVarBase(const std::string &name,const  std::string & describe):m_name(name),m_describe(describe){
        TRANSFORM_TO_LOWER(m_name);
    }
    
    /**
     * @brief 析构函数
     * */
    virtual ~ConfigVarBase() {};
    
    /**
     * @brief 返回配置项名
     * */
    std::string& getName(){  return m_name;  }
    
    /**
     * @brief 返回配置描述
     * */
    std::string& getDescriber(){ return m_describe;  }

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string & val) = 0;
    virtual std::string getTypeName() =  0;

protected:
    //配置项名
    std::string m_name;
    //配置项描述
    std::string m_describe;
};

/**
 * @brief LexicalCast模板类
 * */
template<class F,class T>
class LexicalCast{
public:
    T operator()(const F & val){
        return boost::lexical_cast<T>(val);
    }
};

/**
 * @brief LexicalCast模板类-偏特化string to vector
 * */
template<class T>
class LexicalCast<std::string,std::vector<T> >{
public:
    std::vector<T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::vector<T> out;
        std::stringstream ss;
        for(size_t i =  0; i != node.size(); ++i){
            ss.str("");
            ss << node[i];
            out.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return out;
    }
};

/**
 * @brief LexicalCast模板类-偏特化vector to string
 * */
template<class T>
class LexicalCast<std::vector<T>,std::string>{
public:    
    std::string operator()(const std::vector<T> & val){
        YAML::Node node(YAML::NodeType::Sequence);
        for(size_t i =  0; i != val.size(); ++i){
            node.push_back(LexicalCast<T,std::string>()(val[i]));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief LexicalCast模板类-偏特化list to string
 * */
template<class T>
class LexicalCast<std::list<T>,std::string>{
public:    
    std::string operator()(const std::list<T> & val){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto & it : val){
            node.push_back(LexicalCast<T,std::string>()(it));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


/**
 * @brief LexicalCast模板类-偏特化string to list
 * */
template<class T>
class LexicalCast<std::string,std::list<T> >{
public:    
    std::list<T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::stringstream ss;
        std::list<T> out;
        for(size_t i =  0; i != node.size(); ++i){
            ss.str("");
            ss << node[i];
            out.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return out;
    }
};


/**
 * @brief LexicalCast模板类-偏特化string to map
 * */
template<class T>
class LexicalCast<std::string,std::map<std::string,T> >{
public:    
    std::map<std::string,T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::stringstream ss;
        std::map<std::string,T> out;
        for(auto it =  node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it -> second;
            out.insert(std::make_pair(it -> first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
        }
        return out;
    }
};


/**
 * @brief LexicalCast模板类-偏特化map to string
 * */
template<class T>
class LexicalCast<std::map<std::string,T>,std::string>{
public:    
    std::string operator()(const std::map<std::string,T> & val){
        YAML::Node node(YAML::NodeType::Map);
        for(auto & it : val){
            node[it.first] = LexicalCast<T,std::string>()(it.second);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


/**
 * @brief LexicalCast模板类-偏特化string to unordered_map
 * */
template<class T>
class LexicalCast<std::string,std::unordered_map<std::string,T> >{
public:    
    std::unordered_map<std::string,T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::stringstream ss;
        std::unordered_map<std::string,T> out;
        for(auto it =  node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it -> second;
            out.insert(std::make_pair(it -> first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
        }
        return out;
    }
};

/**
 * @brief LexicalCast模板类-偏特化unordered_map to string
 * */
template<class T>
class LexicalCast<std::unordered_map<std::string,T>,std::string>{
public:    
    std::string operator()(const std::unordered_map<std::string,T> & val){
        YAML::Node node(YAML::NodeType::Map);
        for(auto & it : val){
            node[it.first] = LexicalCast<T,std::string>()(it.second);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief LexicalCast模板类-偏特化string to set
 * */
template<class T>
class LexicalCast<std::string,std::set<T> >{
public:    
    std::set<T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::stringstream ss;
        std::set<T> out;
        for(size_t i =  0; i != node.size(); ++i){
            ss.str("");
            ss << node[i];
            out.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return out;
    }
};

/**
 * @brief LexicalCast模板类-偏特化set to string
 * */
template<class T>
class LexicalCast<std::set<T>,std::string>{
public:    
    std::string operator()(const std::set<T> & val){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto & it : val){
            node.push_back(LexicalCast<T,std::string>()(it));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief LexicalCast模板类-偏特化string to unordered_set
 * */
template<class T>
class LexicalCast<std::string,std::unordered_set<T> >{
public:    
    std::unordered_set<T> operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        std::stringstream ss;
        std::unordered_set<T> out;
        for(size_t i =  0; i != node.size(); ++i){
            ss.str("");
            ss << node[i];
            out.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return out;
    }
};

/**
 * @brief LexicalCast模板类-偏特化unordered_set to string
 * */
template<class T>
class LexicalCast<std::unordered_set<T>,std::string>{
public:    
    std::string operator()(const std::unordered_set<T> & val){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto & it : val){
            node.push_back(it);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief ConfigVar类
 * @details
 *      继承ConfigVarBase
 * */
template<class T, class FromStr = LexicalCast<std::string,T>, class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (T& old_value,const T& new_value)> on_change; //定义配置变更类
    //使用读写锁
    typedef RWMutex RWMutexType;  

    /**
     * @brief 构造函数
     * */
    ConfigVar(const std::string & name,const T & value, const std::string & describe):   \
        ConfigVarBase(name,describe),m_val(value){
    }

    /**
     * @brief 配置值转化为string
     * */
    std::string toString() override {
        RWMutexType::ReadLock lock(m_mutex);
        try{
            return ToStr()(m_val);
            //return boost::lexical_cast<std::string>(m_val);
        }catch(std::exception & e){
            FWL_LOG_ERROR(FWL_LOG_ROOT()) << "ConfigVar::toString exception " <<    \
                e.what() << " convert:" << typeid(m_val).name() << " to string";
        }
        return "";
    }
    
    /**
     * @brief string转化为配置值
     * */
    bool fromString(const std::string & val) override{
        try{
            //模板处理
            setValue(FromStr()(val));
            //m_val = boost::lexical_cast<T>(val);
            return true;
        }catch(std::exception & e){
            FWL_LOG_ERROR(FWL_LOG_ROOT()) << "ConfigVar::fromString exception "<<   \
                e.what() << " convert:string to " << typeid(m_val).name();
        }
        return false;
    }

    /**
     * @brief 返回值类型
     * */
    std::string getTypeName() override{
        RWMutexType::ReadLock lock(m_mutex);
        return typeid(T).name();
    }
    
    /**
     * @brief 返回配置值
     * */
    const T getValue(){
        RWMutexType::ReadLock lock(m_mutex);
        return m_val;
    }
    
    /**
     * @brief 设置配置值
     * */
    void setValue(const T & val){
        {
            RWMutexType::ReadLock lock(m_mutex);
            if(val == m_val){
                return;
            }
            for(auto & it : m_cbs){
                it.second(m_val,val);
            }
        }
        RWMutexType::WriteLock lock(m_mutex);
        m_val = val;
    }

    /**
     * @brief 新增配置监听事件
     * */
    bool addListenerCb(const uint64_t key,const on_change cb){
        if(cb){
            RWMutexType::WriteLock lock(m_mutex);
            m_cbs[key] = cb;
            return true;
        }
        return false;
    }
    
    /**
     * @brief 删除配置舰艇事件
     * */
    bool delListenerCb(const uint64_t key){
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.earse(key);
        return true;
    }

    /**
     * @brief 返回配置监听事件
     * */
    on_change getListenerCb(const uint64_t key) const{
        RWMutexType::ReadLock lock(m_mutex);
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it -> second;
    }

private:
    //配置值
    T m_val;
    //配置变更回调函数
    std::map<uint64_t,on_change> m_cbs;
    //读写锁
    RWMutexType m_mutex;
};


/**
 * @brief Config类
 * */
class Config{
public:
    typedef std::map<std::string,ConfigVarBase::ptr> ConfigVarMap;
	typedef std::map<std::string, uint64_t> ConfigFileMap;	//配置文件(<文件名,最后更新时间)
    typedef RWMutex RWMutexType;  
    /**
     * @brief 查找ConfigVar
     * @param[in] name 配置名
     * @param[in] describe 配置描述
     * @param[in] val 配置值
     * */
    template<typename T>
        static typename ConfigVar<T>::ptr lookUp(const std::string & name, const T & val, const std::string & describe){
            //{}为了不需要手动释放锁，自动释放锁
            {
                RWMutexType::ReadLock lock(getMutex());
                auto it = getData().find(name);
                if(it != getData().end()){
                typename ConfigVar<T>::ptr ptr = std::dynamic_pointer_cast<ConfigVar<T> >(it -> second);
                    if(ptr){
                        FWL_LOG_INFO(FWL_LOG_ROOT()) << "Config::lookUp name=" << name;
                        return ptr;      
                    }
                    else{
                        FWL_LOG_INFO(FWL_LOG_ROOT()) << "Config:lookUp name=" << name
                            << " exist,the type is "<< it->second->getTypeName();
                        return nullptr;
                    }
                }
                if(name.find_first_not_of(CHECK_STRING) != std::string::npos){
                    FWL_LOG_ERROR(FWL_LOG_ROOT()) << "Config::lookUp name illegal";
                    return nullptr;
                }
            }
            RWMutexType::WriteLock lock(getMutex());
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,val,describe));
            getData()[name] = v;
            return v;
        }
     
    /**
     * @brief 查找ConfigVar
     * @param[in] name 配置名
     * */
    template<typename T>
        static typename ConfigVar<T>::ptr lookUp(const std::string & name){
            RWMutexType::ReadLock lock(getMutex());
            auto it = getData().find(name);
            if(it == getData().end()){
                return nullptr;
            }
            return std::dynamic_pointer_cast<ConfigVar<T> >(it -> second);
        }
    
    /**
     * @brief 查找基类ConfigBaseVar
     * */
    static typename ConfigVarBase::ptr lookUpBase(const std::string &name);

    /**
     * @brief 将yaml获取的node加入list
     * */
    static void addNodeList(const std::string &,const YAML::Node & node,std::map<std::string,YAML::Node> &);

    /**
     * @brief 加载配置文件
     * */
    static void loadYamlConfig(const YAML::Node & node);

	//从目录中读取所有配置文件
	static void LoadFromDir(const std::string & path);
private:
	//加载所有节点信息
	static void ListAllMembers(const std::vector<std::string> & path);
    
	//避免调用静态变量函数先于静态变量前初始化出现异常
    static ConfigVarMap& getData(){
        static ConfigVarMap m_datas;
        return m_datas;
    }
    
	//所有配置文件 
	static ConfigFileMap & getFiles(){
		static ConfigFileMap m_files;
		return m_files;
	}

	static RWMutexType& getMutex(){
        static RWMutexType m_mutex;
        return m_mutex;
    }
};

}

#endif

