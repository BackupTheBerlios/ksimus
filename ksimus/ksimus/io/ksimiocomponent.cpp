/***************************************************************************
                          ksimiocomponent.cpp  -  description
                             -------------------
    begin                : Sun Apr 6 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

// C/C++ includes

// QT includes
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qstringlist.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "ksimiocomponent.h"
#include "resource.h"
#include "ksimdebug.h"
#include "ksimiopinselectiondialog.h"
#include "ksimiopin.h"
#include "ksimiojoin.h"
#include "ksimiojoininfo.h"
#include "ksimiojoinlibrary.h"
#include "connectorbase.h"
#include "ksimdata.h"
#include "library.h"

// Forward declaration

Component * KSimIoComponent::create(CompContainer * container, const ComponentInfo * ci)
{
	return new KSimIoComponent(container, ci);
}

const ComponentInfo * KSimIoComponent::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "IO Component"),
	                                QString::fromLatin1("IO/Component"),
	                                i18n("Component", "IO/Component"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-and")*/);
	return &Info;
}


//###############################################################
//###############################################################


class KSimIoComponent::Private
{



public:
	Private()
	:	lastSerial(0)
	{};

	KSimIoJoin::List joinList;
	KSimIoJoin::List activeJoinList;
	unsigned int lastSerial;

	// Some statics
public:
	static const char * const sJoinCount;
	static const char * const sLastJoinSerial;
	static const char * const sJoinDir;
	static const char * const sJoinType;
	static const char * const sJoinSerial;

};


const char * const KSimIoComponent::Private::sJoinCount      = "Join Count";
const char * const KSimIoComponent::Private::sLastJoinSerial = "Last Join Serial";
const char * const KSimIoComponent::Private::sJoinDir        = "Join %1/";
const char * const KSimIoComponent::Private::sJoinType       = "Type";
const char * const KSimIoComponent::Private::sJoinSerial     = "Serial Number";



