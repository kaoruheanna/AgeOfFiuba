#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG};
class Log
{
public:
   Log();
   virtual ~Log();
   ostringstream& Get(TLogLevel level = logINFO);
public:
   static TLogLevel ReportingLevel();
protected:
   ostringstream os;

private:
   Log(const Log&);
   Log& operator =(const Log&);
   ofstream fout;

   TLogLevel messageLevel;
   time_t GetExecutionTime();
   time_t NowTime();
   string TimeToString(time_t time);
};

