/**
 * @file ws_servlet.h
 * @brief websocket servlet encapsulation(return response based on URL)
 * @auth fwl
 * @email 1822363660@qq.com 
 * @data 2024-03-29
 * */

#ifndef __H_WS_SERVLET__
#define __H_WS_SERVLET__ 

#include <memory>
#include <map>
#include <string>
#include <functional>
#include "../mutex.h"
#include "ws_session.h"

namespace fwl{
namespace http{
/**
 * @brief base class
 * */
class WsServlet{
public:
	typedef std::shared_ptr<WsServlet> ptr;	
	/**
	 * @brief constructor
	 * */
	WsServlet(const std::string & name = ""):
		m_name(name){}

	/**
	 * @brief destructor
	 * */
	virtual ~WsServlet(){}

	/**
	 * @brief set name
	 * */
	void setName(const std::string & name) {	m_name = name;	}

	/**
	 * @brief get name 
	 * */
	std::string getName() const {	return m_name;	}
	
	/**
	 * @brief set response according to reuqest
	 * */
	virtual void handle(HttpRequest::ptr req, WsSession::ptr session) = 0;
private:
	std::string m_name;
};

/**
 * @brief function servlet
 * */
class FunctionWsServlet : public WsServlet{
public:
	typedef std::function<void(HttpRequest::ptr, WsSession::ptr)> on_cb;
	/**
	 * @brief constructor
	 * */
	FunctionWsServlet(on_cb cb, const std::string & m_name = ""):
		WsServlet(m_name),
		m_cb(cb){}
	
	/**
	 * @brief set response according to reuqest
	 * */
	void handle(HttpRequest::ptr req, WsSession::ptr session) override;
private:
	on_cb m_cb;
};

/**
 * @brief matching specific Servlet
 * */
class WsServletMatcher : public WsServlet{
public:
	typedef std::shared_ptr<WsServletMatcher> ptr;
	typedef std::function<void(HttpRequest::ptr, WsSession::ptr)> fun_type;
	typedef RWMutex MutexType;
	/**
	 * @brief constructor
	 * @param[in] name: servlet name 
	 * @param[in] default_servlet
	 * */
	WsServletMatcher(const std::string & name = "", WsServlet::ptr default_servlet = nullptr);

	void handle(HttpRequest::ptr req, WsSession::ptr session) override;

	/**
	 * @brief add complete servlet
	 * @param[in] url 
	 * @param[in] slt : function servlet
	 * */
	int addCompleteServlet(const std::string & url, WsServlet::ptr slt);

	/**
	 * @brief add complete servlet
	 * @param[in] url 
	 * @param[in] fun : function
	 * */
	int addCompleteServlet(const std::string & url, fun_type fun);
	
	/**
	 * @brief add fuzzy matching servlet
	 * @param[in] url 
	 * @param[in] slt : function servlet
	 * */
	int addFuzzyServlet(const std::string & url, WsServlet::ptr slt);

	/**
	 * @brief add fuzzy matching servlet
	 * @param[in] url 
	 * @param[in] fun : function
	 * */
	int addFuzzyServlet(const std::string & url, fun_type fun);
	
	/**
	 * @brief add servlet
	 * @param[in] url 
	 * */
	int delServlet(const std::string & url);
	
	/**
	 * @brief get matching servlet
	 * @param[in] url 
	 * */
	WsServlet::ptr getServlet(const std::string & url);
	
private:
	//Read Write Lock 
	MutexType m_mutex;	
	//complete matching 
	std::map<std::string, WsServlet::ptr> m_complete_datas;
	//fuzzy matching 
	std::map<std::string, WsServlet::ptr> m_fuzzy_datas;
	//default servlet 
	WsServlet::ptr m_default;
};

}
}

#endif
