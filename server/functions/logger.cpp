#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <sys/time.h>
using namespace std;

// ─────────────────────────────────────────────
// Logger (Base Class)
// ─────────────────────────────────────────────

Logger::Logger(const string &logFilePath) : logFile(logFilePath) {}

// getTimestamp() — current date ar time formatted akare return kore string hishebe

string Logger::getTimestamp() const
{
    // timeval e duita field ase ...
    // tv_sec = 1 Jan, 1970 theke second
    // tv_usec = microseconds
    struct timeval tv;

    // gettimeofday() function ta tv struct e current time diye fill kore
    // ar ei second argument ta timezone ... nullptr hocche default timezone system (nijer pc) er
    gettimeofday(&tv, nullptr);

    // eta ekta long integer currenct time ke 'now' te vortese string hishebe
    time_t now = tv.tv_sec;

    // localtime() - eta human readable format e antese
    struct tm *t = localtime(&now);

    // microseconds ke milliseconds e antese
    int ms = tv.tv_usec / 1000;

    char buf[30];
    snprintf(buf, sizeof(buf),
             "%02d-%02d-%04d %02d:%02d:%02d:%03d",
             t->tm_mday,
             t->tm_mon + 1,     // month er saathe ek jog korsi karon month 0-11 te store kora
             t->tm_year + 1900, // 1900 jog korsi karon current time to 1900, Jan 1 theke shuru hoise .. actual time ta janar jonne
             t->tm_hour,
             t->tm_min,
             t->tm_sec,
             ms);
    return string(buf);
}

string Logger::getTimestampForID() const
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t now = tv.tv_sec;
    struct tm *t = localtime(&now);
    int ms = tv.tv_usec / 1000;

    char buf[20];
    snprintf(buf, sizeof(buf),
             "%04d%02d%02d%02d%02d%02d%03d",
             t->tm_year + 1900,
             t->tm_mon + 1,
             t->tm_mday,
             t->tm_hour,
             t->tm_min,
             t->tm_sec,
             ms);
    return string(buf);
}

// writeLine() - ekta kore single line append kore log file e
// eta protected ... shudhu matro subclasses eta call krte parbe
void Logger::writeLine(const string &line)
{
    // ofstream in append mode
    // ios::app — adds to end of file, creates file if doesn't exist
    // logFile - the path to the file

    ofstream file(logFile, ios::app);
    if (!file.is_open())
    {
        cerr << "Logger: could not open " << logFile << "\n";
        return;
    }
    file << line << "\n";
    file.close();
}

// ─────────────────────────────────────────────
// ActivityLogger
// ─────────────────────────────────────────────

ActivityLogger::ActivityLogger(const string &logFilePath) : Logger(logFilePath) {} // calls base class constructor
void ActivityLogger::logLogin(const string &username)
{
    string line = getTimestamp() + " | LOGIN           | username: " + username;
    writeLine(line);
    cout << "Activity logged: LOGIN - " << username << "\n";
}

void ActivityLogger::logLogout(const string &username)
{
    string line = getTimestamp() + " | LOGOUT          | username: " + username;
    writeLine(line);
    cout << "Activity logged: LOGOUT - " << username << "\n";
}

void ActivityLogger::logSignup(const string &username)
{
    string line = getTimestamp() + " | SIGNUP          | username: " + username;
    writeLine(line);
    cout << "Activity logged: SIGNUP - " << username << "\n";
}

void ActivityLogger::logPasswordChange(const string &username)
{
    string line = getTimestamp() + " | PASSWORD_CHANGE | username: " + username;
    writeLine(line);
    cout << "Activity logged: PASSWORD_CHANGE - " << username << "\n";
}

void ActivityLogger::logForgotPassword(const string &username)
{
    string line = getTimestamp() + " | FORGOT_PASSWORD | username: " + username;
    writeLine(line);
    cout << "Activity logged: FORGOT_PASSWORD - " << username << "\n";
}

void ActivityLogger::display() const
{
    // Opens file in read mode and prints all lines
    ifstream file(logFile);
    if (!file.is_open())
    {
        cerr << "ActivityLogger: could not open " << logFile << "\n";
        return;
    }
    cout << "\n-------- Activity Log --------\n";
    string line;
    while (getline(file, line))
        cout << line << "\n";
    cout << "------------------------------\n";
    file.close();
}

// ─────────────────────────────────────────────
// TransactionLogger
// ─────────────────────────────────────────────

TransactionLogger::TransactionLogger(const string &logFilePath)
    : Logger(logFilePath) {} // calls base class constructor

string TransactionLogger::generateTrxID(const string &username, char typeChar) const
{
    // ekta character return kore string(1, typchar) - deposit hoile D, withdraw hoile w
    return string(1, typeChar) + "-" + username + "-" + getTimestampForID();
}
void TransactionLogger::logDeposit(const string &username, double amount)
{
    string trxid = generateTrxID(username, 'D');
    ostringstream oss;
    oss << getTimestamp()
        << " | " << setw(20) << left << trxid
        << " | D"
        << " | " << setw(15) << left << username
        << " | " // receiver empty for deposit 
        << " | Amount: " << fixed << setprecision(2) << amount;
    writeLine(oss.str());
    cout << "Transaction logged: DEPOSIT - " << username << " - " << amount << "\n";
}

void TransactionLogger::logWithdraw(const string &username, double amount)
{
    string trxid = generateTrxID(username, 'W');
    ostringstream oss;
    oss << getTimestamp()
        << "|" << trxid
        << "|W"
        << "|" << setw(15) << left << username
        << "|" // receiver empty for withdraw
        << "|" << fixed << setprecision(2) << amount;
    writeLine(oss.str());
    cout << "Transaction logged: WITHDRAW - " << username << " - " << amount << "\n";
}

void TransactionLogger::logTransfer(const string &senderUsername, const string &receiverUserName, double amount)
{
    string trxid = generateTrxID(senderUsername, 'T');
    ostringstream oss;
    oss << getTimestamp()
        << "|" <<  trxid
        << "|T"
        << "|" <<  senderUsername
        << "|" <<  receiverUserName
        << "|" <<  fixed << setprecision(2) << amount;
    writeLine(oss.str());
    cout << "Transaction logged: TRANSFER - " << senderUsername
         << " -> " << receiverUserName << " - " << amount << "\n";
}

void TransactionLogger::display() const
{
    ifstream file(logFile);
    if (!file.is_open())
    {
        cerr << "TransactionLogger: could not open " << logFile << "\n";
        return;
    }
    cout << "\n-------- Transaction Log --------\n";
    string line;
    while (getline(file, line))
        cout << line << "\n";
    cout << "---------------------------------\n";
    file.close();
}