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

class QPainter;
class KSimpleConfig;

class QPopupMenu;
class KSimusDoc;
class Component;
class ConnectorInfo;
class Wire;

class ConnectorBasePrivate;

/**Base class for all connector classes
  *@author Rasmus Diekenbrock
  */


class ConnectorBase : public QObject, public ComponentItem
{
friend class Wire;
friend class Module;
friend class ConnectorPropertyWidget;

	Q_OBJECT
	
public:
	virtual ~ConnectorBase();
	
	//**************************************************************************	
	// *** Data manipulation function ***
	
	/** Returns the related document. */
	KSimusDoc * getDoc() const;
	
	/** Set new connector name.
	  * The name won't be saved in document, if init is true.
	  * The signal signalProperty() is emitted.
	  */
	void setName(const QString & newName, bool init = false);
	/** Returns connector name. */
	const QString & getName() const;
	/** Returns connector init name. */
	const QString & getInitName() const;
	
	/** Set new connector position.
	  * Position is given in grid !!!
	  */
	void setGridPos(const QPoint & position);
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
	/** Compatiblity with old connector class. */
	QPoint getWirePos() const { return getPos(); };
	
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
	bool isTriState() const;
	
	
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
	bool isNegated() const;
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
	virtual bool load(KSimpleConfig & file);
	/** Save properties. */
	virtual void save(KSimpleConfig & file) const;
	
	/** Set the connected wire. Null, if not connected. */
	void setWire(Wire * newWire);
	/** Returns the connected wire. Null, if not connected. */
	Wire * getWire() const;
	
	/** True, if data of the wire is valid. */
	bool isInputDataValid() const;

	/** Add menu items depend on connetor properties.
	  *	Return true, if items are added. */
	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Creates the property widget. */
	virtual QWidget* propertyWidget(QWidget * parent);
	
	
	/** Checks the connector.
	*   eg. if input is connected.
	*   The default implementation does nothing.
	*	Returns the number of errors.
	*/
	
	virtual int checkCircuit();
	/** Checks the component property. The functions is called after the
	*		property dialog.
	*   Eg. all connectors have unique names.
	*   The default implementation does nothing.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);

  /** Resets the connector.
     */
	virtual void reset();
		
	//**************************************************************************	
	// *** Display functions ***
	
	// Setup the colors, brushs, and fills for the connector.
	virtual void setupColorScheme (QPainter * p) const = 0;
	/** Draws the connector. */
	void draw (QPainter * p) const;
	/** Draws the connector on/in the given place and orientation. */
	void draw (QPainter * p, ConnOrientationType orient, int x, int y) const;


protected:
	//**************************************************************************	
	ConnectorBase(	Component * comp,
					const char * name,
					const QPoint & pos,	
					ConnOrientationType orient,
					ConnDirType dir,
					const ConnectorInfo * ci);
					

    /** Returns a generic pointer to the current wire data. */
	const void * getWireData() const;
	
	

protected slots:
	/** Disconnect the wire. */
	void slotDisconnect();
	/** Emits signalDeleteRequest(this).
		*	Is called from RMB popup.
		*/
	void slotDeleteRequest();
	/** Display a status help message for popup menu entries, if highlighted. */
	virtual void popupMenuHighlighted(int msg) const;
	

// Property flags
private:
	ConnectorBasePrivate * m_p;
	// The connected Wire. Null if not connected
	Wire * m_wire;
	// True, if connector is a neg type.
	bool m_negType;
	
	KSimAction m_myActions;

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

};


class ConnectorInputBase : public ConnectorBase
{
friend class Module;
	
	Q_OBJECT
	
	/** Checks the connector.
	*   eg. if input is connected.
	*   The implementation returns false if the connector is not connected.
	*	Returns the number of errors.
	*/
	virtual int checkCircuit();

protected:	
	ConnectorInputBase(	Component * comp,
						const char * name,
						const QPoint & pos,
						ConnOrientationType orient,
						const ConnectorInfo * ci);
					
	/** Returns a pointer to the data that's read from the component.
	  * The default implementation calls the function getWireData().
	  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input. */
	virtual const void * readoutData() const;
	
};


class ConnectorOutputBase : public ConnectorBase
{
friend class Module;
friend class Wire;
	
	Q_OBJECT
		
public:	
	/** Set the data of *this* connector valid / not valid. */
	void setOutputDataValid(bool valid);
	/** True, if data of *this* connector is valid. */
	bool isOutputDataValid() const;

protected:	
	ConnectorOutputBase(	Component * comp,
							const char * name,
							const QPoint & pos,
							ConnOrientationType orient,
							const ConnectorInfo * ci);
					
	/** The function copyData() has to copy data to the output variable.
	  * Reimplementations is required. */
	virtual void copyData(const void * pData) = 0;

    /** Returns a pointer to the data of this output connector. */
	virtual const void * getData() const = 0;

    // True, if data of *this* connector is valid.
    bool dataValid;
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

#define FOR_EACH_CONNECTOR(_it_,_connList_)	\
		for(QListIterator<ConnectorBase> _it_(_connList_);_it_.current();++_it_)

#define FOR_EACH_CONNECTOROUTPUT(_it_,_connList_)	\
		for(QListIterator<ConnectorOutputBase> _it_(_connList_);_it_.current();++_it_)

#define FOR_EACH_CONNECTORINPUT(_it_,_connList_)	\
		for(QListIterator<ConnectorInputBase> _it_(_connList_);_it_.current();++_it_)


#endif
