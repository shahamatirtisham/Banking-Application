#include "../../shared/classes/Date.hpp"
#include "../classes/UserAccount_server.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <libpq-fe.h>





class overall_con_db {
public:
    virtual void show() = 0;
    virtual ~overall_con_db() = default;
};


class PgGuard {
public:
    static bool ensure_conn(PGconn* c, const char* context) {
        if (!c) { std::cerr << context << ": connection is null\n"; return false; }
        if (PQstatus(c) != CONNECTION_OK) {
            std::cerr << context << ": connection not OK: " << PQerrorMessage(c) << "\n";
            return false;
        }
        return true;
    }

    static bool expect_tuples(PGconn* c, PGresult* r, const char* context) {
        if (!r) { std::cerr << context << ": PQexec/PQexecParams returned nullptr\n"; return false; }
        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
            std::cerr << context << ": " << PQerrorMessage(c) << "\n";
            PQclear(r);
            return false;
        }
        return true;
    }

    static bool expect_command(PGconn* c, PGresult* r, const char* context, bool rollback=false) {
        if (!r) {
            std::cerr << context << ": PQexec/PQexecParams returned nullptr\n";
            if (rollback) PQexec(c, "ROLLBACK;");
            return false;
        }
        if (PQresultStatus(r) != PGRES_COMMAND_OK) {
            std::cerr << context << ": " << PQerrorMessage(c) << "\n";
            PQclear(r);
            if (rollback) PQexec(c, "ROLLBACK;");
            return false;
        }
        return true;
    }

    static bool expect_ok(PGconn* c, PGresult* r, const char* context, bool rollback=false) {
        if (!r) {
            std::cerr << context << ": PQexec/PQexecParams returned nullptr\n";
            if (rollback) PQexec(c, "ROLLBACK;");
            return false;
        }
        auto st = PQresultStatus(r);
        if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK) {
            std::cerr << context << ": " << PQerrorMessage(c) << "\n";
            PQclear(r);
            if (rollback) PQexec(c, "ROLLBACK;");
            return false;
        }
        return true;
    }

    static bool command_and_clear(PGconn* c, PGresult*& r, const char* context, bool rollback=false) {
        if (!expect_command(c, r, context, rollback)) { r = nullptr; return false; }
        PQclear(r);
        r = nullptr;
        return true;
    }
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
    pgconfig_info(
        std::string host_in = "localhost",
        std::string port_in = "5432",
        std::string user_in = "myuser",
        std::string password_in = "mypass",
        std::string target_db_in = "bankdb",
        std::string admin_db_in = "postgres"
    )
        : host(std::move(host_in)),
          port(std::move(port_in)),
          user(std::move(user_in)),
          password(std::move(password_in)),
          target_db(std::move(target_db_in)),
          admin_db(std::move(admin_db_in)) {}

    PGconn* connect(const std::string& dbname = "") {
        const std::string& db = dbname.empty() ? admin_db : dbname;

        auto conninfo = [&]() {
            return "host=" + host + " port=" + port + " dbname=" + db +
                   " user=" + user + " password=" + password;
        };

        PGconn* conn = PQconnectdb(conninfo().c_str());

        if (!conn || PQstatus(conn) != CONNECTION_OK) {
            std::cerr << (conn ? PQerrorMessage(conn) : "PQconnectdb returned nullptr") << "\n";
            std::cout << "Connection Failed\n";
            if (conn) PQfinish(conn);
            return nullptr;
        }

        return conn;
    }

    const std::string& get_target_db() const { return target_db; }
    const std::string& get_admin_db()  const { return admin_db; }

    void show() {
        std::cout << "host: " << host << "\n";
        std::cout << "port: " << port << "\n";
        std::cout << "user: " << user << "\n";
        std::cout << "target db : " << target_db << "\n";
        std::cout << "admin db : " << admin_db << "\n";
    }
};


class database : public pgconfig_info, public overall_con_db {
private:
    PGconn* connx;
    bool it_exists;
    std::string db_name;

