/*---------------------------------------------------------------------------*
*
*  File:    except.cpp
*  Date:    7/27/2009
*  Author:  Eric Raaen
*
*  Exception classes derived from std::exception
*  
*
*  $Log: except.cpp,v $
*  Revision 1.1  2009/08/20 17:27:35  er2
*  Ported to Qt4 - still needs testing
*
*
*---------------------------------------------------------------------------*/
#include "except.hpp"


namespace VacPortExceptionPrivate
{
   extern const char timeoutExceptionName[] = "TimeoutException";
   extern const char invalidParameterName[] = "InvalidParameter";
   extern const char invalidRequestName[] = "InvalidRequest";
   extern const char fileIOExceptionName[] = "FileIOException";
}



#ifdef TEST_IT
#include <iostream>
using namespace std;

int main(int, const char **)
{
   cout << "here comes" << endl;
   try
   {
      throw TimeoutException("Just testing");
   }
   catch (std::exception& ex)
   {
      cout << "Caught " << ex.what() << endl;
   }
   return 0;

}
#endif



//*---------------------------------------------------------------------------
//
// StdExceptionBase::StdExceptionBase()
// StdExceptionBase::StdExceptionBase(cp)
// StdExceptionBase::StdExceptionBase(const char *what)
// StdExceptionBase::StdExceptionBase(const QString& what)
// StdExceptionBase::StdExceptionBase(const std::string& what)
// 
//
// Constructors and destructor
//
//----------------------------------------------------------------------------
StdExceptionBase :: StdExceptionBase() throw () : inherited() { }
StdExceptionBase :: StdExceptionBase(const StdExceptionBase& cp) throw() : inherited(cp), text_(cp.text_) { }
StdExceptionBase :: StdExceptionBase(const char *what) throw() : inherited(), text_(what) { }
StdExceptionBase :: StdExceptionBase(const std::string& what) throw() : inherited(), text_(what) { }
#ifdef QSTRING_H
StdExceptionBase :: StdExceptionBase(const QString& s) throw(): 
   inherited(), 
   text_(s.toStdString()) 
{ }
#endif
StdExceptionBase :: ~StdExceptionBase() throw() { }


//*---------------------------------------------------------------------------
//
// StdExceptionBase::operator=(const StdExceptionBase& rhs)
//
// Assignment operator
//
//----------------------------------------------------------------------------
StdExceptionBase& StdExceptionBase :: operator=(const StdExceptionBase& rhs) throw()
{
   if (&rhs != this)
   {
      inherited::operator=(rhs);
      text_ = rhs.text_;
   }
   return *this;
}

//*---------------------------------------------------------------------------
//
// StdExceptionBase::setText(txt)
// StdExceptionBase::appendText(txt)
//
// Setting/adding text to what()/text() 
//
//----------------------------------------------------------------------------
void StdExceptionBase :: setText(const char *txt) { text_ = txt; }
void StdExceptionBase :: setText(const std::string& txt) { text_ = txt; }
void StdExceptionBase :: appendText(const char *txt) { text_ += txt; }
void StdExceptionBase :: appendText(const std::string &txt) { text_ += txt; }
#ifdef QSTRING_H
void StdExceptionBase :: setText(const QString& txt) { text_ = txt.toStdString(); }
void StdExceptionBase :: appendText(const QString& txt) { text_ += txt.toStdString(); }
#endif

//*---------------------------------------------------------------------------
//
// StdExceptionBase::what() const
// StdExceptionBase::text() const
//
//----------------------------------------------------------------------------
const char * StdExceptionBase::what() const throw() { return text_.c_str(); }
const char * StdExceptionBase::text() const { return text_.c_str(); }

