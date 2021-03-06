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
#include <qimage.h>
#include <qbitmap.h>

// KDE includes
#include <klocale.h>
#include <kmessagebox.h>

// Project includes
#include "ksimus.h"
#include "ksimdata.h"
#include "resource.h"
#include "component.h"
#include "compcontainer.h"
#include "compview.h"
#include "wire.h"
#include "ksimundo.h"
#include "ksimusdoc.h"
#include "connectorinfo.h"
#include "connectorbase.h"
#include "simulationexecute.h"
#include "wireproperty.h"
#include "wirepropertyinfo.h"
#include "wirepropertymultipleoutput.h"
#include "watchwidget.h"
#include "library.h"
#include "enumdict.h"

#include "connectorpropertywidget.h"
#include "ksimdebug.h"

#include "implicitconverterlibrary.h"

static const char * const sName     = "Name";
static const char * const sNegType  = "Neg";
static const char * const sHideType = "Hide";


#define HIDDEN_TYPE_ENA	  0x0001
#define HIDDEN_TYPE_INIT  0x0002
#define HIDDEN_TYPE_CURR  0x0004

#define NEG_TYPE_ENA      0x0008
#define NEG_TYPE_INIT     0x0010

#define ERASABLE          0x0020


class ConnectorBase::ConnectorBasePrivate
{
public:	
	ConnectorBasePrivate()
	:	flag(0),
		initName(),
		name(),
		wireName(),
		connInfo(0),
		connectorPos(),
		orientation(CO_UNDEF),
		direction(CD_UNDEF),
		idDisconnect(0),
		idErase(0),
		idNegate(0)
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
	// of a component
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
	int idDisconnect;  // TODO: make static ????
	int idErase;
	int idNegate;
};



// Helper class
// Creates immediatly a QBitmap from a XPM (used in static initializer)
class ConnectorBase::XpmBitmap : public QBitmap
{
public:
	XpmBitmap(const char * const xpm[])
	{
		*static_cast<QBitmap *>(this) = QImage(xpm);
		setMask(*this); // selfMask 

	}
};



KSIMDEBUG_INSTANCE_COUNTER(ConnectorBase);

ConnectorBase::ConnectorBase(Component * comp, const QString & name, const QString & i18nName,
                             const QPoint & pos, ConnOrientationType orient,
                             ConnDirType dir, const ConnectorInfo * ci)
	:	QObject(comp, name.latin1()),
		ComponentItem(comp),
		m_wire(0),
		m_negType(false),
		m_myActions(KSimAction::ALL),
		m_wireProperty(0),
		m_implicitConverter((ImplicitConverter *)0)
{
	m_p = new ConnectorBasePrivate();
	Q_CHECK_PTR(m_p);
	m_p->initName = i18nName;
	m_p->wireName = name;      // Not translated
	m_p->connInfo = ci;
	m_p->connectorPos = pos;
	m_p->orientation = orient;
	m_p->direction = dir;
	
	connect(comp->getDoc(), SIGNAL(signalPreReset()), SLOT(slotPreReset()));

	comp->addConnector(this);
	KSIMDEBUG_INSTANCE_INC(ConnectorBase);
}

