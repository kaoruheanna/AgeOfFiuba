/*
 * MobileView.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef MOBILEVIEW_H_
#define MOBILEVIEW_H_

#include "View.h"
#include "../Models/MobileModel.h"

class MobileView: public View {
public:
	MobileView();
	virtual ~MobileView();
	void setModel(MobileModel *model);

protected:
	SDL_Point getOrigin();

private:
	MobileModel *model;
};

#endif /* MOBILEVIEW_H_ */
