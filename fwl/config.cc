#include "config.h"

namespace fwl{
    //Config类实现
    void Config::addNodeList(const std::string & name,const YAML::Node & node,std::map<std::string,YAML::Node> & out){
        RWMutexType::ReadLock lock(getMutex());
        if(name.find_first_not_of(CHECK_STRING) != std::string::npos){
            FWL_LOG_ERROR(FWL_LOG_ROOT()) << "Config::addNodeList name illegal";
            return;
        }
        out.insert(std::make_pair(name,node));
        if(node.IsMap()){
            for(auto it = node.begin(); it != node.end(); ++it){
                addNodeList(name.empty() ? it -> first.Scalar() : name + "." + it -> first.Scalar(),
                            it -> second, out);
            }        
        }
    }

    void Config::loadYamlConfig(const YAML::Node & root){
        std::map<std::string,YAML::Node> allNodes;
        addNodeList("",root,allNodes);
        
        for(auto & it : allNodes){
            std::string key = it.first;
            if(key.empty()){
                continue;
            }
            ConfigVarBase::ptr var = lookUpBase(key);       
            if(var){
                if(it.second.IsScalar()){
                    var -> fromString(it.second.Scalar());
                }else{
                    std::stringstream ss;
                    ss << it.second;
                    var -> fromString(ss.str());
                }
            }
        }
    }

    typename ConfigVarBase::ptr Config::lookUpBase(const std::string &name){
        RWMutexType::ReadLock lock(getMutex());
        auto it = getData().find(name);
        return it == getData().end() ? nullptr : it -> second;
    }
}
