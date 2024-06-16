#include "config.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

namespace fwl{
	static Logger::ptr g_logger = FWL_LOG_NAME("system");
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
	
	//从目录中读取所有配置文件
	void Config::LoadFromDir(const std::string & path){
		//读取所有.yml文件
		std::vector<std::string> files;
		FSUnit::ListAllFiles(path, files, ".yml");
		//读取文件到节点中
		ListAllMembers(files); 
	}
	
	//加载所有节点信息
	void Config::ListAllMembers(const std::vector<std::string> & files){
		for(auto & file : files){
			try{
				YAML::Node root = YAML::LoadFile(file);
				struct stat tm;
				if(0 != FSUnit::_lstat(file.c_str(), &tm)){
					continue;
				}
				uint64_t new_mtime = tm.st_mtim.tv_sec;
				if(getFiles()[file] < new_mtime){
					loadYamlConfig(root);
					getFiles()[file] = new_mtime;
				}
			}catch(...){
				FWL_LOG_ERROR(g_logger) << "YAML load config files failed, errno=" << errno 
				   << ",errstr=" << strerror(errno);
				return;	
			}
		}
	}
}
