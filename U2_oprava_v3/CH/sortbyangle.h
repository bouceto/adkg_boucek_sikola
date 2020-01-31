#ifndef SORTBYANGLE_H
#define SORTBYANGLE_H

#include "algorithms.h"

class sortbyangle
{
public:
    sortbyangle();

    bool operator()(QPointO &t, QPointO &t1)
    {
        if(fabs(t.omega-t1.omega) < 10e-6)
        {
            if(t.length < t1.length)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        else if(t.omega < t1.omega)
        {
            return 1;
        }

        else
        {
            return 0;
        }
    }
};

#endif // SORTBYANGLE_H
