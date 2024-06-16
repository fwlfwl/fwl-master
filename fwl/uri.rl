#include <string.h>
#include <sstream>
#include "uri.h"

namespace fwl{

/**
 * @brief uri parser by ragel 
 * @details 
 * 		line 9 to line 118
 * 		copy from [https://blog.csdn.net/weixin_43798887/article/details/116100949]	
 * */
%%{
    # See RFC 3986: http://www.ietf.org/rfc/rfc3986.txt

    machine uri_parser;

    gen_delims = ":" | "/" | "?" | "#" | "[" | "]" | "@";
    sub_delims = "!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "=";
    reserved = gen_delims | sub_delims;
    unreserved = alpha | digit | "-" | "." | "_" | "~";
    pct_encoded = "%" xdigit xdigit;

    action marku { mark = fpc; }

    action save_scheme
    {
		if(mark){
			uri->setScheme(std::string(mark, fpc - mark));
		}
        mark = NULL;
    }
    action save_userinfo
    {
		if(mark){
			uri->setUserinfo(std::string(mark, fpc - mark));
        }
		mark = NULL;
    }
    action save_host
    {
		if(mark){
			uri->setHost(std::string(mark, fpc - mark));
		}
		mark = NULL;
    }
    action save_path
    {
        //std::cout << (mark == nullptr) << " " << (fpc -mark) << std::endl;
		if(mark){
			uri->setPath(std::string(mark, fpc - mark));
		}
        mark = NULL;
    }
    action save_port
    {
        if (fpc != mark)
        {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
    action save_query
    {
        if(mark){
			uri->setQuery(std::string(mark, fpc - mark));
		}
        mark = NULL;
    }
    action save_fragment
    {
        if(mark){
			uri->setFragment(std::string(mark, fpc - mark));
		}
        mark = NULL;
    }

    scheme = (alpha (alpha | digit | "+" | "-" | ".")*) >marku %save_scheme;
    userinfo = (unreserved | pct_encoded | sub_delims | ":")*;
    dec_octet = digit | [1-9] digit | "1" digit{2} | 2 [0-4] digit | "25" [0-5];
    IPv4address = dec_octet "." dec_octet "." dec_octet "." dec_octet;
    h16 = xdigit{1,4};
    ls32 = (h16 ":" h16) | IPv4address;
    IPv6address = (                         (h16 ":"){6} ls32) |
                  (                    "::" (h16 ":"){5} ls32) |
                  ((             h16)? "::" (h16 ":"){4} ls32) |
                  (((h16 ":"){1} h16)? "::" (h16 ":"){3} ls32) |
                  (((h16 ":"){2} h16)? "::" (h16 ":"){2} ls32) |
                  (((h16 ":"){3} h16)? "::" (h16 ":"){1} ls32) |
                  (((h16 ":"){4} h16)? "::"              ls32) |
                  (((h16 ":"){5} h16)? "::"              h16 ) |
                  (((h16 ":"){6} h16)? "::"                  );
    IPvFuture = "v" xdigit+ "." (unreserved | sub_delims | ":")+;
    IP_literal = "[" (IPv6address | IPvFuture) "]";
    reg_name = (unreserved | pct_encoded | sub_delims)*;
    host = IP_literal | IPv4address | reg_name;
    port = digit*;

# pchar = unreserved | pct_encoded | sub_delims | ":" | "@";
# add (any -- ascii) support chinese
    pchar = ( (any -- ascii ) | unreserved | pct_encoded | sub_delims | ":" | "@" );
    segment = pchar*;
    segment_nz = pchar+;
    segment_nz_nc = (pchar - ":")+;

# path = path-abempty    ; begins with "/" or is empty
#      / path-absolute   ; begins with "/" but not "//"
#      / path-noscheme   ; begins with a non-colon segment
#      / path-rootless   ; begins with a segment
#      / path-empty      ; zero characters
    path_abempty = ("/" segment)* >marku %save_path;
    path_absolute = ("/" (segment_nz ("/" segment)*)?) >marku %save_path;
    path_noscheme = segment_nz_nc ("/" segment)* >marku %save_path;
    path_rootless = segment_nz ("/" segment)* >marku %save_path;
    path_empty = "" >marku %save_path;


    authority = (userinfo >marku %save_userinfo "@")? host >marku %save_host (":" port >marku %save_port)?;

    query = (pchar | "/" | "?")* >marku %save_query;
    fragment = (pchar | "/" | "?")* >marku %save_fragment;

    hier_part = ("//" authority path_abempty) | path_absolute | path_rootless | path_empty;

    relative_part = ("//" authority path_abempty) | path_absolute | path_noscheme | path_empty;

    relative_ref = relative_part ("?" query)? ("#" fragment)?;

    URI = scheme ":" hier_part ("?" query)? ("#" fragment)?;

    URI_reference = URI | relative_ref;

    main := URI_reference;
    write data;
}%%

//______Uri Implementation_____

/**
 * Create uri
 * */
Uri::ptr Uri::Create(const std::string & uriStr){
	Uri::ptr uri(new Uri);
	int cs = 0;
	const char * mark = 0;
	const char * p = uriStr.c_str();
	const char * pe = p + uriStr.size();
	const char * eof = pe;
	%%write  init;
	%%write exec;
	if(0 == uri -> m_port){
		uri -> setDefaultPort();
	}
	return uri;
}

//set default port 
void Uri::setDefaultPort(){
#define XX(defaultStr, port)	\
	if(0 == strcasecmp(str, defaultStr)){	\
		m_port = port;	\
	}
	
	const char * str = m_scheme.c_str();
	do{
		DEFAULT_PORT_MAP(XX)
	}while(0);	
#undef XX
}

/**
 * @brief is default port
 * */
bool Uri::isDefaultPort() const{
#define XX(defaultStr, port)	\
	if(0 == strcasecmp(str, defaultStr) && m_port == port){	\
		return true;	\
	}
	
 	const char * str = m_scheme.c_str();
	do{
		DEFAULT_PORT_MAP(XX);
	}while(0);
	return false;
}

/**
 * @brief create address by uri
 * */
Address::ptr Uri::CreateAddress(){
	auto addr = Address::LookupAnyIPAddress(m_host);
	if(addr){
		addr -> setPort(m_port);
	}
	return addr;
}

/**
 * @brief streaming output
 * */
std::ostream & Uri::toStream(std::ostream & os) const{
	os	<< m_scheme 
		<< (m_scheme.empty() ? "" : "://")
		<< m_userinfo	
		<< (m_userinfo.empty() ? "" : "@")
		<< m_host 
		<< ((m_host.empty() || isDefaultPort()) ? "" : (":" + std::to_string(m_port)))
		<< m_path
		<< (m_query.empty() ? "" : "?" )
		<< m_query 
	   	<< (m_fragment.empty() ? "" : "#")	
		<< m_fragment;
	return os;
}

/**
 * @brief String output
 * */
std::string Uri::toString() const{
	std::stringstream ss;
	toStream(ss);
	return ss.str();
}	

}
