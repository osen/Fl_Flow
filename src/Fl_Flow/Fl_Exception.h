#include <exception>
#include <string>

template <typename T>
struct Fl_Exception
{
  Fl_Exception(const char* _message) : m_message(_message) { }
  Fl_Exception(const std::string& _message) : m_message(_message) { }
  virtual ~Fl_Exception() throw() {}

  virtual const char* what() const throw()
  {
    return m_message.c_str();
  }

private:
  std::string m_message;

};

#define Fl_Exception Fl_Exception<char>