    static bool exec_cmd(PGconn* c, const std::string& sql) {
        if (!PgGuard::ensure_conn(c, "exec_cmd")) return false;

        PGresult* r = PQexec(c, sql.c_str());
        bool ok = PgGuard::expect_ok(c, r, ("SQL failed: " + sql).c_str());
        if (r) PQclear(r);
        return ok;
    }

public:
    database(PGconn* connx_in = nullptr, bool status = false, std::string name = "")
        : connx(connx_in),
          it_exists(status),
          db_name(std::move(name)),
          pgconfig_info()
    {
        if (db_name.empty()) db_name = get_target_db();

        if (!connx || PQstatus(connx) != CONNECTION_OK) {

            PGconn* admin = connect(get_admin_db());
            if (!admin) {
                it_exists = false;
                connx = nullptr;
                return;
            }

            if (!db_exists(admin, db_name)) {
                bool made = create_db(admin, db_name);
                if (made) {
                    std::cout << "Database has been made\n";
                    it_exists = true;
                } else {
                    std::cout << "Create Database failed\n";
                    it_exists = false;
                    PQfinish(admin);
                    connx = nullptr;
                    return;
                }
            } else {
                it_exists = true;
            }

            PQfinish(admin);

            connx = connect(db_name);
            if (!connx) {
                it_exists = false;
                return;
            }

            if (!init_schema()) {
                std::cerr << "Schema init failed\n";
                it_exists = false;
                return;
            }

        } else {
            it_exists = true;

            if (!init_schema()) {
                std::cerr << "Schema init failed\n";
                it_exists = false;
                return;
            }
        }
    }

    ~database() override {
        if (connx) PQfinish(connx);
        connx = nullptr;
    }

    PGconn* get_conn() const { return connx; }
    bool exists() const { return it_exists; }

    static bool db_exists(PGconn* c, const std::string& dbname) {
        if (!PgGuard::ensure_conn(c, "db_exists")) return false;

        const char* sql = "SELECT 1 FROM pg_database WHERE datname = $1;";
        const char* vals[1] = { dbname.c_str() };

        PGresult* r = PQexecParams(c, sql, 1, nullptr, vals, nullptr, nullptr, 0);
        if (!PgGuard::expect_tuples(c, r, "db_exists")) return false;

        bool exists = PQntuples(r) > 0;
        PQclear(r);
        return exists;
    }

    static bool create_db(PGconn* c, const std::string& dbname) {
        if (!PgGuard::ensure_conn(c, "create_db")) return false;

        char* q = PQescapeIdentifier(c, dbname.c_str(), dbname.size());
        if (!q) return false;

        std::string sql = std::string("CREATE DATABASE ") + q + ";";
        PQfreemem(q);

        return exec_cmd(c, sql);
    }

