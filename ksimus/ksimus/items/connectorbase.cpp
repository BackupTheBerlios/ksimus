/***************************************************************************
                          connectorbase.cpp  -  description
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

// QT includes
#include <qpainter.h>
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "ksimdata.h"
#include "resource.h"
#include "component.h"
#include "compcontainer.h"
#include "compview.h"
#include "wire.h"
#include "ksimundo.h"
#include "connectorinfo.h"
#include "connectorbase.h"

#include "connectorpropertywidget.h"
#include "ksimdebug.h"

static const char * sName = "Name";
static const char * sNegType = "Neg";
static const char * sHideType = "Hide";


#define HIDDEN_TYPE_ENA		0x0001
#define HIDDEN_TYPE_INIT	0x0002
#define HIDDEN_TYPE_CURR	0x0004

#define NEG_TYPE_ENA			0x0008
#define NEG_TYPE_INIT			0x0010

#define ERASABLE					0x0020


class ConnectorBasePrivate
{
public:	
	ConnectorBasePrivate()
	:	flag(0/*HIDDEN_TYPE_ENA*/),
		initName(),
		name(),
		wireName(),
		connInfo(0),
		connectorPos(),
		orientation(CO_UNDEF),
		direction(CD_UNDEF),
		idDisconnect(0),
		idErase(0)
	{};
	
	~ConnectorBasePrivate() {};
	
	// Flag manipulation
	void setFlag (int val) { flag |= val; };
	void resetFlag (int val) { flag &= ~val; };
	void setFlag (bool v, int val) { v?setFlag(val):resetFlag(val); };
	bool isFlagSet (int val) const { return (flag & val); };
	
	// Hidden flags
	void setHiddenEna(bool v)    { setFlag(v,HIDDEN_TYPE_ENA); };
	bool isHiddenEna() const     { return isFlagSet(HIDDEN_TYPE_ENA); };
	
	void setHiddenInit(bool v)   { setFlag(v,HIDDEN_TYPE_INIT); };
	bool isHiddenInit() const    { return isFlagSet(HIDDEN_TYPE_INIT); };
	
	void setHiddenCurrent(bool v){ setFlag(v,HIDDEN_TYPE_CURR); };
	bool isHiddenCurrent() const { return isFlagSet(HIDDEN_TYPE_CURR); };
	
	// Negate flags
	void setNegEna(bool v)    { setFlag(v,NEG_TYPE_ENA); };
	bool isNegEna() const     { return isFlagSet(NEG_TYPE_ENA); };
	
	void setNegInit(bool v)   { setFlag(v,NEG_TYPE_INIT); };
	bool isNegInit() const    { return isFlagSet(NEG_TYPE_INIT); };
	
	// Erase flag
	void setErasable(bool v)	{ setFlag(v,ERASABLE); };
	bool isErasable() const 	{ return isFlagSet(ERASABLE); };
	
	Q_UINT32 flag;


	// Initial name of the connector
	QString initName;
	
	// Current Name of the connector (is empty/null, if initName is used)
	QString name;
	
	// Name of the connector which is used by the wires to find the correct connector
	// on a component
	QString wireName;
	
	// Information class of the connetor
	const ConnectorInfo * connInfo;
	
	// Position off connector relative to component
	QPoint connectorPos;
	
	// Orientation of the connector
	ConnOrientationType orientation;

	// Data dircetion
	ConnDirType direction;
	
	// ID menu entry
	int idDisconnect;
	int idErase;
};



ConnectorBase::ConnectorBase(Component * comp, const QString & name,/* const QString & wireName,*/
                             const QPoint & pos, ConnOrientationType orient,
                             ConnDirType dir, const ConnectorInfo * ci)
	:	QObject(comp, name),
		ComponentItem(comp),
		m_wire(0),
		m_negType(false),
		m_myActions(KSimAction::ALL)
{
	m_p = new ConnectorBasePrivate();
	CHECK_PTR(m_p);
	m_p->initName = i18n(name.latin1());
	m_p->wireName = name;		// Not translated
	m_p->connInfo = ci;
	m_p->connectorPos = pos;
	m_p->orientation = orient;
	m_p->direction = dir;
	
	comp->addConnector(this);
}

ConnectorBase::~ConnectorBase()
{
	delete m_p;
}

/** Returns the related document */
KSimusDoc * ConnectorBase::getDoc() const
{
	return getComponent()->getDoc();	
}
	

//**************************************************************************	
// *** Data manipulation function ***
	

void ConnectorBase::setWireName(const QString & name)
{
	m_p->wireName = name;
}

QString ConnectorBase::getWireName() const
{
	return m_p->wireName;
}


