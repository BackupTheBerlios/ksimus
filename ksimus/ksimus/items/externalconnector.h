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

#include "component.h"
#include "compview.h"

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


class ExternalConnector : public Component  {
public: 
	ExternalConnector(CompContainer * container, const ComponentInfo * ci);
	~ExternalConnector();
	
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
	virtual void checkProperty(QStringList & errorMsg);
	
	bool isInput() const { return input; };
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Returns the *external* connector */
	ConnectorBase * getExternalConn() const;
	/** Returns the *internal* connector */
	ConnectorBase * getInternalConn() const;

	/** Removes the wire of the test connector */
	void removeTestConnector();
	

	/** Returns the relative connector position in pixmap mode.
	 *  The position is measured in gridX / gridY. */
	QPoint getPixmapPos() const { return pixmapPos; };
	/** Sets the relative connector position in pixmap mode.
	 *  The position is measured in gridX / gridY. */
	void setPixmapPos(QPoint pos) { pixmapPos = pos; };
	/** Returns the orientation of the connector in pixmap mode. */
	ConnOrientationType getPixmapOrientation() const { return pixmapOrient; };
	/** Sets the orientation of the connector in pixmap mode.*/
	void setPixmapOrientation(ConnOrientationType orientation);
	
	/** Returns the relative connector position in user view mode.
	 *  The position is measured in gridX / gridY. */
	QPoint getUserViewPos() const { return userViewPos; };
	/** Sets the relative connector position in user view mode.
	 *  The position is measured in gridX / gridY. */
	void setUserViewPos(QPoint pos) { userViewPos = pos; };
	/** Returns the orientation of the connector in user view mode.*/
	ConnOrientationType getUserViewOrientation() const { return userViewOrient; };
	/** Sets the orientation of the connector in user view mode. */
	void setUserViewOrientation(ConnOrientationType orientation);
	
protected:
	bool input;
	QPoint pixmapPos;
	ConnOrientationType pixmapOrient;
	QPoint userViewPos;
	ConnOrientationType userViewOrient;
	
};

#endif