    bool init_schema() {
        if (!PgGuard::ensure_conn(connx, "init_schema")) return false;

        if (!exec_cmd(connx, "BEGIN;")) return false;

        const std::vector<std::string> ddl = {
            R"SQL(
            CREATE TABLE IF NOT EXISTS client_personal_info (
                client_ID   VARCHAR(5) PRIMARY KEY,
                name        VARCHAR(50) NOT NULL,
                username    VARCHAR(64) NOT NULL UNIQUE,
                password    VARCHAR(64) NOT NULL,
                DOB         DATE,
                account_no  VARCHAR(13) UNIQUE,
                favAni      VARCHAR(30),
                salt        VARCHAR(16) NOT NULL
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS client_account_status (
                client_ID VARCHAR(5) PRIMARY KEY
                    REFERENCES client_personal_info(client_ID) ON DELETE CASCADE,
                balance DOUBLE PRECISION NOT NULL DEFAULT 0
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS client_activity_type (
                code INT PRIMARY KEY,
                type VARCHAR(20) NOT NULL UNIQUE
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS client_activity (
                activity_id BIGSERIAL PRIMARY KEY,
                time        TIMESTAMPTZ NOT NULL DEFAULT now(),
                client_id   VARCHAR(5) NOT NULL
                    REFERENCES client_personal_info(client_ID) ON DELETE CASCADE,
                code        INT NOT NULL
                    REFERENCES client_activity_type(code)
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS transaction_types (
                code INT PRIMARY KEY,
                type VARCHAR(10) NOT NULL UNIQUE
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS transactions (
                trxid       VARCHAR(64) PRIMARY KEY,
                time        TIMESTAMPTZ NOT NULL DEFAULT now(),
                sender_id   VARCHAR(5) NOT NULL
                    REFERENCES client_personal_info(client_ID),
                receiver_id VARCHAR(5)
                    REFERENCES client_personal_info(client_ID),
                code        INT NOT NULL
                    REFERENCES transaction_types(code)
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS admin_info (
                admin_ID  VARCHAR(5) PRIMARY KEY,
                name      VARCHAR(50) NOT NULL,
                username  VARCHAR(64) NOT NULL UNIQUE,
                password  VARCHAR(64) NOT NULL
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS admin_activity_type (
                code INT PRIMARY KEY,
                type VARCHAR(20) NOT NULL UNIQUE
            );
            )SQL",
            R"SQL(
            CREATE TABLE IF NOT EXISTS admin_activity (
                activity_id BIGSERIAL PRIMARY KEY,
                time        TIMESTAMPTZ NOT NULL DEFAULT now(),
                admin_id    VARCHAR(5)
                    REFERENCES admin_info(admin_ID) ON DELETE SET NULL,
                code        INT NOT NULL
                    REFERENCES admin_activity_type(code)
            );
            )SQL",
            R"SQL(
            CREATE INDEX IF NOT EXISTS idx_client_activity_client_time
                ON client_activity(client_id, time DESC);
            )SQL",
            R"SQL(
            CREATE INDEX IF NOT EXISTS idx_transactions_sender_time
                ON transactions(sender_id, time DESC);
            )SQL",
            R"SQL(
            CREATE INDEX IF NOT EXISTS idx_transactions_receiver_time
                ON transactions(receiver_id, time DESC);
            )SQL"
        };

        for (const auto& sql : ddl) {
            if (!exec_cmd(connx, sql)) {
                exec_cmd(connx, "ROLLBACK;");
                return false;
            }
        }

        if (!exec_cmd(connx, "COMMIT;")) {
            exec_cmd(connx, "ROLLBACK;");
            return false;
        }

        return true;
    }

    void show() override {
        std::cout << "Status : " << (it_exists ? "Found & Fetchable." : "Not Found.") << "\n";
        std::cout << "Database name : " << db_name << "\n";
        pgconfig_info::show();
    }
};


class User_Queries {
private:
    PGconn* conn;

    static void clear_result(PGresult* r) {
        if (r) PQclear(r);
    }

public:
    explicit User_Queries(PGconn* c) : conn(c) {}

    bool checkUniqueUsername(const std::string& username) {
        if (!PgGuard::ensure_conn(conn, "checkUniqueUsername")) return false;

        const char* sql =
            "SELECT 1 FROM client_personal_info WHERE username = $1 LIMIT 1;";
        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_tuples(conn, res, "checkUniqueUsername SELECT failed")) return false;

        bool isUnique = (PQntuples(res) == 0);
        PQclear(res);
        return isUnique;
    }

    void addUser(const UserAccount_server& user) {
        if (!PgGuard::ensure_conn(conn, "addUser")) return;

        if (!checkUniqueUsername(user.getUsername())) {
            std::cerr << "Username already exists.\n";
            return;
        }

        PGresult* res = PQexec(conn, "BEGIN;");
        if (!PgGuard::command_and_clear(conn, res, "BEGIN failed")) return;

        const char* sql1 =
            "INSERT INTO client_personal_info "
            "(client_ID, name, username, password, DOB, account_no, favAni, salt) "
            "VALUES ($1,$2,$3,$4,$5,$6,$7,$8);";

        Date DOB = user.getDOB();
        std::string Date_str =
            std::to_string(DOB.getYear()) + "-" +
            (DOB.getMonth() < 10 ? "0" : "") + std::to_string(DOB.getMonth()) + "-" +
            (DOB.getDate()  < 10 ? "0" : "") + std::to_string(DOB.getDate());

        const char* values1[8] = {
            user.getClientID().getHexadecimalVal().c_str(),
            user.getName().c_str(),
            user.getUsername().c_str(),
            user.getPassword().c_str(),
            Date_str.c_str(),
            user.getAccountNo().c_str(),
            user.getFavAni().c_str(),
            user.getSalt().c_str()
        };

        res = PQexecParams(conn, sql1, 8, nullptr, values1, nullptr, nullptr, 0);
        if (!PgGuard::expect_command(conn, res, "Insert personal info failed", true)) return;
        PQclear(res);

        const char* sql2 = "INSERT INTO client_account_status (client_ID) VALUES ($1);";
        const char* values2[1] = { user.getClientID().getHexadecimalVal().c_str() };

        res = PQexecParams(conn, sql2, 1, nullptr, values2, nullptr, nullptr, 0);
        if (!PgGuard::expect_command(conn, res, "Insert account status failed", true)) return;
        PQclear(res);

        res = PQexec(conn, "COMMIT;");
        if (!PgGuard::expect_command(conn, res, "COMMIT failed", true)) return;
        PQclear(res);

        std::cout << "User successfully added.\n";
    }

    bool hasEnoughBalance(const std::string& username, double amount) {
        if (!PgGuard::ensure_conn(conn, "hasEnoughBalance")) return false;

        const char* sql =
            "SELECT cas.balance "
            "FROM client_account_status cas "
            "JOIN client_personal_info cpi "
            "ON cas.client_ID = cpi.client_ID "
            "WHERE cpi.username = $1;";

        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_tuples(conn, res, "hasEnoughBalance SELECT failed")) return false;

        if (PQntuples(res) == 0) { PQclear(res); return false; }

        double balance = std::atof(PQgetvalue(res, 0, 0));
        PQclear(res);
        return balance >= amount;
    }