/** Set new connector name
  * The name won't be saved, if init is true
  */
void ConnectorBase::setName(const QString & newName, bool init)
{
	QString nameSimp(newName.simplifyWhiteSpace());
	bool changed(false);
	
	if(getName() != nameSimp)
	{
		changed = true;
	}
	
	// newName == initName
	if (nameSimp == m_p->initName)
	{
		m_p->name = QString();	// Null string
	}
	// Init
	else if (init)
	{
		m_p->initName = nameSimp;
		m_p->name = QString();	// Null string
	}
	else
	{
		m_p->name = nameSimp;
	}
	if (changed)
	{
		emit signalProperty();
		emit signalSetName(getName());
	}
}

/** Returns connector name */
const QString & ConnectorBase::getName() const
{
	if (m_p->name.isEmpty())
	{
		return m_p->initName;
	}
	else
	{
		return m_p->name;
	}
}

/** Returns connector init name */
const QString & ConnectorBase::getInitName() const
{
	return m_p->initName;
}

/** Set new connector position
  * Position is given in grid !!!
  */
void ConnectorBase::setGridPos(const QPoint & position)
{
	m_p->connectorPos = position;
}

/** Set new connector position
  * Position is given in grid !!!
  */
void ConnectorBase::setGridPos(int x, int y)
{
	setGridPos(QPoint(x,y));
}

/** Returns the current relative connector position
  * Position is given in grid !!!
  */	
const QPoint & ConnectorBase::getGridPos() const
{
	return m_p->connectorPos;
}
	
/** Returns the current relative connector position */	
QPoint ConnectorBase::getRelPos() const
{
	return QPoint ( m_p->connectorPos.x()*gridX+gridX/2, m_p->connectorPos.y()*gridY+gridY/2);
}

/** Returns the current absolute connector position */	
QPoint ConnectorBase::getPos() const
{
	return (getRelPos() + getComponent()->getSheetView()->getPos());
}

QPoint ConnectorBase::getWirePos() const
{
	CompView * cv = getComponent()->getSheetView();
	QPoint pos(getRelPos() + cv->getPos());

	switch (getOrientation())
	{
		case CO_LEFT:
			pos.rx() -= gridX;
			break;
			
		case CO_TOP:
			pos.ry() -= gridY;
			break;
			
		case CO_RIGHT:
			pos.rx() += gridX;
			break;
			
		case CO_BOTTOM:
			pos.ry() += gridY;
			break;
		
		default:
			KSIMDEBUG_VAR("Bad orientation",getOrientation());
			break;
	}
	return cv->mapToRotation(pos);
}

QPoint ConnectorBase::getWireGridPos() const
{
	QPoint p(getWirePos());
	return QPoint(p.x()/gridX, p.y()/gridY);
}
			

/** Returns true, if the given positon hit the connector */
eHitType ConnectorBase::isHit(int x, int y) const
{
	QPoint pos(getPos());
	
	if ( isHidden()
		|| (x <= pos.x()-gridX/2)
		|| (x >= pos.x()+gridX/2)
		|| (y <= pos.y()-gridY/2)
		|| (y >= pos.y()+gridY/2))
	{
		return NO_HIT;
	}
	else
	{
		return CONNECTOR_HIT;
	}
}


/** Returns the connector information object */
const ConnectorInfo * ConnectorBase::getConnInfo() const
{
	return m_p->connInfo;
}

/** Returns the connector type */
const char * ConnectorBase::getConnType() const
{
	if (getConnInfo())
		return getConnInfo()->getLibName();
	else
		return 0;
}

/** Set the orientation of the connector */
void ConnectorBase::setOrientation(ConnOrientationType orient)
{
	m_p->orientation = orient;
}

/** Returns the orientation of the connector */
ConnOrientationType ConnectorBase::getOrientation() const
{
	return m_p->orientation;
}
	
/** True, if connector is a input */
bool ConnectorBase::isInput() const
{
	return (m_p->direction == CD_INPUT);
}
	
/** True, if connector is a output */
bool ConnectorBase::isOutput() const
{
	return (m_p->direction == CD_OUTPUT);
}
	
/** True, if connector is a tris state connector */
bool ConnectorBase::isTriState() const
{
	return (m_p->direction == CD_TRISTATE);
}
	
// ### Hide functions
/** Enables the hide function of the connector */
void ConnectorBase::setHideEnabled(bool ena)
{
	m_p->setHiddenEna(ena);
}
/** Enables the hide function of the connector */
bool ConnectorBase::isHideEnabled() const
{
	return m_p->isHiddenEna();
}

/** Set the connector as hidden
  * The change won't be saved, if init is true
  */
