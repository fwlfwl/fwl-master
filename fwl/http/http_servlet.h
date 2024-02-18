/**
 * @file http_servlet.h
 * @brief http_servlet encapsulation(return response based on URL)
 * @auth fwl
 * @email 1822363660@qq.com 
 * @data 2024-02-18
 * */

#ifndef __H_HTTP_SERVLET__
#define __H_HTTP_SERVLET__ 

#include <memory>
#include <map>
#include <string>
#include <functional>
#include "../mutex.h"
#include "http.h"
#include "http_session.h"

namespace fwl{
namespace http{
/**
 * @brief base class
 * */
class Servlet{
public:
	typedef std::shared_ptr<Servlet> ptr;	
	/**
	 * @brief constructor
	 * */
	Servlet(const std::string & name = ""):
		m_name(name){}

	/**
	 * @brief destructor
	 * */
	virtual ~Servlet(){}

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
	virtual void handle(HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session) = 0;
private:
	std::string m_name;
};

/**
 * @brief function servlet
 * */
class FunctionServlet : public Servlet{
public:
	typedef std::function<void(HttpRequest::ptr, HttpResponse::ptr, HttpSession::ptr)> callback;
	/**
	 * @brief constructor
	 * */
	FunctionServlet(callback cb, const std::string & m_name = "");
	
	/**
	 * @brief set response according to reuqest
	 * */
	void handle(HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session) override;
private:
	callback m_cb;
};

/**
 * @brief matching specific Servlet
 * */
class ServletMatcher : public Servlet{
public:
	typedef std::shared_ptr<ServletMatcher> ptr;
	typedef std::function<void(HttpRequest::ptr, HttpResponse::ptr, HttpSession::ptr)> fun_type;
	typedef RWMutex MutexType;
	/**
	 * @brief constructor
	 * @param[in] name: servlet name 
	 * @param[in] default_servlet
	 * */
	ServletMatcher(const std::string & name = "", Servlet::ptr default_servlet = nullptr);

	void handle(HttpRequest::ptr req, HttpResponse::ptr res, HttpSession::ptr session) override;

	/**
	 * @brief add complete servlet
	 * @param[in] url 
	 * @param[in] slt : function servlet
	 * */
	int addCompleteServlet(const std::string & url, Servlet::ptr slt);

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
	int addFuzzyServlet(const std::string & url, Servlet::ptr slt);

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
	Servlet::ptr getServlet(const std::string & url);
	
private:
	//Read Write Lock 
	MutexType m_mutex;	
	//complete matching 
	std::map<std::string, Servlet::ptr> m_complete_datas;
	//fuzzy matching 
	std::map<std::string, Servlet::ptr> m_fuzzy_datas;
	//default servlet 
	Servlet::ptr m_default;
};

}
}

#endif