ConnectorBase::~ConnectorBase()
{
	delete m_p;
	KSIMDEBUG_INSTANCE_DEC(ConnectorBase);
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

/** Returns the connected name componentName::connectorName. */
QString ConnectorBase::getFullName() const
{
	return getComponent()->getName() + QString::fromLatin1("::") + getName();
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
		return (const char *)0;
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
bool ConnectorBase::isTristate() const
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
	
/** Returns true if the connector is connected to a wire. */
bool ConnectorBase::isConnected() const
{
	return getWire() != 0;
}


/** Returns the @ref WireProperty of the wire to where the connector is connected.
  * Returns zero if the connector is not connected. Is valid after proceeding all
  * @ref checkCircuit functions. */
WireProperty * ConnectorBase::getWireProperty() const
{
	return m_wireProperty;
}

/** Sets the @ref WireProperty of the wire to where the connector is connected.
  * In most cases this function is without any interest. */
void ConnectorBase::setWireProperty(WireProperty * wireProperty)
{
//	KSIMDEBUG_VAR("ConnectorBase::setWireProperty", getFullName());
	m_wireProperty = wireProperty;
	
	if (m_implicitConverter)
	{
		delete m_implicitConverter;
		m_implicitConverter = 0;
	}

	if (m_wireProperty)
	{
		// ==> New wire property
		if(m_wireProperty->getInfo()->getDataType() != getConnInfo()->getDataType())
		{
			// ==> different data types
			if(!isInput())
			{
				// This should not be happend if output or tristate connector
				KSIMDEBUG("Different dataTypes");
				KSIMDEBUG_VAR("",m_wireProperty->getInfo()->getDataType());
				KSIMDEBUG_VAR("",getConnInfo()->getDataType());
				Q_ASSERT(0);
			}
			const ImplicitConverterInfo * ici = g_library->getImplicitConverterLib()
			     ->findDataType(m_wireProperty->getInfo()->getDataType(), getConnInfo()->getDataType());
			if (!ici)
			{
				// This should not be happend if output or tristate connector
				KSIMDEBUG("Impicit Convert not found");
				KSIMDEBUG_VAR("",m_wireProperty->getInfo()->getDataType());
				KSIMDEBUG_VAR("",getConnInfo()->getDataType());
				Q_ASSERT(0);
			}
			m_implicitConverter = ici->create(this);
			Q_CHECK_PTR(m_implicitConverter);
			if (!m_implicitConverter)
			{
				// This should not be happend if output or tristate connector
				KSIMDEBUG("Impicit Convert not found");
				KSIMDEBUG_VAR("",m_wireProperty->getInfo()->getDataType());
				KSIMDEBUG_VAR("",getConnInfo()->getDataType());
				Q_ASSERT(0);
			}
		}
	}
}

/** Adds the component in the list for execute next cycle. */
void ConnectorBase::executeComponentNext()
{
	getDoc()->getExecute().executeComponentNext(getComponent());
}

/** Adds the @ref WireProperty in the list for execute next cycle. */
void ConnectorBase::executeWirePropertyNext()
{
	if (getWireProperty())
	{
		getDoc()->getExecute().executeWirePropertyNext(getWireProperty());
	}
}

WatchItemBase * ConnectorBase::makeWatchItem()
{
	KMessageBox::sorry( getDoc()->getApp(),
	                    i18n("The watch functionality is not implementated yet.\n"
	                         "Connector Type: %1")
	                         .arg(this->getConnInfo()->getName()));
	return (WatchItemBase *)0;
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
		m_p->idErase = popup->insertItem(i18n("D&elete Connector"),this, SLOT(slotDeleteRequest()));
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
	
	if (isNegateEnabled())
	{
		m_p->idNegate = popup->insertItem(i18n("&Negate connector"), this,SLOT(slotToggleNegType()));
		popup->setItemChecked(m_p->idNegate, isNegated());
	}
	else
	{
		m_p->idNegate = 0;
	}

	connect(popup, SIGNAL(highlighted(int)), SLOT(popupMenuHighlighted(int)));

	emit signalInitPopupMenu(popup);
	
	return true;
}	

/** Creates the property widget */
PropertyWidget* ConnectorBase::propertyWidget(QWidget * parent)
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
	else if (msg == m_p->idErase)
	{
		getComponent()->statusHelpMsg(i18n("Erase the connector"));
	}
	else if (msg == m_p->idNegate)
	{
		getComponent()->statusHelpMsg(i18n("Negate the boolean connector"));
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

/** Toggles the negated type */
void ConnectorBase::slotToggleNegType()
{
	getComponent()->undoChangeProperty(i18n("Negate connector"));
	setNegate(!isNegated());
	getComponent()->setModified();
	getComponent()->refresh();
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
	
/** Setup the connector for a new circuit execution.
*/
void ConnectorBase::setupCircuit()
{
}

void ConnectorBase::slotPreReset()
{
	setWireProperty((WireProperty *)0);
}


void ConnectorBase::checkProperty(QStringList & /*errorMsg*/)
{
	// Nothing to do yet
}

/** Resets the connector
*/
void ConnectorBase::reset()
{
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

void ConnectorBase::drawLine(QPainter * p, ConnOrientationType orient, int x, int y, 
                             const QColor & color, Qt::PenStyle style, bool wired)
{
	if (Qt::SolidLine == style)
	{
		// use rect
		p->setPen(color);
		p->setBrush(color);
	}
	else
	{
		// use lines
		p->setPen(QPen(color, 1, style));
	}

	switch (orient)
	{
		case CO_TOP:
		{
			x--;
			y += gridY / 2;
			int h = wired ? (-gridY * 3 / 2) : -gridY;
			if (Qt::SolidLine == style)
			{
				p->drawRect(x, y - 1, 2, h);
			}
			else
			{
				h += y;
				p->drawLine(x, y, x, h);
				x++;
				p->drawLine(x, y, x, h);
			}
		}
		break;

		case CO_RIGHT:
		{
			y--;
			x -= gridX / 2;
			int w = wired ? (gridX * 3 / 2) : gridX;
			if (Qt::SolidLine == style)
			{
				p->drawRect(x, y, w, 2);
			}
			else
			{
				w += x;
				p->drawLine(x, y, w, y);
				y++;
				p->drawLine(x, y, w, y);
			}
		}
		break;

		case CO_BOTTOM:
		{
			x--;
			y -= gridY / 2;
			int h = wired ? (gridY * 3 / 2) : gridY;
			if (Qt::SolidLine == style)
			{
				p->drawRect(x, y, 2, h);
			}
			else
			{
				h += y;
				p->drawLine(x, y, x, h);
				x++;
				p->drawLine(x, y, x, h);
			}
		}
		break;

		case CO_LEFT:
		{
			x += gridX / 2;
			y --;
			int w = wired ? -(gridX * 3 / 2) : -gridX;
			if (Qt::SolidLine == style)
			{
				p->drawRect(x - 1, y, w, 2);
			}
			else
			{
				w += x;
				p->drawLine(x, y, w, y);
				y++;
				p->drawLine(x, y, w, y);
			}
		}
		break;

		default:
			KSIMDEBUG_VAR("Unknown orientation", (int)orient);
			break;
	}
}



void ConnectorBase::drawCircle(QPainter * p, ConnOrientationType orient,
                               int x, int y, const QColor & color, bool inner)
{
	// The "outter" bubble
	static const char * const xpmOutter[] =
	{
		"6 6 2 1",
		" 	c None",
		".	c #000000",
		" .... ",
		"......",
		"......",
		"......",
		"......",
		" .... "
	};
	static ConnectorBase::XpmBitmap mapOutter(xpmOutter);

	// The "inner" bubble
	static const char * const xpmInner[] =
	{
		"6 6 2 1",
		" 	c None",
		".	c #000000",
		"  ..  ",
		"  ..  ",
		"..  ..",
		"..  ..",
		"  ..  ",
		"  ..  "
	};
	static ConnectorBase::XpmBitmap mapInner(xpmInner);

	p->setPen(color);
	switch (orient)
	{
		case CO_TOP:
			x += - gridX/2 + 1;
			y += - gridY/2 + 2;
			break;

		case CO_RIGHT:
			x += - gridX/2;
			y += - gridY/2 + 1;
			break;

		case CO_BOTTOM:
			x += - gridX/2 + 1;
			y += - gridY/2;
			break;

		case CO_LEFT:
			x += - gridX/2 + 2;
			y += - gridY/2 + 1;
			break;

		default:
			KSIMDEBUG_VAR("Unknown orientation", (int)orient);
			break;
	}
	p->drawPixmap(x, y, inner ? mapInner : mapOutter);
}

/** Draws the connector on/in the given place and orientation */
void ConnectorBase::draw (QPainter * p, ConnOrientationType orient,  int x, int y) const
{
	p->save();

	const WireColorScheme & colorScheme = getColorScheme();

	if (colorScheme.isDualColor())
	{
		drawLine(p, orient, x, y, colorScheme.getBackgroundColor(), SolidLine, (getWire() != 0));
		drawLine(p, orient, x, y, colorScheme.getForegroundColor(), DotLine, (getWire() != 0));
		if (isNegated())
		{
			drawCircle(p, orient, x, y, colorScheme.getBackgroundColor(), false);
			drawCircle(p, orient, x, y, colorScheme.getForegroundColor(), true);
		}
	}
	else
	{
		drawLine(p, orient, x, y, colorScheme.getColor(), SolidLine, (getWire() != 0));
		if (isNegated())
		{
			drawCircle(p, orient, x, y, colorScheme.getColor(), false);
		}
	}

	p->restore();
}


//**************************************************************************
// *** static funtions
EnumDict<ConnOrientationType>::tData EnumDict<ConnOrientationType>::data[]
      = { {"CO_UNDEF", CO_UNDEF},
          {"CO_TOP",   CO_TOP},
          {"CO_RIGHT", CO_RIGHT},
          {"CO_BOTTOM",CO_BOTTOM},
          {"CO_LEFT",  CO_LEFT},
          {"0",        CO_UNDEF}, // Old KSimus versions <= 0.3.6
          {"1",        CO_TOP},
          {"2",        CO_RIGHT},
          {"3",        CO_BOTTOM},
          {"4",        CO_LEFT},
          {0,         (ConnOrientationType)0}};

static const EnumDict<ConnOrientationType> & getConnOrientationTypeDict()
{
	static EnumDict<ConnOrientationType> connOrientationTypeDict(11);
	return connOrientationTypeDict;
}

const char * ConnectorBase::convertOrientation(ConnOrientationType orientation)
{
	return getConnOrientationTypeDict().find(orientation);
}

ConnOrientationType ConnectorBase::convertOrientation(const char * orientation, ConnOrientationType defaultOrient)
{
	return getConnOrientationTypeDict().find(orientation, defaultOrient);
}

//**************************************************************************	
// *** class ConnectorInputBase ***
	
ConnectorInputBase::ConnectorInputBase(	
	Component * comp, const QString & name, const QString & i18nName,
	const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBase (comp,name,i18nName,pos,orient,CD_INPUT,ci)
{
};

/** Checks the connector
*   eg. if input is connected.
*   The implementation returns false if the connector is not connected.
*   Returns the number of errors
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
		Component * comp, const QString & name, const QString & i18nName,
		const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBase (comp,name,i18nName,pos,orient,CD_OUTPUT,ci)
{
};

//**************************************************************************	
// *** class ConnectorTristateBase ***
	

ConnectorTristateBase::ConnectorTristateBase(	
		Component * comp, const QString & name, const QString & i18nName,
		const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBase (comp,name,i18nName,pos,orient,CD_TRISTATE,ci),
		m_outActive(false)
{
};

void ConnectorTristateBase::reset()
{
	ConnectorBase::reset();
	
	setOutputActive(false);
}

void ConnectorTristateBase::setOutputActive(bool active)
{
	if (m_outActive != active)
	{
		m_outActive = active;
		//executeWirePropertyNext();
	}
}

int ConnectorTristateBase::checkCircuit()
{
	int errors = ConnectorBase::checkCircuit();
	
	if (getWire()
	  && getWire()->getWireProperty()
	  && !getWire()->getWireProperty()->inherits("WirePropertyMultipleOutput"))
	{
		// Not connected
		logError(QString::fromLatin1("Internal Error: WireProperty of wire %1 doesn't inherit WirePropertyMultipleOutput")
		                             .arg(getWire()->getName()));
		errors += 1;
	}
	return errors;
}

bool ConnectorTristateBase::isActive() const
{
	if  (getWire() && getWire()->getWireProperty())
	{
		return ((WirePropertyMultipleOutput*)getWire()->getWireProperty())->getActiveConnectorCount() != 0;
	}
	return false;
}

#include "connectorbase.moc"

