#include "ws_servlet.h"
#include <fnmatch.h>

namespace fwl{
namespace http{

/**
 * @brief set response according to reuqest
 * */
void FunctionWsServlet::handle(HttpRequest::ptr req, WsSession::ptr session){
	if(m_cb){
		m_cb(req, session);
	}
}
/**
 * @brief constructor
 * @param[in] name: servlet name 
 * @param[in] default_servlet
 * */
WsServletMatcher::WsServletMatcher(const std::string & name, WsServlet::ptr default_servlet):
	WsServlet(name),
	m_default(default_servlet){
		if(!m_default){
			//no default,set default response
			m_default.reset(new FunctionWsServlet([](HttpRequest::ptr req, WsSession::ptr sess){
						//default close websocket
						sess -> sendMessage(OPCODE::CLOSE, true, false, "1000");
						}));				
		}
	}

void WsServletMatcher::handle(HttpRequest::ptr req, WsSession::ptr session){
	auto slt = getServlet(req -> getPath());
	if(slt){
		slt -> handle(req, session);
	}
}

/**
 * @brief add complete servlet
 * @param[in] url 
 * @param[in] slt : function servlet
 * */
int WsServletMatcher::addCompleteServlet(const std::string & url, WsServlet::ptr slt){
	if(url.empty() || !slt){
		return -1;
	}
	MutexType::WriteLock wLock(m_mutex);
	m_complete_datas[url] = slt;
	return 0;
}

/**
 * @brief add complete servlet
 * @param[in] url 
 * @param[in] fun : function
 * */
int WsServletMatcher::addCompleteServlet(const std::string & url, fun_type cb){
	return addCompleteServlet(url, std::make_shared<FunctionWsServlet>(cb));
}

/**
 * @brief add fuzzy matching servlet
 * @param[in] url 
 * @param[in] slt : function servlet
 * */
int WsServletMatcher::addFuzzyServlet(const std::string & url, WsServlet::ptr slt){
	if(url.empty() || !slt){
		return -1;
	}
	MutexType::WriteLock wLock(m_mutex);
	m_fuzzy_datas[url] = slt;
	return 0;
}

/**
 * @brief add fuzzy matching servlet
 * @param[in] url 
 * @param[in] fun : function
 * */
int WsServletMatcher::addFuzzyServlet(const std::string & url, fun_type cb){
	return addFuzzyServlet(url, std::make_shared<FunctionWsServlet>(cb));
}

/**
 * @brief add servlet
 * @param[in] url 
 * */
int WsServletMatcher::delServlet(const std::string & url){
	MutexType::WriteLock wLock(m_mutex);
	//erase complete matching function 
	m_complete_datas.erase(url);
	//erase fuzzy matching function 
	m_fuzzy_datas.erase(url);	
	return 0;
}

/**
 * @brief get complete matching servlet
 * @param[in] url 
 * */
WsServlet::ptr WsServletMatcher::getServlet(const std::string & url){
	//start complete matching
	auto data = m_complete_datas.find(url);
	if(data != m_complete_datas.end()){
		return data -> second;
	}
	//complete matiching failed, then start fuzzy mathcing
	for(auto & it : m_fuzzy_datas){
		if(0 == fnmatch(it.first.c_str(), url.c_str(), 0)){
			return it.second;
		}
	}
	return m_default;
}

}
}