void ConnectorBase::setHide(bool hide, bool init)
{
	if(isHidden() != hide)
	{
		m_p->setHiddenCurrent(hide);
		emit signalProperty();
	}
		
	if (init)
	{
		m_p->setHiddenInit(hide);
	}
}

/** Returns true, if connector is hidden */
bool ConnectorBase::isHidden() const
{
	return m_p->isHiddenCurrent();
}

/** Returns true, if connector is hidden at init time */
bool ConnectorBase::isInitHidden() const
{
	return m_p->isHiddenInit();
}

/** Enables the negate function of the connector */
void ConnectorBase::setNegateEnabled(bool ena)
{
	m_p->setNegEna(ena);
}
/** Enables the negate function of the connector */
bool ConnectorBase::isNegateEnabled() const
{
	return m_p->isNegEna();
}

/** Set the connector as negated type
  * The negType won't be saved, if init is true
  */
void ConnectorBase::setNegate(bool neg, bool init)
{
	if(isNegated() != neg)
	{
		m_negType = neg;
		emit signalProperty();
	}
	if (init)
	{
		m_p->setNegInit(neg);
	}
}

/** Returns true, if connector is negated */
bool ConnectorBase::isNegated() const
{
	return m_negType;
}

/** Returns true, if connector is negated at init time */
bool ConnectorBase::isInitNegate() const
{
	return m_p->isNegInit();
}

void ConnectorBase::setErasable(bool erasable)
{
	m_p->setErasable(erasable);
}

bool ConnectorBase::isErasable() const
{
	return m_p->isErasable();
}
	

/** Set the connected wire. Null if not connected */
void ConnectorBase::setWire(Wire * newWire)
{
	m_wire = newWire;
	
	emit signalConnect(newWire);
	if (newWire)
	{
		emit signalConnect();
	}
	else
	{
		emit signalDisconnect();
	}
}

/** Returns the connected wire. Null if not connected */
Wire * ConnectorBase::getWire() const
{
	return m_wire;
}

/** True, if data of the wire is valid */
bool ConnectorBase::isInputDataValid() const
{
	if (getWire())
	{
		return (getWire()->getCurrentData() != 0);
	}
	return false;
}

/** Returns a generic pointer to the current wire data
  * Returns null, if no wire connected or no valid data
  */
const void * ConnectorBase::getWireData() const
{
//	if (isInputDataValid())
	if (getWire())
	{
		return getWire()->getCurrentData();
	}
	else
	{
		return 0;
	}
}

/** Load properties
	Returns true if successful */
bool ConnectorBase::load(KSimData & file)
{
	setName(file.readEntry(sName, getInitName()));
	setNegate(file.readBoolEntry(sNegType, isInitNegate()));
	setHide(file.readBoolEntry(sHideType, isInitHidden()));
	return true;
}
	
/** Save properties */
void ConnectorBase::save(KSimData & file) const
{
	// save only if set
	if (!m_p->name.isEmpty())
	{
		file.writeEntry(sName, getName());
	}
	if (isNegated() != isInitNegate())
	{
		file.writeEntry(sNegType, isNegated());
	}
	if (isHidden() != isInitHidden())
	{
		file.writeEntry(sHideType, isHidden());
	}
}

/** Add menu items depend on connetor properties */
bool ConnectorBase::initPopupMenu(QPopupMenu * popup)
{
	popup->insertSeparator();
	m_p->idDisconnect = popup->insertItem(i18n("&Disconnect wire"),this, SLOT(slotDisconnect()));
	// exist no connection
	if (!getWire())
	{
		popup->setItemEnabled(m_p->idDisconnect, false);
	}

	if (isErasable())
	{
		m_p->idErase = popup->insertItem(i18n("&Erase Connector"),this, SLOT(slotDeleteRequest()));
		// exist connection
		if (getWire())
		{
			popup->setItemEnabled(m_p->idErase, false);
		}
	}
	else
	{
		m_p->idErase = 0;
	}
		
	connect(popup, SIGNAL(highlighted(int)), SLOT(popupMenuHighlighted(int)));

	emit signalInitPopupMenu(popup);
	
	return true;
}	

/** Creates the property widget */
QWidget* ConnectorBase::propertyWidget(QWidget * parent)
{
	return new ConnectorPropertyWidget(this, parent, getName());
}

/** Display a help for the popup menu */
void ConnectorBase::popupMenuHighlighted(int msg) const
{
	if (msg == m_p->idDisconnect)
	{
		getComponent()->statusHelpMsg(i18n("Disconnect the wire"));
	}

	if (msg == m_p->idErase)
	{
		getComponent()->statusHelpMsg(i18n("Erase connector"));
	}
}


