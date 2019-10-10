//
// Created by 赵立伟 on 2018/12/4.
//

#ifndef FLYDB_ABSTRACTLOGFACTORY_H
#define FLYDB_ABSTRACTLOGFACTORY_H

#include "AbstractLogHandler.h"

class AbstractLogFactory {
public:
    virtual AbstractLogHandler* getLogger() = 0;
};

#endif //FLYDB_ABSTRACTLOGFACTORY_H
