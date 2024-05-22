#include"../fwl/unit.h"
#include"../fwl/log.h"
#include"../fwl/config.h"

#if 0

fwl::ConfigVar<int>::ptr g_int_value_config = fwl::Config::lookUp("system.port",8080, "system port");

fwl::ConfigVar<float>::ptr g_float_value_config = fwl::Config::lookUp("system.port", (float)8080, "system port_float");

fwl::ConfigVar<std::vector<int> >::ptr g_vector_value_config = fwl::Config::lookUp("system.vec_int", std::vector<int>({11,22}), "system vec_int");

fwl::ConfigVar<std::list<int> >::ptr g_list_value_config = fwl::Config::lookUp("system.list_int",std::list<int>({23,34,56}), "system list_int");

fwl::ConfigVar<std::set<int> >::ptr g_set_value_config = fwl::Config::lookUp("system.set_int", std::set<int>({100,200,700,500}), "system list");

fwl::ConfigVar<std::map<std::string,std::vector<int>>>::ptr g_map_value_config = fwl::Config::lookUp("system.map_vec", std::map<std::string,std::vector<int>>({ {"key1",{1,23}}, {"key2",{90,80}} }), "system map");

fwl::ConfigVar<std::unordered_set<int> >::ptr g_unorderedset_value_config = fwl::Config::lookUp("system.unorderedSet_int", std::unordered_set<int>{33,232}, "system unorderedSet");

fwl::ConfigVar<std::unordered_map<std::string,float> >::ptr g_unorderedmap_value_config = fwl::Config::lookUp("system.unorderedMap_float", std::unordered_map<std::string,float>({{"key_1",128.8}, {"key_2",2.7}}), "system unorderMap");

#define XX(g_val,name,prefix) \
    {   \
        if(g_val){ \
            auto v = g_val -> getValue();   \
            for(auto it = v.begin();it != v.end(); ++it){ \
                FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name ":" << *it; \
            }   \
            FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name " yaml:" << g_val -> toString();    \
        }       \
    }

#define XXM(g_val,name,prefix)  \
    {   \
        if(g_val){  \
            auto v = g_val -> getValue();   \
            for(auto & it : v){  \
                FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name ":{"<< it.first << "-" << it.second << "}";   \
            }   \
            FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name " yaml:" << g_val -> toString(); \
        }   \
    }

void testConfig(){
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before int_value_config:" <<  (nullptr == g_int_value_config ? "nullptr" : g_int_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before int_value_config yaml:" <<  (nullptr == g_int_value_config ? 0 : g_int_value_config -> getValue());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before float_value_config:" << (nullptr == g_float_value_config ? "nullptr" : g_float_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before float_value_config yaml:" <<  (nullptr == g_float_value_config ? 0.0 : g_float_value_config -> getValue());

    XX(g_list_value_config, list_value_config, before)
    XX(g_vector_value_config, vector_value_config, before)
    XX(g_set_value_config ,set_value_config, before)
    XX(g_unorderedset_value_config,unordered_value_config, before)
    //XXM(g_map_value_config, map_value_config,before)
    XXM(g_unorderedmap_value_config, unorderedmap_value_config,before)

    
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "Load config file";
    YAML::Node root = YAML::LoadFile("../test.yml");

    fwl::Config::loadYamlConfig(root);

    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after int_value_config:" <<  (nullptr == g_int_value_config ? "nullptr" : g_int_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after int_value_config yaml:" <<  (nullptr == g_int_value_config ? 0 : g_int_value_config -> getValue());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after float_value_config:" << (nullptr == g_float_value_config ? "nullptr" : g_float_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after float_value_config yaml:" << (nullptr == g_float_value_config ? 0.0 : g_float_value_config -> getValue());
    
    XX(g_list_value_config,list_value_config,after)
    XX(g_vector_value_config,vector_value_config,after)
    XX(g_set_value_config,set_value_config,after)
    XX(g_unorderedset_value_config,unordered_value_config,after)
    //XXM(g_map_value_config,map_value_config,after)
    XXM(g_unorderedmap_value_config,unorderedmap_value_config,after)
}

#endif


/**
 * @brief 测试自定义类
 * */
class Person{
public:
    Person(){}
    
    const std::string printString() const{
        std::stringstream ss;
        ss << "person.name-" << name + " person.age-" << age
            << " person.sex-" << (sex ? "man" : "woman");
        return ss.str();
    }
    
