/*
   Feet of Fury

   menu_extras/picviewMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: picviewMenu.h,v 1.2 2003/05/11 18:50:49 bard Exp $
*/

#ifndef __PICVIEW_MENU_H
#define __PICVIEW_MENU_H

#include "extrasMenu.h"

class MessageBox;

class PicviewMenu : public ExtrasMenu {
public:
	PicviewMenu(MenuExtras * parent, ExtrasMenu * back, const char * srcfn, const char **msg, int msgcnt);
	virtual ~PicviewMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);

private:
	void redo();

	RefPtr<Drawable>	m_bkg;
	RefPtr<Banner>		m_pic;

	RefPtr<Texture>		m_txrPic;

	RefPtr<MessageBox>	m_msgbox;
	const char		** m_msg;
	int			m_msgCnt;
	
	float			m_zoom;
	float			m_xoffs, m_yoffs;

	ExtrasMenu		* m_back;
};

#endif	// __PICVIEW_MENU_H

