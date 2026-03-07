#ifndef DATABASE_HANDLING_HPP
#define DATABASE_HANDLING_HPP

#include "../../shared/classes/Date.hpp"
#include "../classes/UserAccount_server.hpp"

#include <string>
#include <vector>
// in Ubuntu, the include directory of libpq-fe is:            
// #include <postgresql/libpq-fe.h>
// in Arch, the include directory of libpq-fe is:
#include <libpq-fe.h> 



class OverallConDB 
{
public:
    virtual void show() = 0;
    virtual ~OverallConDB() = default;
};


class PG_Guard 
{
public:
    static bool ensure_conn(PGconn* c, const char* context);
    static bool expect_tuples(PGconn* c, PGresult* r, const char* context);
    static bool expect_command(PGconn* c, PGresult* r, const char* context, bool rollback = false);
    static bool expect_ok(PGconn* c, PGresult* r, const char* context, bool rollback = false);
    static bool command_and_clear(PGconn* c, PGresult*& r, const char* context, bool rollback = false);
};


class PG_Config_Info {
private:
    std::string host;
    std::string port;
    std::string user;
    std::string password;
    std::string target_db;
    std::string admin_db;

public:
    PG_Config_Info(
        std::string host_in = "localhost",
        std::string port_in = "5432",
        std::string user_in = "myuser",
        std::string password_in = "mypass",
        std::string target_db_in = "bankdb",
        std::string admin_db_in = "postgres"
    );

    PGconn* connect(const std::string& dbname = "");

    const std::string& get_target_db() const;
    const std::string& get_admin_db() const;

    void show();
};


class Database : public PG_Config_Info, public OverallConDB 
{
private:
    PGconn* connx;
    bool it_exists;
    std::string db_name;

    static bool exec_cmd(PGconn* c, const std::string& sql);

public:
    Database(PGconn* connx_in = nullptr, bool status = false, std::string name = "");
    ~Database() override;

    PGconn* get_conn() const;
    bool exists() const;

    static bool db_exists(PGconn* c, const std::string& dbname);
    static bool create_db(PGconn* c, const std::string& dbname);

    bool init_schema();

    void show() override;
};


class User_Queries 
{
private:
    PGconn* conn;

    static void clear_result(PGresult* r);

public:
    explicit User_Queries(PGconn* c);

    bool checkUniqueUsername(const std::string& username);

    void addUser(const UserAccount_server& user);

    bool hasEnoughBalance(const std::string& username, double amount);

    bool updateBalance(const std::string& username, double newBalance);

    UserAccount_server getUserAccount_server(const string &username);

    double User_Queries::getBalance(const string &username);
};


class DatabaseUpdates 
{
private:
    PGconn* conn;

public:
    explicit DatabaseUpdates(PGconn* connection);

    bool deleteUserByUsername(const std::string& username);
    

    bool updatePasswordByUsername(const std::string& username,
                                  const std::string& newPassword,
                                  const std::string& newSalt);

    std::string getSaltByUsername(const std::string& username);

    std::string getPasswordByUsername(const std::string& username);
};

#endif 
