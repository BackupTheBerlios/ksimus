/***************************************************************************
                          externalconnector.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
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

#ifndef EXTERNALCONNECTOR_H
#define EXTERNALCONNECTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "componentpropertygeneralwidget.h"

// Forward declaration
class QCheckBox;
class QGroupBox;

/**Base class for external connectors
	Used for modules
  *@author Rasmus Diekenbrock
  */

//###############################################################

class ExternalConnectorSV : public CompView
{
public:
	ExternalConnectorSV(Component * comp);
	~ExternalConnectorSV();
	virtual void draw(QPainter * p);
};

//###############################################################

class ExternalConnectorMultipleOutputSV : public CompView
{
public:
	ExternalConnectorMultipleOutputSV(Component * comp);
	~ExternalConnectorMultipleOutputSV();
	virtual void draw(QPainter * p);
};

//###############################################################


class ExternalConnector : public Component
{
	Q_OBJECT

public: 
	~ExternalConnector();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Checks the component.
	*   eg. all required inputs are connected.
	*   The default implementation calls the checks functions of the connectors.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Checks the component property. The functions is called after the
	*		property dialog.
	*   Eg. all connectors have unique names.
	*		The implementations checks the name of the externalconnectros (have to be unique) and
	*		calls the default implementation.
	*
	*   Append the message to errorMsg.
	*/
//	virtual void checkProperty(QStringList & errorMsg);
	
	bool isInput() const { return m_flags.input; };
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Creates the general property page for the property dialog.
	  * This function creates a @ref BooleanButtonPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Returns the *external* connector */
	ConnectorBase * getExternalConn() const;
	/** Returns the *internal* connector */
	ConnectorBase * getInternalConn() const;
	/** Returns the *module* connector. The module connector is set by the module if the
	 *  ExternalConnector is part of this module. The connector represent the external
	 *  connector of the module. */
	ConnectorBase * getModuleConn() const;
	/** Returns the used external connector. This is the external connector if not used in a module
	 *  or the module connector if used in a module.  */
	ConnectorBase * getUsedExternalConn() const;

	/** Sets the *external* connector */
	void setExternalConn(ConnectorBase * extConn);
	/** Sets the *internal* connector */
	void setInternalConn(ConnectorBase * inConn);
	/** Sets the *module* connector. The module connector is set by the module if the
	 *  ExternalConnector is part of this module. The connector represent the external
	 *  connector of the module. */
	void setModuleConn(ConnectorBase * modConn);

	/** Returns the relative connector position in pixmap mode.
	 *  The position is measured in gridX / gridY. */
	QPoint getPixmapPos() const { return m_pixmapPos; };
	/** Sets the relative connector position in pixmap mode.
	 *  The position is measured in gridX / gridY. */
	void setPixmapPos(QPoint pos) { m_pixmapPos = pos; };
	/** Returns the orientation of the connector in pixmap mode. */
	ConnOrientationType getPixmapOrientation() const { return m_pixmapOrient; };
	/** Sets the orientation of the connector in pixmap mode.*/
	void setPixmapOrientation(ConnOrientationType orientation);
	
	/** Returns the relative connector position in user view mode.
	 *  The position is measured in gridX / gridY. */
	QPoint getUserViewPos() const { return m_userViewPos; };
	/** Sets the relative connector position in user view mode.
	 *  The position is measured in gridX / gridY. */
	void setUserViewPos(QPoint pos) { m_userViewPos = pos; };
	/** Returns the orientation of the connector in user view mode.*/
	ConnOrientationType getUserViewOrientation() const { return m_userViewOrient; };
	/** Sets the orientation of the connector in user view mode. */
	void setUserViewOrientation(ConnOrientationType orientation);

	/** Sets the recursion lock bit. Use it in the function @ref calculate. */
	void setRecursionLocked(bool locked) { m_flags.recursionLocked = locked; };
	/** Returns the value of the recursion lock bit. Use it in the function @ref calculate. */
	bool isRecursionLocked() const { return m_flags.recursionLocked; };
		
	/** Sets the external connector as optional. The user can hide or unhide the connector. */
	void setOptionalConn(bool ena) { m_flags.optionalConn = ena; };
	/** Returns true, if the external connector is optional. */
	bool isOptionalConn() const { return m_flags.optionalConn; };
	/* Sets the optional connector hidden (ena = false) or unhidden (ena = true). */
	void setOptionalConnEnabled(bool ena) { m_flags.optionalConnEnabled = ena; };
	/** Returns true, if the optional connector is not hidden. */
	bool isOptionalConnEnabled() const { return m_flags.optionalConnEnabled; };


protected:
	/** Creates an external connector.
	  * @param input       True, if the external connector is an input (on the left side of the module)
	  * @param multiOutput True, if the data type of the external connector supports multiple outputs
	  */
	ExternalConnector(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput = false);
	/** Creates an external connector without a view.
	  * You have to set the input type and you have to create a view.
	  */
	ExternalConnector(CompContainer * container, const ComponentInfo * ci);
	
	struct
	{
		unsigned int input               :1;
		unsigned int recursionLocked     :1;
		unsigned int optionalConn        :1;
		unsigned int optionalConnEnabled :1;
		unsigned int useModuleConn       :1;
	} m_flags;
	QPoint m_pixmapPos;
	ConnOrientationType m_pixmapOrient;
	QPoint m_userViewPos;
	ConnOrientationType m_userViewOrient;
	ConnectorBase * m_internalConn;
	ConnectorBase * m_externalConn;
	ConnectorBase * m_moduleConn;
	
private:
	void init();
	
};


//###############################################################

class ExternalConnectorPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	ExternalConnectorPropertyGeneralWidget(ExternalConnector * comp, QWidget *parent=0, const char *name=0);
//	~ExternalConnectorPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ExternalConnector * getExtConn() { return (ExternalConnector *)getComponent();	};

private slots:
	void slotOptionalConnToggled(bool state);

private:
	QGroupBox * m_optionalbox;
	QCheckBox * m_optionalConn;
	QCheckBox * m_optionalConnEna;
};


#endif