/** disconnect the wire */
void ConnectorBase::slotDisconnect()
{
	// do nothing, if not connected */
	if (getWire())
	{
		getComponent()->getUndo()->begin(i18n("Disconnect wire"));
		getComponent()->getContainer()->delConnection(this);
		getComponent()->getUndo()->end();
		getComponent()->setModified();
		getComponent()->refresh();
	}
}
		
void ConnectorBase::slotDeleteRequest()
{
	emit signalDeleteRequest(this);
}

/** Checks the connector
*   eg. if input is connected.
*   The default implementation does nothing.
*	Returns the number of errors
*/
int ConnectorBase::checkCircuit()
{
	// Nothing to check yet!
	return 0;
}
	
void ConnectorBase::checkProperty(QStringList & /*errorMsg*/)
{
	// Nothing todo yet
}

/** Resets the connector
*/
void ConnectorBase::reset()
{
	// Nothing to do yet!
}

	
//**************************************************************************	
// *** Display functions ***
	
/** Draws the connector */
void ConnectorBase::draw (QPainter * p) const
{
	if (!isHidden())
	{
		QPoint pos(getRelPos());
		draw(p, getOrientation(), pos.x(), pos.y());
	}
}

/** Draws the connector on/in the given place and orientation */
void ConnectorBase::draw (QPainter * p, ConnOrientationType orient,  int x, int y) const
{
	p->save();
	
	setupColorScheme(p);
	
	switch (orient)
	{
		case CO_TOP:
		{
			if (isNegated())
			{
				p->drawEllipse(x-3, y-2, 6 , 6);
			}
			if (getWire()) //wired?
			{
				p->drawLine(x, y-gridY, x, y+gridY/2);
			}
			else
			{
				p->drawLine(x, y-gridY/2, x, y+gridY/2);
			}
		}
		break;
			
		case CO_RIGHT:
		{
			if (isNegated())
			{
				p->drawEllipse(x-gridX/2, y-3, 6 , 6);
			}
			if (getWire()) //wired?
			{
				p->drawLine(x-gridX/2, y, x+gridX, y);
			}
			else
			{
				p->drawLine(x-gridX/2, y, x+gridX/2, y);
			}
		}
		break;
		
		case CO_BOTTOM:
		{
			if (isNegated())
			{
				p->drawEllipse(x-3, y-4, 6 , 6);
			}
			if (getWire()) //wired?
			{
				p->drawLine(x, y-gridY/2, x, y+gridY);
			}
			else
			{
				p->drawLine(x, y-gridY/2, x, y+gridY/2);
			}
		}
		break;
		
		case CO_LEFT:
		{
			if (isNegated())
			{
				p->drawEllipse(x-2, y-3, 6 , 6);
			}
			if (getWire()) //wired?
			{
				p->drawLine(x-gridX, y, x+gridX/2, y);
			}
			else
			{
				p->drawLine(x-gridX/2, y, x+gridX/2, y);
			}
		}
		break;

		default:
			KSIMDEBUG_VAR("Unknown orientation", (int)orient)
			break;
	}
	p->restore();
}


//**************************************************************************	
// *** class ConnectorInputBase ***
	
ConnectorInputBase::ConnectorInputBase(	
	Component * comp, const QString & name, const QPoint & pos,
	ConnOrientationType orient,	const ConnectorInfo * ci)
	:	ConnectorBase (comp,name,pos,orient,CD_INPUT,ci)
{
};

/** Returns a pointer to the data that's read from the component
  * The default implementation calls the function getWireData()
  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
const void * ConnectorInputBase::readoutData() const
{
	return getWireData();
}

/** Checks the connector
*   eg. if input is connected.
*   The implementation returns false if the connector is not connected.
*	Returns the number of errors
*/
int ConnectorInputBase::checkCircuit()
{
	int errors = ConnectorBase::checkCircuit();
	
	if (!getWire() && !isHidden())
	{
		// Not connected
		logWarning(i18n("%1 is not connected").arg(getName()));
//		errors += 1  WARNING!;
	}
	return errors;
}

//**************************************************************************	
// *** class ConnectorOutputBase ***
	

ConnectorOutputBase::ConnectorOutputBase(	
	Component * comp, const QString & name, const QPoint & pos,
	ConnOrientationType orient,	const ConnectorInfo * ci)
	:	ConnectorBase (comp,name,pos,orient,CD_OUTPUT,ci),
		dataValid(true)
{
};

/** Set the data of *this* connector valid / not valid */
void ConnectorOutputBase::setOutputDataValid(bool valid)
{
	dataValid = valid;
}

/** True, if data of *this* connector is valid */
bool ConnectorOutputBase::isOutputDataValid() const
{
	return dataValid;
}