KSimIoComponent::KSimIoComponent(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci)
{
	m_p = new Private();
	CHECK_PTR(m_p);

	KSIMDEBUG_VAR("KSimIoComponent::KSimIoComponent", getName());
	// Initializes the sheet view
	if (getSheetMap())
	{
		new KSimIoComponentView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

KSimIoComponent::~KSimIoComponent()
{
	KSIMDEBUG_VAR("KSimIoComponent::~KSimIoComponent", getName());
	// Remove joins
	while(m_p->joinList.count())
	{
		// join removes it self from m_p->joinList !!!
		delete m_p->joinList.getFirst();
	};

	delete m_p;
}


void KSimIoComponent::save(KSimData & file) const
{

	Component::save(file);

	file.writeEntry(Private::sJoinCount, m_p->joinList.count());
	file.writeEntry(Private::sLastJoinSerial, m_p->lastSerial);

	unsigned int i = 1;

	FOR_EACH_IO_JOIN(it, m_p->joinList)
	{
		file.pushGroupRel(QString::fromLatin1(Private::sJoinDir).arg(i));
		file.writeEntry(Private::sJoinType, it.current()->getInfo()->getLibName());
		file.writeEntry(Private::sJoinSerial, it.current()->getSerialID());
		it.current()->save(file);
		file.popGroup();
		i++;
	}
}

bool KSimIoComponent::load(KSimData & file, bool copyLoad)
{
	bool loadOk = Component::load(file, copyLoad);

	if (copyLoad)
	{
	
	}
	else
	{
		// Load joins only if it is not a copy
		
		unsigned int i, joinCount, serial;
		QString joinType;
		KSimIoJoin * join;
		KSimIoJoin::List newJoinList;

		m_p->lastSerial = file.readUnsignedNumEntry(Private::sLastJoinSerial, 0);

		joinCount = file.readUnsignedNumEntry(Private::sJoinCount, 0);

		for (i = 1; i <= joinCount; i++)
		{
			file.pushGroupRel(QString::fromLatin1(Private::sJoinDir).arg(i));

			joinType = file.readEntry(Private::sJoinType, QString::null);
			ASSERT(joinType != QString::null);
			serial = file.readUnsignedNumEntry(Private::sJoinSerial, 0);
			ASSERT(serial != 0);

			// Join exists ?
			join = m_p->joinList.findBySerial(serial);
			if (join)
			{
				//Exists
				if (joinType == join->getInfo()->getLibName())
				{
					// Correct type
					m_p->joinList.findRef(join);
					m_p->joinList.take();      // Remove from list
					newJoinList.append(join);  // Add to new list
				}
				else
				{
					KSIMDEBUG(QString::fromLatin1("Type mismatch: Loading type %1, Existing type %2")
				          .arg(joinType).arg(join->getInfo()->getLibName()));
				  // Join will be removed !!
					loadOk = false;
				}
			}
			else
			{
				//Exists not! Create it
				bool res = g_library->getIoJoinLib()->create(&join, joinType, this);
				if (res)
				{
					newJoinList.append(join);  // Add to new list
					join->setSerialID(serial);
					join->createConnector();
				}
				else
				{
					KSIMDEBUG(QString::fromLatin1("Creation of join type %1 failed").arg(joinType));
					loadOk = false;
				}
			}

			// Exists join now?
			if (join)
			{
				// Load join properties
				loadOk &= join->load(file);
			}
			file.popGroup();
		}

		// Remove not longer used joins
		while(m_p->joinList.count())
		{
			// join removes it self from m_p->joinList !!!
			delete m_p->joinList.getFirst();
		};

		// Set new join list
		m_p->joinList = newJoinList;
	}

	updateLayout();

	return loadOk;
}

int KSimIoComponent::checkCircuit()
{
	int errors = Component::checkCircuit();
	QStringList unknwonDevices;

	m_p->activeJoinList.clear();
	FOR_EACH_IO_JOIN(it,m_p->joinList)
	{
		if (it.current()->getDevice() == 0)
		{
			if (!unknwonDevices.contains(it.current()->getDeviceName()))
			{
				unknwonDevices.append(it.current()->getDeviceName());
			}
		}
		else
		{
			m_p->activeJoinList.append(it.current());
		}
	}
	for(QStringList::ConstIterator it = unknwonDevices.begin(); it != unknwonDevices.end(); ++it)
	{
		// Waring only
		logWarning(i18n("IO Component", "IO Component uses unknown device '%1'.").arg(*it));
	}

	return errors;
}

/** Reset all simulation variables */
void KSimIoComponent::reset()
{
	Component::reset();
	FOR_EACH_IO_JOIN(it, m_p->activeJoinList)
	{
		it.current()->reset();
	}
}

/** Executes the simulation of this component */
void KSimIoComponent::calculate()
{
	Component::calculate();
	FOR_EACH_IO_JOIN(it, m_p->activeJoinList)
	{
		it.current()->calculate();
	}
	executeNext();
}




/** Initialize the component popup menu */
bool KSimIoComponent::initPopupMenu(QPopupMenu * popup)
{
	bool insert = Component::initPopupMenu(popup);
	if (!insert)
		popup->insertSeparator();
	popup->insertItem(i18n("&Insert IO Pin"), this, SLOT(slotPinSelection()));
	return true;
}

void KSimIoComponent::removeJoin(KSimIoJoin * delJoin)
{
	ASSERT(delJoin != (KSimIoJoin *)0);
	
	if (!m_p->joinList.removeRef(delJoin))
	{
		KSIMDEBUG(QString::fromLatin1("IoComponent %1: Join %2 not in join list!")
		          .arg(getName())
		          .arg(delJoin->getPin()->getName()));
	}
}



void KSimIoComponent::slotPinSelection()
{
	const KSimIoPin * pin = KSimIoPinSelectionDialog::execute(i18n("Select IO Pin")/*,
                                                            QWidget *parent,
                                                            const char *name)*/);

	if (pin && pin->getSelectedJoinInfo())
	{
		undoChangeProperty(i18n("IO Component", "Add connector"));
		setModified();

		const KSimIoJoinInfo * info = pin->getSelectedJoinInfo();
		KSimIoJoin * join = info->create(this, info);
		CHECK_PTR(join);
		join->setSerialID(getNextSerial());
		join->setPin(pin);
		join->createConnector();
		m_p->joinList.append(join);
		updateLayout();
	}
}


void KSimIoComponent::updateLayout()
{
	if(getSheetView())
	{
		((KSimIoComponentView*)getSheetView())->updateLayout();
	}
}

unsigned int KSimIoComponent::getNextSerial()
{
	bool found;

	do
	{
		m_p->lastSerial++;
		found = false;

		// Serial in use?
		FOR_EACH_IO_JOIN(it, m_p->joinList)
		{
			if (it.current()->getSerialID() == m_p->lastSerial)
			{
				found = true;
				break;
			}
		}
	}
	while (found || (m_p->lastSerial == 0));

	return m_p->lastSerial;
}

	
//###############################################################
//###############################################################


KSimIoComponentView::KSimIoComponentView(KSimIoComponent * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 3*gridX, 3*gridY));
	enableRotation(true);
//	enableConnectorSpacingTop(false);
//	enableConnectorSpacingRight(false);
//	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);

	updateLayout();
}

