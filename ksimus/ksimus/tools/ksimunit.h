/***************************************************************************
                          ksimunit.h  -  description
                             -------------------
    begin                : Mon Sep 10 2001
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

#ifndef KSIMUNIT_H
#define KSIMUNIT_H

// C-Includes

// QT-Includes
#include <qstring.h>
#include <qlist.h>

// KDE-Includes

// Forward declaration
class KSimTimeServer;

/**	
	*	The aim of this class is to provide an uniform way to handle units and to convert
	* value without units to values with units (See also @ref KSimUnitList ).
	*
	* The class conatins a string of the unit, convert functions and a range check function.
	*
	* Usage:
	*
	*	Create a set of objects of derived class of this class and collect them into a
	*	@ref KSimUnitList object. It is required that all unit classes uses the same
	* base value (e.g. time units: the "ms" unit convert from ms to s, the "min" unit
	*	from min to s).
	*
	*	Via the @ref KSimUnitList object is it possible to
	* find the prefered unit.
	*	
	*	@short Base class for all unit classes.
  *	@author Rasmus Diekenbrock
  */

class KSimUnitBase
{
public: 
	/** The destructor. */
	virtual ~KSimUnitBase();
	
	/** Converts the value_with_unit into a value without unit.
		*
		*	You have to implement the function properly.
		*/
	virtual double fromUnit (double value_with_unit) const = 0;
	/** Converts the value_without_unit into a value with unit.
		*
		*	You have to implement the function properly.
		*/
	virtual double toUnit (double value_without_unit) const = 0;
	/** Check if the given value_without_unit is in the prefered range.
		*	
		* The default implementation converts the value into a vlaue with unit and
		* compares it with the min and max prefered value (see @ref setMinPrefered and
		* @ref setMaxPrefered).
		*
		*	The return value is always false, if the unit is hidden.
		*/
	virtual bool isPrefered (double value_without_unit) const;
	
	/** Returns the unit string. */
	const QString & getUnitString() const;

	/** Sets the unit as a a hidden unit.
		*	
		* Hidden units are used, if a unit should be known but not be used.
		* e.g. the programm should except inputs in dm (decimeter) but does not used it
		*	by itself.
		*/
	void setHidden(bool hidden);
	/** Returns true, if the unit is a hidden unit.
		*	
		* Hidden units are used, if a unit should be known but not be used.
		* e.g. the programm should except inputs in dm (decimeter) but does not used it
		*	by itself.
		*/
	bool isHidden() const;

	/** Sets the minimum prefered value.
		*/
	void setMinPrefered(double min_with_unit);
	/** Returns the minimum prefered value.
		*/
	double getMinPrefered() const;
	/** Sets the maximum prefered value.
		*/
	void setMaxPrefered(double max_with_unit);
	/** Returns the minimum prefered value.
		*/
	double getMaxPrefered() const;
	/** Sets the minimum and maximum prefered value.
		*/
	void setPrefered(double min_with_unit, double max_with_unit);

protected:
	/** The constructor.
		*/
	KSimUnitBase(const QString & unitString, double preferMin, double preferMax, bool hidden = false);
	

private:
	QString m_unitStr;
	double m_preferMin;
	double m_preferMax;
	bool m_hidden;

};



//########################################################################################
//########################################################################################

/**	Unit class for propertional convertions.
	*
	*	Formula:
	*
	*	value_without_unit = factor * value_with_unit
	*	
	*	@short unit class for propertoinal convertions.
  *	@author Rasmus Diekenbrock
  */


class KSimProportionalUnit : public KSimUnitBase
{
public:
	/** Creates a unit object. */
	KSimProportionalUnit(const QString & unitString, double factor, bool hidden = false);
	/** Creates a unit object. The unit is not hidden. */
	KSimProportionalUnit(const QString & unitString, double factor, double preferMin, double preferMax);
	
	/** Converts the value_with_unit into a value without unit.
		*
		*	Returns: factor * value_with_unit
		*/
	virtual double fromUnit (double value_with_unit) const;
	/** Converts the value_without_unit into a value with unit.
		*
		*	Returns: value_without_unit / factor
		*/
	virtual double toUnit (double value_without_unit) const;
	
	/** Returns the factor. */
	double getFactor() const;


private:
	double m_factor;

};


//###############################################################
//###############################################################


class KSimUnitTicks : public KSimUnitBase
{
public:
	/** Creates a unit object. */
	KSimUnitTicks(const KSimTimeServer & timeServer, const QString & name, bool hidden = false);
	
	/** Converts the value_with_unit into a value without unit.
		*
		*	You have to implement the function properly.
		*/
	virtual double fromUnit (double value_with_unit) const;
	/** Converts the value_without_unit into a value with unit.
		*
		*	You have to implement the function properly.
		*/
	virtual double toUnit (double value_without_unit) const;
	
private:
	const KSimTimeServer & m_timeServer;
};

//########################################################################################
//########################################################################################

/**	Class for unit management.
	*
	*	Finds a unit.
	*	
	*	@see KSimUnitBase
	*	@short Class for unit management.
  *	@author Rasmus Diekenbrock
  */


