#pragma once
#include <string>
#include <fstream>
#include <sys/time.h>

using namespace std;

// ─── Abstract Base Class ──────────────────────────────────────
// Contains shared functionality for all loggers
// Cannot be instantiated directly — must be subclassed
class Logger
{
protected:
    string logFile; // path to the log file

    // Returns current timestamp as [DD-MM-YYYY HH:MM:SS:MS]
    string getTimestamp() const;

    // Returns timestamp as compact string for IDs e.g. 20260308143022123
    string getTimestampForID() const;

    // Writes a single line to the log file (append mode)
    // Protected — only subclasses can call this
    void writeLine(const string &line);

public:
    // explicit — prevents implicit conversions
    //            e.g: Logger l = "somepath.log" error dekhabe
    //            likthte hobe Logger l("somepath.log")
    explicit Logger(const string &logFilePath);

    // Pure virtual — every subclass MUST implement this
    // etai Logger ke abstract class banaise
    virtual void display() const = 0;

    // Virtual destructor — required for proper cleanup in inheritance
    // Without this, deleting a subclass through a base class pointer
    // would only call the base destructor, causing memory leaks
    // = default means: compiler generates the default destructor
    virtual ~Logger() = default;
};

// ─── ActivityLogger ───────────────────────────────────────────
// Logs user activity events to activity.log
// Format: [DD-MM-YYYY HH:MM:SS:MS] EVENT | username: xxx
class ActivityLogger : public Logger
{
public:
    explicit ActivityLogger(const string &logFilePath = "../logs/activity.log");

    void logLogin(const string &username);
    void logLogout(const string &username);
    void logSignup(const string &username);
    void logPasswordChange(const string &username);
    void logForgotPassword(const string &username);

    // Implements pure virtual from Logger
    void display() const override;
};

// ─── TransactionLogger ────────────────────────────────────────
// Format: [DD-MM-YYYY HH:MM:SS:MS] TRXID: xxx | Type: xxx | ...
class TransactionLogger : public Logger
{
private:
    // unique transaction ID generate kore
    // Format: <TypeChar>-<username>-<timestamp>
    // e.g: D-arkam-20260308143022123
    string generateTrxID(const string &username, char typeChar) const;

public:
    explicit TransactionLogger(const string &logFilePath = "../logs/transaction.log");

    void logDeposit(const string &username, double amount);
    void logWithdraw(const string &username, double amount);
    void logTransfer(const string &senderUsername, const string &receiverUserName, double amount);

    // Implements pure virtual from Logger
    void display() const override;
};