    bool updateBalance(const std::string& username, double newBalance) {
        if (!PgGuard::ensure_conn(conn, "updateBalance")) return false;

        const char* sql =
            "UPDATE client_account_status cas "
            "SET balance = $1 "
            "FROM client_personal_info cpi "
            "WHERE cas.client_ID = cpi.client_ID "
            "AND cpi.username = $2;";

        std::string balStr = std::to_string(newBalance);
        const char* values[2] = { balStr.c_str(), username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 2, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_command(conn, res, "updateBalance UPDATE failed")) return false;

        PQclear(res);
        return true;
    }
};


class DatabaseUpdates {
private:
    PGconn* conn;

public:
    explicit DatabaseUpdates(PGconn* connection) : conn(connection) {}

    bool deleteUserByUsername(const std::string& username) {
        if (!PgGuard::ensure_conn(conn, "deleteUserByUsername")) return false;

        const char* sql =
            "DELETE FROM client_personal_info "
            "WHERE username = $1;";

        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_command(conn, res, "deleteUserByUsername DELETE failed")) return false;

        PQclear(res);
        return true;
    }

    bool updatePasswordByUsername(const std::string& username,
                                  const std::string& newPassword,
                                  const std::string& newSalt) {
        if (!PgGuard::ensure_conn(conn, "updatePasswordByUsername")) return false;

        const char* sql =
            "UPDATE client_personal_info "
            "SET password = $1, salt = $2 "
            "WHERE username = $3;";

        const char* values[3] = { newPassword.c_str(), newSalt.c_str(), username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 3, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_command(conn, res, "updatePasswordByUsername UPDATE failed")) return false;

        PQclear(res);
        return true;
    }

    std::string getSaltByUsername(const std::string& username) {
        if (!PgGuard::ensure_conn(conn, "getSaltByUsername")) return "";

        const char* sql =
            "SELECT salt FROM client_personal_info WHERE username = $1 LIMIT 1;";
        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_tuples(conn, res, "getSaltByUsername SELECT failed")) return "";

        if (PQntuples(res) == 0) { PQclear(res); return ""; }

        std::string salt = PQgetvalue(res, 0, 0);
        PQclear(res);
        return salt;
    }

    std::string getPasswordByUsername(const std::string& username) {
        if (!PgGuard::ensure_conn(conn, "getPasswordByUsername")) return "";

        const char* sql =
            "SELECT password FROM client_personal_info WHERE username = $1 LIMIT 1;";
        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
        if (!PgGuard::expect_tuples(conn, res, "getPasswordByUsername SELECT failed")) return "";

        if (PQntuples(res) == 0) { PQclear(res); return ""; }

        std::string password = PQgetvalue(res, 0, 0);
        PQclear(res);
        return password;
    }
};