/***************************************************************************
                          ksimdebug.h  -  description
                             -------------------
    begin                : Tue Feb 6 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : ksimus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _KSIMDEBUG_H_
#define _KSIMDEBUG_H_

#include <kdebug.h>

// Use the global define DEBUG to switch on debug messages globally
// Use the local define NO_KSIMDEBUG to switch off all debug messages in a file
// Use the local define NO_KSIMDEBUG_ENTRYEXIT to switch off entry and exit messages

//#define NO_KSIMDEBUG_INSTANCE_COUNTER 1
#define NO_KSIMDEBUG_ENTRYEXIT 1

// The defines must set before this include file !!!


#if !defined(NO_KSIMASSERT_FATAL)
	#define QT_FATAL_ASSERT 1
#endif //!defined(NO_KSIMASSERT_FATAL)

class KSimDebugIndent
{
public:
	KSimDebugIndent()
		:	m_indent(0)
	{};
	
	void inc()	{ m_indent++; };
	void dec()	{ m_indent--; };
	QString print() const;
	
private:
	int m_indent;
};
extern KSimDebugIndent __kSimDebugIndent__;


class KSimDebugFuncEntryExit
{
public:
	KSimDebugFuncEntryExit(const char * txt)
		: m_txt(txt)
	{ 
		kdDebug() << __kSimDebugIndent__.print() << ">> " << txt << endl;
		__kSimDebugIndent__.inc(); 
	}
	~KSimDebugFuncEntryExit()
	{ 
		__kSimDebugIndent__.dec();
		kdDebug() << __kSimDebugIndent__.print() << "<< " << m_txt << endl; 
	};
private:
	const char * m_txt;
};

class KSimDebugInstanceCounter
{
public:
	KSimDebugInstanceCounter(const char * name)
		:	m_name(name),
			m_cnt(0)
	{};
	~KSimDebugInstanceCounter()
	{
		if (m_cnt != 0)
			printResult();
	}
	void inc() { m_cnt++; };
	void dec() { m_cnt--; };
	void printResult() const; 
private:
	const char * m_name;
	int m_cnt;
};


#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	#define __KSIMDEBUG__DEBUG_INFO__    __FILE__ << "(" << __LINE__ << ") "
	#define KSIMDEBUG(str)               {kdDebug() << __kSimDebugIndent__.print() << __KSIMDEBUG__DEBUG_INFO__ << str << endl;}
	#define KSIMDEBUG_VAR(str,x)         {kdDebug() << __kSimDebugIndent__.print() << __KSIMDEBUG__DEBUG_INFO__ << str << " " << #x << " = " << x << endl;}
	#define KSIMDEBUG_FUNCTION           {kdDebug() << __kSimDebugIndent__.print() << k_funcinfo << endl;}
	#define KSIMASSERT(x)                Q_ASSERT(x)
	#define KSIMASSERT_VAR(test, var)    { if (!(test)) {kdDebug() << __kSimDebugIndent__.print() << #var << " = " << var << endl; Q_ASSERT(test); } }
	#define KSIMVERIFY(x)                Q_ASSERT(x)
	#define KSIMVERIFY_VAR(test, var)    { if (!(test)) {kdDebug() << __kSimDebugIndent__.print() << #var << " = " << var << endl; Q_ASSERT(test); } }
	
	#if defined(__GNUC__) && !defined(NO_KSIMDEBUG_ENTRYEXIT)
		#define KSIMDEBUG_ENTRYEXIT KSimDebugFuncEntryExit __kSimDebugFuncEntryExit__(__PRETTY_FUNCTION__);
	#endif // defined(__GNUC__) && !defined(NO_KSIMDEBUG_ENTRYEXIT)

	#if !defined(NO_KSIMDEBUG_INSTANCE_COUNTER)
		#define KSIMDEBUG_INSTANCE_COUNTER(className) static KSimDebugInstanceCounter __kSimDebugInstanceCounter__##className(#className);
		#define KSIMDEBUG_INSTANCE_INC(className) __kSimDebugInstanceCounter__##className.inc();
		#define KSIMDEBUG_INSTANCE_DEC(className) __kSimDebugInstanceCounter__##className.dec();
	#endif // !defined(NO_KSIMDEBUG_INSTANCE_COUNTER)
#else // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	#define KSIMDEBUG(str)
	#define KSIMDEBUG_VAR(str,x)
	#define KSIMDEBUG_FUNCTION
	#define KSIMASSERT(x)
	#define KSIMASSERT_VAR(test, var)
	#define KSIMVERIFY(x)                Q_ASSERT(x)
	#define KSIMVERIFY_VAR(test, var)    { if (!(test)) {kdDebug() << __kSimDebugIndent__.print() << #var << " = " << var << endl; Q_ASSERT(test); } }
#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)

#ifndef KSIMDEBUG_ENTRYEXIT
	#define KSIMDEBUG_ENTRYEXIT
#endif
#ifndef KSIMDEBUG_INSTANCE_COUNTER
	#define KSIMDEBUG_INSTANCE_COUNTER(className)
	#define KSIMDEBUG_INSTANCE_INC(className)
	#define KSIMDEBUG_INSTANCE_DEC(className)
#endif


#endif  // _KSIMDEBUG_H_
