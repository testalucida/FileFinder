// TryAndError.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <string>
#include <functional>
#include <regex>
#include <iostream>

#include "DirectoryIterator.h"
#include "FileFinder.h"
#include "searchcriteriagroup.h"
#include "FileFinderInputModel.h"
#include "FileFinderController.h"
#include "HitList.h"

#include <myfltk/Flx_Table.h>

#include <my/TableData.h>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Output.H>

using namespace std;
using namespace std::placeholders;
using namespace my;
using namespace myfltk;

class HitListener {
public:
	void showHits( const EntryPtr pEntry ) {
		printf( "hit: %s%s\n", pEntry->directory.c_str(), pEntry->name.c_str() );
	}
};

class Iterator {
public:
	Iterator() {
	}

	void iterateDirectory( const std::string &start = "" ) {
		HitListener hili;
		DirectoryIterator itr( start );
		itr.setHitCallback( std::bind( &HitListener::showHits, hili, _1 ) );
		itr.iterate();
	}
};

void testregex() {
	// regular expression
   const std::regex pattern("hello ([0-9]+)");

   // the source text
   std::string text = "hello 1, hello 2, hello 17, and done!";

   const std::sregex_token_iterator end;
   
   for (std::sregex_token_iterator i(text.cbegin(), text.cend(), pattern);
        i != end;
        ++i)
   {
      std::cout << *i << std::endl;
   }
}

class Callback {
public:
	void onMatch( const EntryPtr pEntry ) {
		fprintf( stderr, "Callback::onMatch: %s%s\n", 
			     pEntry->directory.c_str(), pEntry->name.c_str() );
	}
};

int main( void ) {

	FileFinderInputModel ffinModel;
	ffinModel.setSearchPath( "C:\\tmp" );

	Fl_Double_Window win(500, 100, 575, 440, "Dateien suchen und finden");
		SearchCriteriaGroup searchCritGrp( 0, 0, 575, 187, ffinModel );
		Flx_Table hitListTable( 0, 187, 575, 228 );
		Fl_Output statusMsg(0, 415, 575, 25, "Statusmessage");
		statusMsg.color((Fl_Color)131);
	win.end();
	win.resizable( hitListTable );


	HitList hitList;
	FileFinderController ffCtrl( ffinModel, hitList, statusMsg );

	searchCritGrp.setOnChooseSearchPathCallback( 
		bind( &FileFinderController::onChooseSearchPath, &ffCtrl, _1 ) );

	searchCritGrp.setOnStartStopCallback( 
		bind( &FileFinderController::onStartStopSearch, &ffCtrl, _1 ) );

	ffCtrl.setOnFinishedSearchCallback( 
		bind( &SearchCriteriaGroup::onFinishedSearch, &searchCritGrp ) );

	ffCtrl.setOnCancelCallback(
		bind( &SearchCriteriaGroup::onCancelSearch, &searchCritGrp ) );

	hitListTable.setTableData( hitList );

	win.show();

	int rc = Fl::run();
	//hitList.dump();
	return rc;
}


int main1( void )
{
   struct _finddata_t c_file;
   intptr_t hFile;

   // Find first .c file in current directory 
   if( (hFile = _findfirst( "C:\\fltk-1.3.1\\*.*", &c_file )) == -1L )
      printf( "No *.c files in current directory!\n" );
   else
   {
      printf( "Listing of .c files\n\n" );
      printf( "DIR RDO HID SYS ARC  FILE         DATE %25c SIZE\n", ' ' );
      printf( "--- --- --- --- ---  ----        ---- %25c ----\n", ' ' );
      do {
         char buffer[30];	 
		 printf( ( c_file.attrib & _A_SUBDIR ) ? " Y  " : " N  " );
         printf( ( c_file.attrib & _A_RDONLY ) ? " Y  " : " N  " );
         printf( ( c_file.attrib & _A_SYSTEM ) ? " Y  " : " N  " );
         printf( ( c_file.attrib & _A_HIDDEN ) ? " Y  " : " N  " );
         printf( ( c_file.attrib & _A_ARCH )   ? " Y  " : " N  " );
         ctime_s( buffer, _countof(buffer), &c_file.time_write );
         printf( " %-12s %.24s  %9ld\n",
            c_file.name, buffer, c_file.size );
      } while( _findnext( hFile, &c_file ) == 0 );
      _findclose( hFile );
   }
   getchar();
   return 0;
}


#include <random>
#include <iostream>

void f(int n1, int n2, int n3, const int& n4, int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}
 
int g(int n1)
{
    return n1;
}
 
struct Foo {
	Foo() : data(10) {}
    void print_sum(int n1, int n2)
    {
        std::cout << n1+n2 << '\n';
    }

	void fnc1( int n ) {
	}

    int data;
};
 
typedef std::function<void ( int n )> INT_CALLBACK;
typedef std::function<void ( void )> VOID_CALLBACK;

class Caller {
public:
	void setCallback( INT_CALLBACK cb ) {
		_cb = cb;
	}

	void setVoidCallback( VOID_CALLBACK cb ) {
		_vcb = cb;
	}

	INT_CALLBACK _cb;
	VOID_CALLBACK _vcb;
};

class Callee {
public:
	Callee() {
	}

	void callback( int n ) {
	}

	void voidCallback( void ) {
	}
};



int main3()
{
    using namespace std::placeholders;
 
    // demonstrates argument reordering and pass-by-reference
    int n = 7;
    auto f1 = std::bind(f, _2, _1, 42, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001); // 1 is bound by _1, 2 is bound by _2, 1001 is unused
 
    // nested bind subexpressions share the placeholders
    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    f2(10, 11, 12);
 
    // common use case: binding a RNG with a distribution
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    std::function<int()> rnd = std::bind(d, e);
    for(int n=0; n<10; ++n)
        std::cout << rnd() << ' ';
    std::cout << '\n';
 
    // bind to a member function
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, foo, 95, _1);
    f3(5);

	auto f5 = std::bind(&Foo::fnc1, foo, _1);
 
    // bind to member data
    auto f4 = std::bind(&Foo::data, _1);
    std::cout << f4(foo) << '\n';
	return 0;
}

int main4() {
	main3();
	Caller caller;
	Callee callee;
	caller.setCallback( bind( &Callee::callback, callee, _1 ) );
	caller.setVoidCallback( bind( &Callee::voidCallback, callee ) );
	return 0;
}