    Person& operator=(const Person & in_person){
        name = in_person.name;
        age = in_person.age;
        sex = in_person.sex;
        return *this;
    }
    
    bool operator==(const Person & other_person) const{
        return name == other_person.name 
            && age == other_person.age
            && sex == other_person.sex;
    }

    std::string name = "";
    uint32_t age = 0;
    bool sex = true;
};

namespace fwl{
/**
 * @brief LexicalCast模板类-全特化(自定义测试类test_config.cpp::Person)string to Person
 * */
template<>
class LexicalCast<std::string,Person>{
public:    
    Person operator()(const std::string & val){
        YAML::Node node = YAML::Load(val);
        Person p;
        p.name = node["name"].as<std::string>();
        p.age = node["age"].as<uint32_t>();
        p.sex = node["sex"].as<bool>();
        return p;
    }
};

/**
 * @brief LexicalCast模板类-全特化(自定义测试类test_config.cpp::Person)Person to string
 * */
template<>
class LexicalCast<Person,std::string>{
public:    
    std::string operator()(const Person & p){
        YAML::Node node(YAML::NodeType::Map);
        node["name"] = p.name;
        node["age"] = p.age;
        node["sex"] = p.sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}

fwl::ConfigVar<Person> ::ptr g_person_value_config = fwl::Config::lookUp("class.person", Person(), "class person");
fwl::ConfigVar<std::map<std::string,std::vector<Person>>>::ptr g_mapVecPer_value_config = fwl:: Config::lookUp("class.person_map", std::map<std::string,std::vector<Person>>(), "class person map");

#define XXMVP(g_val,name,prefix)  \
    {   \
        if(g_val){  \
            auto v = g_val -> getValue();   \
            for(auto & it : v){  \
                FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name << it.first;   \
                for(auto & i : it.second){    \
                    FWL_LOG_INFO(FWL_LOG_ROOT())<< #prefix" "<< i.printString();    \
                }   \
            }   \
            FWL_LOG_INFO(FWL_LOG_ROOT()) << #prefix " " #name " yaml:" << g_val -> toString(); \
        }   \
    }

void testPersonConfig(){
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before g_person_value_config:" <<  (nullptr == g_person_value_config ? "nullptr" : g_person_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "before g_person_value_config yaml:" << g_person_value_config -> getValue().printString();
    XXMVP(g_mapVecPer_value_config,"g_mapVecPer_value_config",before);


    auto tmp_cb = [](Person & old_value, const Person & new_value){ FWL_LOG_INFO(FWL_LOG_ROOT()) << "old_value:" << old_value.printString() << ",new value:"<< new_value.printString(); };
    g_person_value_config -> addListenerCb(1,tmp_cb);

    FWL_LOG_INFO(FWL_LOG_ROOT()) << "Load config file";
    YAML::Node root = YAML::LoadFile("../test.yml");
    fwl::Config::loadYamlConfig(root);

    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after g_person_value_config:" <<  (nullptr == g_person_value_config ? "nullptr" : g_person_value_config -> toString());
    FWL_LOG_INFO(FWL_LOG_ROOT()) << "after g_person_value_config yaml:" << g_person_value_config -> getValue().printString();
    XXMVP(g_mapVecPer_value_config,"g_mapVecPer_value_config",after);
}

//日志和配置文件整合之后测试
void testLogConfig(){
    FWL_LOG_WARN(FWL_LOG_ROOT()) << " root test before";
    fwl::Logger::ptr sys_logger = FWL_LOG_NAME("system");
    FWL_LOG_FATAL(sys_logger) << " system test before";
    
    std::cout << "--------------------------" << std::endl;
    std::cout << fwl::loggerMgr::getInstance() -> toYAMLString() << std::endl;
    std::cout << "--------------------------" << std::endl;
    
    YAML::Node root = YAML::LoadFile("../log.yml");
    fwl::Config::loadYamlConfig(root);
   
    std::cout << "--------------------------" << std::endl;
    std::cout << fwl::loggerMgr::getInstance() -> toYAMLString() << std::endl;
    std::cout << "--------------------------" << std::endl;
    
    FWL_LOG_WARN(FWL_LOG_ROOT()) << " root test after";
    FWL_LOG_FATAL(sys_logger) << " system test after";
}

int main(){
    //testPersonConfig();
    //testConfig();
   testLogConfig();
    return 0;
}

#undef XX
#undef XXM
#undef XXMVP
