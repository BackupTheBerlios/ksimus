/***************************************************************************
                          connectorbase.h  -  description
                             -------------------
    begin                : Sun Oct 1 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef CONNECTORBASE_H
#define CONNECTORBASE_H


// QT includes
#include <qobject.h>
#include <qlist.h>

// KDE includes

// Project includes
#include "types.h"
#include "componentitem.h"
#include "ksimaction.h"
#include "implicitconverter.h"

class QPainter;
class KSimData;

class QPopupMenu;
class KSimusDoc;
class Component;
class ConnectorInfo;
class Wire;
class WireProperty;
class WireColorScheme;
class PropertyWidget;
class WatchItemBase;


/**Base class for all connector classes
  *@author Rasmus Diekenbrock
  */


class ConnectorBase : public QObject, public ComponentItem
{
friend class Wire;
friend class Module;
friend class ConnectorPropertyWidget;

class ConnectorBasePrivate;

	
	Q_OBJECT
	
public:
	virtual ~ConnectorBase();
	
	//**************************************************************************	
	// *** Data manipulation function ***
	
	/** Returns a pointer to the data that's read from the component.
	  * Reimplementations is required. */
	virtual const void * readoutData() const = 0;
	
	/** The function copyData() has to copy data to the output variable.
	  * Reimplementations is required. */
	virtual void copyData(const void * pData) = 0;
	
	/** The function putData() is called if new data has to copied into a input connector.
	  * This function calls the implicit converter before calling copyData() if a converter
	  * is available. */
	void putData(const void * pData)
	{
		if (m_implicitConverter)
		{
			copyData(m_implicitConverter->convert(pData));
		}
		else
		{
			copyData(pData);
		}
	};

	/** Returns the related document. */
	KSimusDoc * getDoc() const;
	
	/** Sets the name which is used by the wire to identify the correct connector in a component.
	  * This name have to be unique in the component. Do not set a translate name.
	  * The untranslated name (see @refsetName, @ref getName) is used as default.
	  */
	void setWireName(const QString & name);

	/** Returns the name which is used by the wire to identify the correct connector in a component.
	  * This name have to be unique in the component.
	  * The untranslated name (see @refsetName, @ref getName) is used as default.
	  */
	QString getWireName() const;
	
	/** Set new connector name. Use a translated name if possible.
	  * The name won't be saved in document, if init is true.
	  * The signal signalProperty() is emitted.
	  */
	void setName(const QString & newName, bool init = false);
	/** Returns the translated connector name. */
	const QString & getName() const;
	/** Returns the translated connector init name. */
	const QString & getInitName() const;
	/** Returns the connected name componentName::connectorName. */
	QString getFullName() const;
	
	/** Set new connector position.
	  * Position is given in grid !!!
	  */
	void setGridPos(const QPoint & position);
	/** Set new connector position.
	  * Position is given in grid !!!
	  */
	void setGridPos(int x, int y);
	/** Returns the current relative connector position.
	  * Position is given in grid !!!
	  */	
	const QPoint & getGridPos() const;
	/** Returns the current relative connector position.
	  */	
	QPoint getRelPos() const;
	/** Returns the current absolute connector position.
	  */	
	QPoint getPos() const;
	/** Returns the position where the wire has to connect. */
	QPoint getWirePos() const;
	/** Returns the position where the wire has to connect. The position is converted to grid. */
	QPoint getWireGridPos() const;
	
	/** Returns true, if the given positon hit the connector. */
	eHitType isHit(int x, int y) const;
	
	/** Returns the connector information object. */
	const ConnectorInfo * getConnInfo() const;

	/** Returns the connector type. */
	const char * getConnType() const;
	
	/** Set the orientation of the connector. */
	void setOrientation(ConnOrientationType orient);
	/** Returns the orientation of the connector. */
	ConnOrientationType getOrientation() const;
	
	/** True, if connector is a input. */
	bool isInput() const;
	/** True, if connector is a output. */
	bool isOutput() const;
	/** True, if connector is a tris state connector. */
	bool isTristate() const;
	
	
	// ### Hide functions
	/** Enables the hide function of the connector. */
	void setHideEnabled(bool ena);
	/** Enables the hide function of the connector. */
	bool isHideEnabled() const;
	/** Set the connector as hidden.
	  * The change won't be saved in document, if init is true.
	  * The signal signalProperty() is emitted.
	  */
	void setHide(bool hide, bool init = false);
	/** Returns true, if connector is hidden. */
	bool isHidden() const;
	/** Returns true, if connector is hidden at init time. */
	bool isInitHidden() const;
		
	
	// ### Negate functions
	/** Enables the negate function of the connector. */
	void setNegateEnabled(bool ena);
	/** Enables the negate function of the connector. */
	bool isNegateEnabled() const;
	/** Set the connector as negated type.
	  * The negType won't be saved in document, if init is true.
	  * The signal signalProperty() is emitted.
	  */
	void setNegate(bool neg, bool init = false);
	/** Returns true, if connector is negated. */
	bool isNegated() const{ return m_negType; };
	
