/***************************************************************************
                          ksimtype.h  -  description
                             -------------------
    begin                : Sun Dec 23 2001
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

#ifndef KSIMTYPE_H
#define KSIMTYPE_H

// C-Includes

// QT-Includes
#include <qstring.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class KSimData;


//#######################################################################################
//#######################################################################################




/**Base class for specific KSimus types.
  *@author Rasmus Diekenbrock
  */


class KSimTypeBase
{
public:
	/** Returns the current value as string. The function must be implemented in a sub class. */
	virtual QString text() const = 0;
	/** Sets a new value. The function must be implemented in a sub class. */
	virtual void setText(const QString & text, bool * ok = 0) = 0;
	
	/** Saves the current value in a @ref KSimData object. Uses the function @ref text. */
	void save(KSimData & config, const char * pKey) const;
	/** Loads the value from a @ref KSimData object. Uses the function @ref setText. */
	bool load(KSimData & config, const char * pKey, const QString& aDefault = QString::null);
};

/** Useful to create a simple text() function for a KSimTypeBase sub class. */
#define MAKE_KSimTypeBase_text(TYPE, CONVERT)                                 \
                     QString TYPE ::text() const { return CONVERT(value()); }

/** Useful to create a simple setText() function for a KSimTypeBase sub class. */
#define MAKE_KSimTypeBase_setText(TYPE, CONVERT)                              \
                     void TYPE ::setText(const QString & text, bool * ok) {   \
                     bool myOk;                                               \
                     int i = CONVERT (&myOk);                                 \
                     if (myOk)  m_value = i;                                  \
                     if (ok)    *ok = myOk; }


//#######################################################################################
//#######################################################################################




/**Template class for specific KSimus types.
  *@author Rasmus Diekenbrock
  */

template<class Type> class KSimTypeTemplate : public KSimTypeBase
{
public:
	/** Creates a new object. */
	KSimTypeTemplate()
	{	};
	
	/** The destructor. */
	virtual ~KSimTypeTemplate()
	{ };
	
	/** Returns the value. This allowes to handle this object like a Type object.
	    E.g. if Type is int the object work like a normal int varibles. */
	operator Type &(){ return m_value; };

	/** Returns the current value. Use this function if you want to read the value from a const object. */
	Type value() const { return m_value; };
	
protected:
	Type m_value;
};



//#######################################################################################
//#######################################################################################


/** This class gives a simple int variable the common interface like defined in @ref KSimTypeBase and
  * @ref KSimTypeTemplate.
  *
  * @author Rasmus Diekenbrock
  */


class KSimInt : public KSimTypeTemplate<int>
{
public:
	/** Constructs a new object with the value zero.*/
	KSimInt()
		: KSimTypeTemplate<int>()
	{	
		m_value = 0;
	};
	
	/** Constructs a new object with the value i.*/
	KSimInt(int i)
	{	
		m_value = i;
	};
	
	/** Constructs a new object with the value like i.*/
	KSimInt(const KSimInt & i)
		: KSimTypeTemplate<int>()
	{	
		m_value = i.value();
	};
	
	/** Returns the current value as string.*/
	virtual QString text() const;
	/** Sets a new value. */
	virtual void setText(const QString & text, bool * ok = 0);
};


//#######################################################################################
//#######################################################################################


/** This class gives a simple unsigned int variable the common interface like defined in @ref KSimTypeBase and
  * @ref KSimTypeTemplate.
  *
  * @author Rasmus Diekenbrock
  */


class KSimUInt : public KSimTypeTemplate<unsigned int>
{
public:
	/** Constructs a new object with the value zero.*/
	KSimUInt()
		: KSimTypeTemplate<unsigned int>()
	{	
		m_value = 0;
	};
	
	/** Constructs a new object with the value i.*/
	KSimUInt(unsigned int u)
	{	
		m_value = u;
	};
	
	/** Constructs a new object with the value like i.*/
	KSimUInt(const KSimUInt & u)
		: KSimTypeTemplate<unsigned int>()
	{	
		m_value = u.value();
	};
	
	/** Returns the current value as string.*/
	virtual QString text() const;
	/** Sets a new value. */
	virtual void setText(const QString & text, bool * ok = 0);
};






#endif
