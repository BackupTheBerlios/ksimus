/***************************************************************************
                          enumdict.h  -  description
                             -------------------
    begin                : Fri Mar 23 2001
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

#ifndef ENUMDICT_H
#define ENUMDICT_H


class KSimData;
class QString;
template<class type> class Q_EXPORT QDict;

/**	The base class for @ref EnumDict.
	*
	* @internal
	*	@short The base class for @ref EnumDict.
  * @author Rasmus Diekenbrock
  */

class EnumBaseDict
{
private:
	typedef struct
	{
		const char * name;
		int value;
	} tData;

	const tData * m_data;
	QDict<int> * m_dict;

public:
	~EnumBaseDict();
	
	const int * find(const char * name) const;
	int find(const char * name, int defaultValue) const;
	const char * find(int value) const;

	void save(KSimData & conf, const char * key, int value) const;
	int load(const KSimData & conf, const char * key, int defaultValue) const;
	const int * load(const KSimData & conf, const char * key) const;

protected:	
	EnumBaseDict(const tData * pData);
	
};

/** A template class for storing enums.
	*
	* This template provides a wrapper from enums to const char *. The template supports converting
	*	in both dircetion as well as storing in @ref KConfigBase objects.
	*
	*	Define a @ref EnumDict:
	*
	*	<pre>	
	* enum eFoo {eZero, eOne, eTwo, eThree);
	*
	* EnumDict<eFoo> fooDict;
	*
	* EnumDict<eFoo>::tData EnumDict<eFoo>::data[]
	*      = { {"Zero",  eZero},
	*          {"One",   eOne},
	*          {"Two",   eTwo},
	*          {"Three", eThree},
	*          {0,       (eFoo)0} };
	* </pre>
	*
	*	Use a @ref EnumDict:
	*
	*	<pre>	
	* eFoo foo;
	*
	* foo = fooDict.find("Zero", eOne);
	* </pre>
	*
	* @short A template class for storing enums.
  * @author Rasmus Diekenbrock
  */


template<class T>
class EnumDict : public EnumBaseDict
{
public: 
	/** Constructor. Nothing important. */
	EnumDict()
		: EnumBaseDict((const EnumBaseDict::tData*)data) {};

	/** Converts a string to an enum. If the string does not exist in EnumDict<T>::data the
			function returns a null pointer. */
	const T * find(const char * name) const;
	/** Converts a string to an enum. If the string does not exist in EnumDict<T>::data the
			function returns the defaultValue. */
	T find(const char * name, T defaultValue) const;
	/** Converts an enum to a string. If the enum does not exist in EnumDict<T>::data the
			function returns a null pointer. */
	const char * find(T value) const;
	/** Saves the enum in a @ref KSimData object. The enum is stored as string.
		*/
	void save(KSimData & conf, const char * key, T value) const;
	/** Load an enum from a @ref KSimData object. The enum is expected as string. If the string
			does not exist in EnumDict<T>::data the	function returns the defaultValue.
		*/
	T load(const KSimData & conf, const char * key, T defaultValue) const;
	/** Load an enum from a @ref KSimData object. The enum is expected as string. If the string
			does not exist in EnumDict<T>::data the	function returns a null pointer.
		*/
	const T * load(const KSimData & conf, const char * key) const;
	
private:
		/** Stores the relation between one string and one enum. */
		typedef struct {
			const char * name;
			T value;
		} tData;

		/** Implement this data array as shown above. */
		static tData data[];
};


template<class T>
const T * EnumDict<T>::find(const char * name) const
{
	return (const T *)EnumBaseDict::find(name);
};
template<class T>
T EnumDict<T>::find(const char * name, T defaultValue) const
{
	return (T) EnumBaseDict::find(name,(int)defaultValue);
}
template<class T>
const char * EnumDict<T>::find(T value) const
{
	return EnumBaseDict::find((int)value);
}

template<class T>
void EnumDict<T>::save(KSimData & conf, const char * key, T value) const
{
	EnumBaseDict::save(conf, key, (int) value);
}
	
template<class T>
T EnumDict<T>::load(const KSimData & conf, const char * key, T defaultValue) const
{
	return (T) EnumBaseDict::load(conf, key, (int) defaultValue);
}

template<class T>
const T * EnumDict<T>::load(const KSimData & conf, const char * key) const
{
	return (const T *) EnumBaseDict::load(conf, key);
}



#endif
