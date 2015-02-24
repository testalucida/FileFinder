/* 
 * File:   FileFinderService.cpp
 * Author: max
 * 
 * Created on 22. Februar 2015, 18:36
 */

#include "FileFinderService.h"
#include "ApplicationStarter.h"

#include <my/FileHelper.h>

using namespace my;

FileFinderService *FileFinderService::_this = 0;

FileFinderService::FileFinderService() {
       
}

FileFinderService &FileFinderService::instance() {
    if( _this == NULL ) {
        _this = new FileFinderService();
    }
    return *_this;
}

SearchCriteriaPtr FileFinderService::getSearchCriteria() {
    SearchCriteriaPtr pSearchCrit( new SearchCriteria() );
    //set some defaults
    StringPtr curDir = FileHelper::instance().getCurrentDirectory();
    pSearchCrit->searchPath.add( curDir->c_str() );
    pSearchCrit->filePattern.add( "*.cpp; *.h" );
    pSearchCrit->includeSubDirs = true;
    pSearchCrit->matchCase = false;
    pSearchCrit->matchWord = false;
    return pSearchCrit;
    
}

HitListPtr FileFinderService::getHitList() {
    HitListPtr pHitList( new HitList() );
    return pHitList;
}