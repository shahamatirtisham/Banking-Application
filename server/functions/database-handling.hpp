#pragma once

#include "../../shared/classes/Date.hpp"
#include "../classes/UserAccount_server.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <postgresql/libpq-fe.h>   



class overall_con_db {
public:
    virtual void show() = 0;
    virtual ~overall_con_db() = default;
};


class PgGuard {
public:
    static bool ensure_conn(PGconn* c, const char* context);
    static bool expect_tuples(PGconn* c, PGresult* r, const char* context);
    static bool expect_command(PGconn* c, PGresult* r, const char* context, bool rollback=false);
    static bool expect_ok(PGconn* c, PGresult* r, const char* context, bool rollback=false);
    static bool command_and_clear(PGconn* c, PGresult*& r, const char* context, bool rollback=false);
};


class pgconfig_info {
private:
    std::string host;
    std::string port;
    std::string user;
    std::string password;
    std::string target_db;
    std::string admin_db;

public:
    pgconfig_info(std::string host_in = "localhost",
                  std::string port_in = "5432",
                  std::string user_in = "myuser",
                  std::string password_in = "mypass",
                  std::string target_db_in = "bankdb",
                  std::string admin_db_in = "postgres");

    PGconn* connect(const std::string& dbname = "");

    const std::string& get_target_db() const;
    const std::string& get_admin_db()  const;

    void show();
};


class database : public pgconfig_info, public overall_con_db {
private:
    PGconn* connx;
    bool it_exists;
    std::string db_name;

    static bool exec_cmd(PGconn* c, const std::string& sql);

public:
    database(PGconn* connx_in = nullptr, bool status = false, std::string name = "");
    ~database();

    PGconn* get_conn() const;
    bool exists() const;

    static bool db_exists(PGconn* c, const std::string& dbname);
    static bool create_db(PGconn* c, const std::string& dbname);

    bool init_schema();

    void show() override;
};


class User_Queries {
private:
    PGconn* conn;
    static void clear_result(PGresult* r);

public:
    explicit User_Queries(PGconn* c);

    bool checkUniqueUsername(const std::string& username);
    void addUser(const UserAccount_server& user);

    bool hasEnoughBalance(const std::string& username, double amount);
    bool updateBalance(const std::string& username, double newBalance);
};


class DatabaseUpdates {
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
