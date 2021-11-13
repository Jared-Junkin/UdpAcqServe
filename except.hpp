/*---------------------------------------------------------------------------*
*
*  File:    except.hpp
*  Date:    7/24/2009
*  Author:  Eric Raaen
*
*  Exception classes derived from std::exception
*
*  $Log: except.hpp,v $
*  Revision 1.1  2009/08/20 17:27:35  er2
*  Ported to Qt4 - still needs testing
*
*
*---------------------------------------------------------------------------*/
#ifndef EXCEPT_HPP
#define EXCEPT_HPP
#include <stdio.h>
#include <exception>
#include <string>
#include <QString>
                                                               
namespace VacPortExceptionPrivate
{
   extern const char timeoutExceptionName[];
   extern const char invalidParameterName[]; 
   extern const char invalidRequestName[];
   extern const char fileIOExceptionName[];
}

//*---------------------------------------------------------------------------
//
// Class: StdExceptionBase : std::exception
//
// Move VAC base exceptions to std::exception base hierarchy
//
//----------------------------------------------------------------------------
class StdExceptionBase : public std::exception
{
   typedef std::exception inherited;

public:
   StdExceptionBase() throw();
   StdExceptionBase(const char *s) throw();
   StdExceptionBase(const std::string& s) throw();

   StdExceptionBase(const StdExceptionBase& cp) throw();
   StdExceptionBase& operator=(const StdExceptionBase& rhs) throw();
   virtual ~StdExceptionBase() throw();


   virtual const char *what() const throw();
   const char *text () const;

   void setText (const char *txt);
   void setText (const std::string& txt);
   void appendText (const char *moreText);
   void appendText (const std::string& moreText);

#ifdef QSTRING_H
   StdExceptionBase(const QString& s) throw();
   void setText (const QString& txt);
   void appendText (const QString& moreText);
#endif

#if 0
   enum Severity {unrecoverable, recoverable};
   bool isRecoverable () const;
   void setSeverity (Severity s);

   void setErrorId (unsigned long id);
   unsigned long errorId () const;
#endif

private:
   std::string text_;

};

typedef StdExceptionBase Exception;



#endif