	/** Returns true, if connector is negated at init time. */
	bool isInitNegate() const;

	/** Set the connector erasable. The default is not erasable.
		* This function is used internal.
		*/
	void setErasable(bool erasable);
	/** Returns true, if connector is erasable.
		* This function is used internal.
		*/
	bool isErasable() const;
		
	
  /** Returns the actions object. In this object describes all of this connector required actions. */
	KSimAction & getAction() { return m_myActions; };
	
	/** Load properties.
		Returns true if successful. */
	virtual bool load(KSimData & file);
	/** Save properties. */
	virtual void save(KSimData & file) const;
	
	/** Set the connected wire. Null, if not connected. */
	void setWire(Wire * newWire);
	/** Returns the connected wire. Null, if not connected. */
	Wire * getWire() const;
	
	/** Returns true if the connector is connected to a wire. */
	bool isConnected() const;
	
	/** Returns the @ref WireProperty of the wire to where the connector is connected.
	  * Returns zero if the connector is not connected. Is valid after proceeding all
	  * @ref checkCircuit functions. */
	WireProperty * getWireProperty() const;
	
	/** Sets the @ref WireProperty of the wire to where the connector is connected.
	  * In most cases this function is without any interest. */
	void setWireProperty(WireProperty * wireProperty);
	
	/** Add menu items depend on connetor properties.
	  *	Return true, if items are added. */
	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Creates the property widget. */
	virtual PropertyWidget* propertyWidget(QWidget * parent);
	
	
	/** Checks the connector.
	*   eg. if input is connected.
	*   The default implementation does nothing.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Setup the connector for a new circuit execution.
	*/
	virtual void setupCircuit();
	
	/** Checks the component property. The functions is called after the
	*   property dialog.
	*   Eg. all connectors have unique names.
	*   The default implementation does nothing.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);

	/** Resets the connector.
	  */
	virtual void reset();

	/** Adds the component in the list for execute next cycle. */
	void executeComponentNext();
	/** Adds the @ref WireProperty in the list for execute next cycle. */
	void executeWirePropertyNext();

	
	//**************************************************************************	
	// *** Display functions ***
	
	// Get the colors for the connector
	virtual const WireColorScheme & getColorScheme() const = 0;
	/** Draws the connector. */
	void draw (QPainter * p) const;
	/** Draws the connector on/in the given place and orientation. */
	void draw (QPainter * p, ConnOrientationType orient, int x, int y) const;

	/** Returns a text which represents the current value. */
	virtual QString getValueText() const = 0;
	
	/** Returns a watch item. 
	*   The default implementation return a null pointer. Must reimplemented in a sub class.
	*/
	virtual WatchItemBase * makeWatchItem();
	
	//**************************************************************************
	// *** static funtions
	static const char * convertOrientation(ConnOrientationType orientation);
	static ConnOrientationType convertOrientation(const char * orientation, ConnOrientationType defaultOrient = CO_UNDEF);

protected:
	//**************************************************************************	
	
	/** Constructor of the base class of all connectors.
	  *
	  *
	  * @param comp     The component which contains this connector.
	  * @param name     The *untranslated* connector name. This name is used as internal
	  *                 identifier and should not be translated.
	  * @param i18nName The *translated* connector name. This name is used in the status bar and
	  *                 the property widget.
	  * @param pos      The position of the connector relative to the component top left (measured in grid).
	  * @param orient   Gives the direction from where the connector can be connected.
	  * @param dir      Declares the connector as input, put or tristate
	  * @param ci       Gives the connector the exact type info.
	  */
	ConnectorBase(Component * comp,				
	              const QString & name,
	              const QString & i18nName,
	              const QPoint & pos,	
	              ConnOrientationType orient,
	              ConnDirType dir,
	              const ConnectorInfo * ci);
	
	

protected slots:
	/** Disconnect the wire. */
	void slotDisconnect();
	/** Emits signalDeleteRequest(this).
		*	Is called from RMB popup.
		*/
	void slotDeleteRequest();
	/** Toggles the neg Type */
	void slotToggleNegType();
	/** Display a status help message for popup menu entries, if highlighted. */
	virtual void popupMenuHighlighted(int msg) const;
	
private slots:
	void slotPreReset();


// Property flags
private:
	ConnectorBasePrivate * m_p;
	// The connected Wire. Null if not connected
	Wire * m_wire;
	// True, if connector is a neg type.
	bool m_negType;
	
