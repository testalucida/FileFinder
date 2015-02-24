/* 
 * File:   FileFinderService.h
 * Author: max
 *
 * Created on 22. Februar 2015, 18:36
 */

#ifndef FILEFINDERSERVICE_H
#define	FILEFINDERSERVICE_H

#include "SearchCriteria.h"
#include "HitList.h"

class FileFinderService {
public:
    static FileFinderService &instance();
    SearchCriteriaPtr getSearchCriteria();
    HitListPtr getHitList();
private:
    FileFinderService();
private:
    static FileFinderService *_this;

};

#endif	/* FILEFINDERSERVICE_H */

