/*      
   Feet of Fury

   finishall.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: finishall.h,v 1.1 2003/03/22 08:19:33 bard Exp $
*/

#ifndef __FINISHALL_H
#define __FINISHALL_H

// Calls setFinished() on a set of objects when it is
// triggered. The parent object and animation are ignored.

class FinishAll : public Trigger {
public:
	FinishAll() { }
	virtual ~FinishAll() { }

	void addTarget(Drawable *t) {
		m_targets.insertHead(t);
	}

	void trigger(Drawable *t, Animation *a) {
		ListNode<Drawable> * n = m_targets.getHead();
		while (n) {
			(*n)->setFinished();
			n = n->getNext();
		}
		Trigger::trigger(t, a);
	}

private:
	List<Drawable>		m_targets;
};

#endif	/* __REMOVEALL_H */

