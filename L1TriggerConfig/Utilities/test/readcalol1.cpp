#include "L1Trigger/L1TCommon/src/Setting.cc"
#include "L1Trigger/L1TCommon/src/Mask.cc"
#include "L1Trigger/L1TCommon/src/XmlConfigReader.cc"
#include "L1Trigger/L1TCommon/src/TrigSystem.cc"

#include <iostream>
#include <fstream>

// To compile run these lines in your CMSSW_X_Y_Z/src/ :
/*
cmsenv
eval "export `scram tool info xerces-c | sed -n -e 's/INCLUDE=/XERC_INC=/gp'`"
eval "export `scram tool info xerces-c | sed -n -e 's/LIBDIR=/XERC_LIB=/gp'`"
eval "export `scram tool info boost    | sed -n -e 's/INCLUDE=/BOOST_INC=/gp'`"
eval "export `scram tool info boost    | sed -n -e 's/LIBDIR=/BOOST_LIB=/gp'`"
g++ -g -std=c++11 -o test readcalol1.cpp -I./ -I$CMSSW_RELEASE_BASE/src -I$XERC_INC -L$XERC_LIB -lxerces-c -I$BOOST_INC -L$BOOST_LIB -lboost_thread -lboost_signals -lboost_date_time -L$CMSSW_RELEASE_BASE/lib/$SCRAM_ARCH/ -lFWCoreMessageLogger -lCondFormatsL1TObjects
*/

using namespace std;

int main(int argc, char *argv[]){
    // read the input xml file into a string
    ifstream input( argv[1] );
    if( !input ){ cout << "Cannot open " << argv[1] << " file" << endl; return 0; }

    string xmlPayload;
    size_t nLinesRead=0;

    while( !input.eof() ){
        string tmp;
        getline( input, tmp, '\n' );
        xmlPayload.append( tmp );
        nLinesRead++;
    }

    cout << "read " << nLinesRead << " lines" << endl;
    input.close();

    // parse the string using the XML reader
    XmlConfigReader xmlReader;
    xmlReader.readDOMFromString( xmlPayload ); // initialize it

    l1t::TrigSystem ts;
    ts.addProcRole("processors", "processors"); //
    xmlReader.readRootElement( ts, "calol1" ); // extract all of the relevant context (labeled "calol1" here)
    ts.setConfigured();

    // feel free to play with the containers:
    map<string, l1t::Setting> conf = ts.getSettings("processors"); // use your context id here
//    map<string, l1t::Mask>    rs   = ts.getMasks   ("processors"); // don't call a context that doesn't exist

    string tmp = conf["layer1ECalScaleETBins"].getValueAsStr();
    cout << "layer1ECalScaleETBins =" << tmp << endl;


    return 0;
}

