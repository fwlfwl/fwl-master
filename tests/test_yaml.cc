#include"../fwl/base_head.h"
#include"../fwl/log.h"
#include<yaml-cpp/yaml.h>

void print_yaml(const YAML::Node & node, int level =  0){
    if(node.IsScalar()){
        FWL_LOG_INFO(FWL_LOG_ROOT()) << std::string(level * 4,' ')
            << node.Scalar() << " - " << node.Tag() << " - " << level;
    }else if(node.IsNull()){
        FWL_LOG_INFO(FWL_LOG_ROOT()) << std::string(level * 4, ' ')
            << "NULL -" << node.Tag() << " - " << level;
    }else if(node.IsMap()){
        for(auto it = node.begin(); it != node.end(); ++it){
           FWL_LOG_INFO(FWL_LOG_ROOT()) << std::string(level * 4, ' ')
                << it -> first << " - " << it->first.Tag() << " - " << level;
            print_yaml(it->second,level +  1);
        }
    }else if(node.IsSequence()){
        for(size_t i = 0; i < node.size(); ++i){
            FWL_LOG_INFO(FWL_LOG_ROOT()) << std::string(level * 4, ' ')
                << i << " - " << node[i].Tag() << " - " << level;
            print_yaml(node[i],level+ 1);
        }
    }
}

void test_yaml(){
    YAML::Node node = YAML::LoadFile("../log.yml");
    //FWL_LOG_INFO(FWL_LOG_ROOT()) << node;
    print_yaml(node);
}

int main()
{
    test_yaml();
}