void KSimIoComponentView::draw(QPainter * p)
{
	CompView::draw(p);

	drawFrame(p);
	p->drawText(getDrawingPlace(), AlignCenter, QString::fromLatin1("IO"));
}

void KSimIoComponentView::updateLayout()
{
	unsigned int top    = 0;
	unsigned int right  = 0;
	unsigned int bottom = 0;
	unsigned int left   = 0;


	FOR_EACH_CONNECTOR(it, *getComponent()->getConnList())
	{
		switch(it.current()->getOrientation())
		{
			case CO_UNDEF:
				KSIMDEBUG(QString::fromLatin1("IO-Conn %1 %2 Orientation undefined")
				          .arg(getComponent()->getName())
				          .arg(it.current()->getName()));
				break;

			case CO_TOP:
				top ++;
				break;

			case CO_RIGHT:
				right ++;
				break;

			case CO_BOTTOM:
				bottom ++;
				break;

			case CO_LEFT:
				left ++;
				break;
		}
	}

	unsigned int width  = QMAX(QMAX(top, bottom) * 2 + 1, 3);
	unsigned int height = QMAX(QMAX(right, left) * 2 + 1, 3);
	unsigned int topPos    = 1;
	unsigned int rightPos  = 1;
	unsigned int bottomPos = 1;
	unsigned int leftPos   = 1;

	if (top != 0)
	{
		height ++;
		leftPos ++;
		rightPos ++;
		enableConnectorSpacingTop(true);
	}
	else
	{
		enableConnectorSpacingTop(false);
	}

	if (right != 0)
	{
		width ++;
		enableConnectorSpacingRight(true);
	}
	else
	{
		enableConnectorSpacingRight(false);
	}

	if (bottom != 0)
	{
		height ++;
		enableConnectorSpacingBottom(true);
	}
	else
	{
		enableConnectorSpacingBottom(false);
	}

	if (left != 0)
	{
		width ++;
		topPos ++;
		bottomPos ++;
		enableConnectorSpacingLeft(true);
	}
	else
	{
		enableConnectorSpacingLeft(false);
	}

	FOR_EACH_CONNECTOR(it, *getComponent()->getConnList())
	{
		switch(it.current()->getOrientation())
		{
			case CO_UNDEF:
				break;

			case CO_TOP:
				it.current()->setGridPos(topPos, 0);
				topPos += 2;
				break;

			case CO_RIGHT:
				it.current()->setGridPos(width-1, rightPos);
				rightPos += 2;
				break;

			case CO_BOTTOM:
				it.current()->setGridPos(bottomPos, height-1);
				bottomPos += 2;
				break;

			case CO_LEFT:
				it.current()->setGridPos(0, leftPos);
				leftPos += 2;
				break;
		}
	}

	QRect place = getPlace();
	place.setWidth(width * gridX);
	place.setHeight(height * gridY);
	setPlace(place, true);

}


