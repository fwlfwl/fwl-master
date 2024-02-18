#include <fnmatch.h> //fuzzy function 
#include "http_servlet.h"
#include "../log.h"

#define NOT_FOUND_BODY(url) std::string("<html><head>"	\
"<title>404 Not Found</title>"	\
"</head><body>"	\
"<h1>Not Found</h1>"	\
"<p>The requested URL "+url+" was not found on this server.</p>"	\
"</body></html>")

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{
namespace http{

/**
 * @brief constructor
 * */
FunctionServlet::FunctionServlet(callback cb, const std::string & name):
	Servlet(name),
	m_cb(cb){}

/**
 * @brief set response according to reuqest
 * */
void FunctionServlet::handle(HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session){
	if(m_cb){
		m_cb(req, res, session);
	}else{
		FWL_LOG_WARN(g_logger) << "Callback is null, do nothing";
	}
}

/**
 * @brief constructor
 * @param[in] name: servlet name 
 * @param[in] default_servlet
 * */
ServletMatcher::ServletMatcher(const std::string & name, Servlet::ptr default_servlet):
	Servlet(name),
	m_default(default_servlet){
		if(!m_default){
			//no default,set default response
			m_default.reset(new FunctionServlet([](HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session){
						res -> setStatus(fwl::http::HttpStatus::NOT_FOUND);
						res -> setHeader("Content-Type", "text/html");
					  	res -> setBody(NOT_FOUND_BODY(req -> getPath()));
						}));				
		}
	}

void ServletMatcher::handle(HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session){
	auto slt = getServlet(req -> getPath());
	if(slt){
		slt -> handle(req, res, session);
	}
}

/**
 * @brief add complete servlet
 * @param[in] url 
 * @param[in] slt : function servlet
 * */
int ServletMatcher::addCompleteServlet(const std::string & url, Servlet::ptr slt){
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
int ServletMatcher::addCompleteServlet(const std::string & url, fun_type cb){
	return addCompleteServlet(url, std::make_shared<FunctionServlet>(cb));
}

/**
 * @brief add fuzzy matching servlet
 * @param[in] url 
 * @param[in] slt : function servlet
 * */
int ServletMatcher::addFuzzyServlet(const std::string & url, Servlet::ptr slt){
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
int ServletMatcher::addFuzzyServlet(const std::string & url, fun_type cb){
	return addFuzzyServlet(url, std::make_shared<FunctionServlet>(cb));
}

/**
 * @brief add servlet
 * @param[in] url 
 * */
int ServletMatcher::delServlet(const std::string & url){
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
Servlet::ptr ServletMatcher::getServlet(const std::string & url){
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
	//not find 
	return m_default;
}
}
}
