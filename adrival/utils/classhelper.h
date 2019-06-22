#ifndef ADRIVAL_CLASSHELPER_H
#define ADRIVAL_CLASSHELPER_H

#define OBJECTADDRESS(classname,menbername,pmenber)\
	(classname*)((unsigned long)pmenber-(unsigned long)(&((classname*)nullptr)->menbername))

#endif // !ADRIVAL_CLASSHELPER_H