class KSimUnitList : public QList<KSimUnitBase>
{
public:
	/** The constructor. */
	
	KSimUnitList(const QString & listName);
	/** Adds a new unit to the list.
		*/
	void add(const KSimUnitBase * unit);
	/** Sets a default unit. This unit is used, if no prefered unit is found. The unit must exist
		*	this list.
		*	A unknown unit or empty string removes the default unit.
		*/
	void setDefaultUnit(const QString & unitStr);
	/** Returns the default unit.
		*	Returns an empty string, if no default unit is selected.
		*/
	const QString & getDefaultUnit() const;
	
	/** Find the prefered unit for the given reference. If more than one prefered unit exist, the
		*	first found unit is returned. If no prefered unit is found, the default unit or, if no default
		* unit is selected, a null pointer is returned.
		*/
	const KSimUnitBase * findPrefered(double reference) const;
	/** Returns the unit object which matches the given unit string.
		*	Returns a null pointer if no unit object is found.
		*/
	const KSimUnitBase * findUnit(const QString & unitStr) const;
	
	/** Returns true, if this is a static unit list.
		*	Some predefined, commonly usabel unit list are static. This saves memory and speed.
		*/
	bool isStatic() const;

	/** Returns the name of the unit list. Use a proper name for the list! */		
	const QString & getListName() const;

	/** Returns the number of visible units. */
	unsigned int countVisible() const;

	/** Returns the number of hidden units. */
	unsigned int countHidden() const;


protected:
	
	/** Internal function. Sorts the unit objects. */
	virtual int compareItems(QCollection::Item di1, QCollection::Item di2);
	
	/** Internal function. Sets the unit list static.
			The object will not be deleted during deleting a multi unit list (@ref KSimMultiUnitList).*/
	void setStatic(bool setStatic);
	
private:
	QString m_listName;
	QString m_defaultUnit;
	bool m_isStatic;
};

/** Adds a unit to a unit list.
	*	@see KSimUnitList
	*	@see KSimUnitBase
	*/
inline KSimUnitList & operator<<( KSimUnitList & list, KSimUnitBase * unit )
{
	list.add(unit);
	return list;
}


//########################################################################################
//########################################################################################

/**	Class for static unit list.
	*
	*	Use this class, if you use a unit list object in more than one multi unit list.
	*	
	*	@short Class for static unit lists.
  *	@author Rasmus Diekenbrock
  */

class KSimUnitListStatic : public KSimUnitList
{
public:
	/** The constructor. Creates a empty, static unit list. */
	KSimUnitListStatic(const QString & listName);

	/** Returns a static unit list with time units.
		*
		* Units: ps, ns, µs, ms, s
		*
		* Hidden units: us(==µs), min, h, a
		*
		*	Default unit: s
		*
		*	Base unit: s
		*/
	static const KSimUnitListStatic & getTimeUnitList();
	/** Returns a static unit list with length units.
		*
		* Units: am, fm, pm, nm, µm, mm, m, km, Mm, Gm, Tm
		*
		* Hidden units: um(==µm), cm, dm
		*
		*	Default unit: m
		*
		*	Base unit: m
		*/
	static const KSimUnitListStatic & getLengthUnitList();

};

//########################################################################################
//########################################################################################

/**	Class for manage more than one unit list.
	*
	*	
	*	
	*	@see KSimUnitList
	*	@short Class for manage more than one unit list.
  *	@author Rasmus Diekenbrock
  */

class KSimMultiUnitList
{
public:
	/** Creates an empty multi unit list. */
	KSimMultiUnitList();
	/** The destructor. */
	~KSimMultiUnitList();
	
	/** Removes all unit lists from this multi unit list. */
	void clear();
	/** Returns the number of unit lists. */
	unsigned int count() const { return m_list.count(); };
	
	/** Finds a unit list by the list name. */
	const KSimUnitList * findUnitList(const QString & listName) const;
	
	/** Finds a unit by the unit string. If listName is a valid unit list name
		*	the search starts with this unit list. If not or not found the unit is searched in
		*	all unit lists. The function returns a pointer to the unit object or a null pointer,
		*	if not found.
		*	If listName is not null, listName is set to the unit list name of the found unit object
		*	or an empty string if no unit object is found.
		*/
	const KSimUnitBase * findUnit(const QString & unitStr, QString * listName = 0) const;
	/** Finds a unit by the prefered value. If listName is a valid unit list name
		*	the search starts with this unit list. If not or not found the unit is searched in
		*	all unit lists. The function returns a pointer to the unit object or a null pointer,
		*	if not found.
		*	If listName is not null, listName is set to the unit list name of the found unit object
		*	or an empty string if no unit object is found.
		*/
	const KSimUnitBase * findPrefered(double reference, QString * listName = 0) const;
	
	/** Adds a unit list to the multi unit list.
	*/
	void append(const KSimUnitList & unitList);
	
	/** Returns a list of all unit lists. */
	QList<KSimUnitList> & getList() { return m_list; };
	/** Returns a list of all unit lists. */
	const QList<KSimUnitList> & getList() const { return m_list; };
	
	
protected:
	/** Internal */
	QList<KSimUnitList> m_list;

};

#endif