	KSimAction m_myActions;
	WireProperty * m_wireProperty;
	ImplicitConverter * m_implicitConverter;
	
signals:
	/** This signal is emitted immediately after the connector is connected to a wire.
		*/
	void signalConnect();
	/** This signal is emitted immediately after the connector is disconnected.
		*/
	void signalDisconnect();
	/** This signal is emitted immediately after the connectors wire is changed.
			wire is a pointer to the new wire or null if connetor is disconnected.
		*/
	void signalConnect(Wire * wire);

	/** This signal is emmited immediately after connector a connector property has changed.
			Properties: Hide/Show, Negate, Name
		*/
	void signalProperty();
	
	/** This signal is emitted, if the connector should be deleted.
		* This signal is used internal.
		*/
	void signalDeleteRequest(ConnectorBase *);

	/** This signal is emmited, if the connector name is chanched.*/
	void signalSetName(const QString & newName);

	/** Add menu items depend on connetor properties */
	void signalInitPopupMenu(QPopupMenu * popup);
};


class ConnectorInputBase : public ConnectorBase
{
	Q_OBJECT
	
	/** Checks the connector.
	*   eg. if input is connected.
	*   The implementation returns false if the connector is not connected.
	*	Returns the number of errors.
	*/
	virtual int checkCircuit();

protected:	
	/**
	 * Constructs a input connector. Use this constructor if you derive this class.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 * @param orient    Sets the orientation of the connector.
	 * @param ci        Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorInputBase(Component * comp,
	                   const QString & name,
	                   const QString & i18nName,
	                   const QPoint & pos,
	                   ConnOrientationType orient,
	                   const ConnectorInfo * ci);
	
};


class ConnectorOutputBase : public ConnectorBase
{
	
	Q_OBJECT
		
public:	

protected:	
	/**
	 * Constructs a output connector. Use this constructor if you derive this class.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 * @param orient    Sets the orientation of the connector.
	 * @param ci        Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorOutputBase(Component * comp,
	                    const QString & name,
	                    const QString & i18nName,
	                    const QPoint & pos,
	                    ConnOrientationType orient,
	                    const ConnectorInfo * ci);
};


class ConnectorTristateBase : public ConnectorBase
{
	
	Q_OBJECT
		
public:	

	/** Checks the connector.
	*   eg. if input is connected.
	*   Checks if the WireProperty inherits WirePropertyMultipleOutput.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();

	/** Resets the connector.
	  */
	virtual void reset();
	
	/** Sets the connector output active. */
	void setOutputActive(bool active);
	/** Returns true if the connector output is active. */
	bool isOutputActive() const { return m_outActive; };
	/** Returns true if the wire has one or more active connectors. */
	bool isActive() const;

protected:	
	/**
	 * Constructs a tristate connector. Use this constructor if you derive this class.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 * @param orient    Sets the orientation of the connector.
	 * @param ci        Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorTristateBase(Component * comp,
	                      const QString & name,
	                      const QString & i18nName,
	                      const QPoint & pos,
	                      ConnOrientationType orient,
	                      const ConnectorInfo * ci);
	
private:
	bool m_outActive;
};


class ConnectorList : public QList<ConnectorBase>
{
public:
//	ConnectorList();
//	~ConnectorList();
};

class ConnectorOutputList : public QList<ConnectorOutputBase>
{
public:
//	ConnectorOutputList();
//	~ConnectorOutputList();
};

class ConnectorInputList : public QList<ConnectorInputBase>
{
public:
//	ConnectorOutputList();
//	~ConnectorOutputList();
};

class ConnectorTristateList : public QList<ConnectorTristateBase>
{
public:
//	ConnectorOutputList();
//	~ConnectorOutputList();
};

#define FOR_EACH_CONNECTOR(_it_,_connList_)	\
		for(QListIterator<ConnectorBase> _it_(_connList_);_it_.current();++_it_)

#define FOR_EACH_CONNECTOROUTPUT(_it_,_connList_)	\
		for(QListIterator<ConnectorOutputBase> _it_(_connList_);_it_.current();++_it_)

#define FOR_EACH_CONNECTORINPUT(_it_,_connList_)	\
		for(QListIterator<ConnectorInputBase> _it_(_connList_);_it_.current();++_it_)

#define FOR_EACH_CONNECTORTRISTATE(_it_,_connList_)	\
		for(QListIterator<ConnectorTristateBase> _it_(_connList_);_it_.current();++_it_)

#endif
