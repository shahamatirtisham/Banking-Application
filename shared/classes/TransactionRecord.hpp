#pragma once
#include <vector>
#include <iostream>



class TransactionRecord
{
    static std::string filename;

    std::string timeStamp, trnxID, trnxType, sender, receiver;
    double amount;
    public:
    TransactionRecord();
    static TransactionRecord deserialize(const std::string &line);
    std::string get_timeStamp() const;
    std::string get_trnxID() const;
    std::string get_trnxType() const; 
    std::string get_sender() const; 
    std::string get_receiver() const;
    double get_amount() const;

    void set_timeStamp(std::string);
    void set_trnxID(std::string);
    void set_trnxType(std::string); 
    void set_sender(std::string); 
    void set_receiver(std::string);

    static std::vector<TransactionRecord> retrieveTransactions();

};