#include "../../shared/classes/Date.hpp"
#include "../classes/UserAccount_server.hpp"
#include "database-handling.hpp"
#include <iostream>
#include <string>
#include <vector>

// in Ubuntu, the include directory of libpq-fe is:
// #include <postgresql/libpq-fe.h>
// in Arch, the include directory of libpq-fe is:
#include <libpq-fe.h>
using namespace std;

bool PG_Guard::ensure_conn(PGconn *c, const char *context)
{
    if (!c)
    {
        cerr << context << ": connection is null\n";
        return false;
    }
    if (PQstatus(c) != CONNECTION_OK)
    {
        cerr << context << ": connection not OK: " << PQerrorMessage(c) << "\n";
        return false;
    }
    return true;
}
bool PG_Guard::expect_tuples(PGconn *c, PGresult *r, const char *context)
{
    if (!r)
    {
        cerr << context << ": PQexec/PQexecParams returned nullptr\n";
        return false;
    }
    if (PQresultStatus(r) != PGRES_TUPLES_OK)
    {
        cerr << context << ": " << PQerrorMessage(c) << "\n";
        PQclear(r);
        return false;
    }
    return true;
}
bool PG_Guard::expect_command(PGconn *c, PGresult *r, const char *context, bool rollback)
{
    if (!r)
    {
        cerr << context << ": PQexec/PQexecParams returned nullptr\n";
        if (rollback)
            PQexec(c, "ROLLBACK;");
        return false;
    }
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
    {
        cerr << context << ": " << PQerrorMessage(c) << "\n";
        PQclear(r);
        if (rollback)
            PQexec(c, "ROLLBACK;");
        return false;
    }
    return true;
}
bool PG_Guard::expect_ok(PGconn *c, PGresult *r, const char *context, bool rollback)
{
    if (!r)
    {
        cerr << context << ": PQexec/PQexecParams returned nullptr\n";
        if (rollback)
            PQexec(c, "ROLLBACK;");
        return false;
    }
    auto st = PQresultStatus(r);
    if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK)
    {
        cerr << context << ": " << PQerrorMessage(c) << "\n";
        PQclear(r);
        if (rollback)
            PQexec(c, "ROLLBACK;");
        return false;
    }
    return true;
}
bool PG_Guard::command_and_clear(PGconn *c, PGresult *&r, const char *context, bool rollback)
{
    if (!expect_command(c, r, context, rollback))
    {
        r = nullptr;
        return false;
    }
    PQclear(r);
    r = nullptr;
    return true;
}

PG_Config_Info::PG_Config_Info(
    string host_in,
    string port_in,
    string user_in,
    string password_in,
    string target_db_in,
    string admin_db_in)
    : host(move(host_in)),
      port(move(port_in)),
      user(move(user_in)),
      password(move(password_in)),
      target_db(move(target_db_in)),
      admin_db(move(admin_db_in)) {}

PGconn *PG_Config_Info::connect(const string &dbname)
{
    const string &db = dbname.empty() ? admin_db : dbname;

    auto conninfo = [&]()
    {
        return "host=" + host + " port=" + port + " dbname=" + db +
               " user=" + user + " password=" + password;
    };

    PGconn *conn = PQconnectdb(conninfo().c_str());

    if (!conn || PQstatus(conn) != CONNECTION_OK)
    {
        cerr << (conn ? PQerrorMessage(conn) : "PQconnectdb returned nullptr") << "\n";
        cout << "Connection Failed\n";
        if (conn)
            PQfinish(conn);
        return nullptr;
    }

    return conn;
}

const string &PG_Config_Info::get_target_db() const { return target_db; }
const string &PG_Config_Info::get_admin_db() const { return admin_db; }

void PG_Config_Info::show()
{
    cout << "host: " << host << "\n";
    cout << "port: " << port << "\n";
    cout << "user: " << user << "\n";
    cout << "target db : " << target_db << "\n";
    cout << "admin db : " << admin_db << "\n";
}

Database::Database(PGconn *connx_in, bool status, string name)
    : connx(connx_in),
      it_exists(status),
      db_name(move(name)),
      PG_Config_Info()
{
    if (db_name.empty())
        db_name = get_target_db();

    if (!connx || PQstatus(connx) != CONNECTION_OK)
    {

        PGconn *admin = connect(get_admin_db());
        if (!admin)
        {
            it_exists = false;
            connx = nullptr;
            return;
        }

        if (!db_exists(admin, db_name))
        {
            bool made = create_db(admin, db_name);
            if (made)
            {
                cout << "Database has been made\n";
                it_exists = true;
            }
            else
            {
                cout << "Create Database failed\n";
                it_exists = false;
                PQfinish(admin);
                connx = nullptr;
                return;
            }
        }
        else
        {
            it_exists = true;
        }

        PQfinish(admin);

        connx = connect(db_name);
        if (!connx)
        {
            it_exists = false;
            return;
        }

        if (!init_schema())
        {
            cerr << "Schema init failed\n";
            it_exists = false;
            return;
        }
    }
    else
    {
        it_exists = true;

        if (!init_schema())
        {
            cerr << "Schema init failed\n";
            it_exists = false;
            return;
        }
    }
}
Database::~Database() {}
bool Database::exec_cmd(PGconn *c, const string &sql)
{
    if (!PG_Guard::ensure_conn(c, "exec_cmd"))
        return false;

    PGresult *r = PQexec(c, sql.c_str());
    bool ok = PG_Guard::expect_ok(c, r, ("SQL failed: " + sql).c_str());
    if (r)
        PQclear(r);
    return ok;
}
PGconn *Database::get_conn() const { return connx; }
bool Database::exists() const { return it_exists; }
bool Database::db_exists(PGconn *c, const string &dbname)
{
    if (!PG_Guard::ensure_conn(c, "db_exists"))
        return false;

    const char *sql = "SELECT 1 FROM pg_Database WHERE datname = $1;";
    const char *vals[1] = {dbname.c_str()};

    PGresult *r = PQexecParams(c, sql, 1, nullptr, vals, nullptr, nullptr, 0);
    if (!PG_Guard::expect_tuples(c, r, "db_exists"))
        return false;

    bool exists = PQntuples(r) > 0;
    PQclear(r);
    return exists;
}
bool Database::create_db(PGconn *c, const string &dbname)
{
    if (!PG_Guard::ensure_conn(c, "create_db"))
        return false;

    char *q = PQescapeIdentifier(c, dbname.c_str(), dbname.size());
    if (!q)
        return false;

    string sql = string("CREATE Database ") + q + ";";
    PQfreemem(q);

    return exec_cmd(c, sql);
}
bool Database::init_schema()
{
    if (!PG_Guard::ensure_conn(connx, "init_schema"))
        return false;

    if (!exec_cmd(connx, "BEGIN;"))
        return false;

    const vector<string> ddl =
        {
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
        )SQL"};

    for (const auto &sql : ddl)
    {
        if (!exec_cmd(connx, sql))
        {
            exec_cmd(connx, "ROLLBACK;");
            return false;
        }
    }

    if (!exec_cmd(connx, "COMMIT;"))
    {
        exec_cmd(connx, "ROLLBACK;");
        return false;
    }

    return true;
}
void Database::show()
{
    cout << "Status : " << (it_exists ? "Found & Fetchable." : "Not Found.") << "\n";
    cout << "Database name : " << db_name << "\n";
    PG_Config_Info::show();
}

User_Queries::User_Queries(PGconn *c) : conn(c) {}
void User_Queries::clear_result(PGresult *r)
{
    if (r)
        PQclear(r);
}
bool User_Queries::checkUniqueUsername(const string &username)
{
    if (!PG_Guard::ensure_conn(conn, "checkUniqueUsername"))
        return false;

    const char *sql =
        "SELECT 1 FROM client_personal_info WHERE username = $1 LIMIT 1;";
    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_tuples(conn, res, "checkUniqueUsername SELECT failed"))
        return false;

    bool isUnique = (PQntuples(res) == 0);
    PQclear(res);
    return isUnique;
}
void User_Queries::addUser(const UserAccount_server &user)
{
    if (!PG_Guard::ensure_conn(conn, "addUser"))
        return;

    PGresult *res = PQexec(conn, "BEGIN;");
    if (!PG_Guard::command_and_clear(conn, res, "BEGIN failed"))
        return;

    const char *sql1 =
        "INSERT INTO client_personal_info "
        "(client_ID, name, username, password, DOB, account_no, favAni, salt) "
        "VALUES ($1,$2,$3,$4,$5,$6,$7,$8);";

    Date DOB = user.getDOB();

    string _ID = user.getClientID().getHexadecimalVal();
    string name = user.getName();
    string username = user.getUsername();
    string password = user.getPassword();
    string _DOB = DOB.getDate_string();
    string accountNo = user.getAccountNo();
    string favAni = user.getFavAni();
    string salt = user.getSalt();

    const char *values1[8] = {

        _ID.c_str(),
        name.c_str(),
        username.c_str(),
        password.c_str(),
        _DOB.c_str(),
        accountNo.c_str(),
        favAni.c_str(),
        salt.c_str(),
    };

    res = PQexecParams(conn, sql1, 8, nullptr, values1, nullptr, nullptr, 0);
    if (!PG_Guard::expect_command(conn, res, "Insert personal info failed", true))
        return;
    PQclear(res);

    const char *sql2 = "INSERT INTO client_account_status (client_ID) VALUES ($1);";
    const char *values2[1] = {user.getClientID().getHexadecimalVal().c_str()};

    res = PQexecParams(conn, sql2, 1, nullptr, values2, nullptr, nullptr, 0);
    if (!PG_Guard::expect_command(conn, res, "Insert account status failed", true))
        return;
    PQclear(res);

    res = PQexec(conn, "COMMIT;");
    if (!PG_Guard::expect_command(conn, res, "COMMIT failed", true))
        return;
    PQclear(res);

    cout << "User successfully added.\n";
}
bool User_Queries::hasEnoughBalance(const string &username, double amount)
{
    if (!PG_Guard::ensure_conn(conn, "hasEnoughBalance"))
        return false;

    const char *sql =
        "SELECT cas.balance "
        "FROM client_account_status cas "
        "JOIN client_personal_info cpi "
        "ON cas.client_ID = cpi.client_ID "
        "WHERE cpi.username = $1;";

    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_tuples(conn, res, "hasEnoughBalance SELECT failed"))
        return false;

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return false;
    }

    double balance = atof(PQgetvalue(res, 0, 0));
    PQclear(res);
    return balance >= amount;
}
bool User_Queries::updateBalance(const string &username, double newBalance)
{
    if (!PG_Guard::ensure_conn(conn, "updateBalance"))
        return false;

    const char *sql =
        "UPDATE client_account_status cas "
        "SET balance = $1 "
        "FROM client_personal_info cpi "
        "WHERE cas.client_ID = cpi.client_ID "
        "AND cpi.username = $2;";

    string balStr = to_string(newBalance);
    const char *values[2] = {balStr.c_str(), username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 2, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_command(conn, res, "updateBalance UPDATE failed"))
        return false;

    PQclear(res);
    return true;
}

DatabaseUpdates::DatabaseUpdates(PGconn *connection) : conn(connection) {}
bool DatabaseUpdates::deleteUserByUsername(const string &username)
{
    if (!PG_Guard::ensure_conn(conn, "deleteUserByUsername"))
        return false;

    const char *sql =
        "DELETE FROM client_personal_info "
        "WHERE username = $1;";

    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_command(conn, res, "deleteUserByUsername DELETE failed"))
        return false;

    PQclear(res);
    return true;
}



UserAccount_server User_Queries::getUserAccount_server(const string &username)
{
    const char *sql = "SELECT name, username, password, DOB, account_no, favAni, balance "
                      "FROM client_personal_info c "
                      "JOIN client_account_status s ON c.client_ID = s.client_ID "
                      "WHERE username = $1;";

    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return UserAccount();
    }

    string _ID = PQgetvalue(res, 0, 0);
    string name = PQgetvalue(res, 0, 1);
    string usr = PQgetvalue(res, 0, 2);
    string pass = PQgetvalue(res, 0, 3);
    string dob = PQgetvalue(res, 0, 4);
    string acc = PQgetvalue(res, 0, 5);
    string fav = PQgetvalue(res, 0, 6);
    string salt = PQgetvalue(res, 0, 7);
    PQclear(res);

    Hexadecimal ID = Hexadecimal(_ID);
    Date DOB(dob);

    double balance = getBalance(username);

    UserAccount userAccount(name, usr, pass, DOB, balance, acc, fav);
    return UserAccount_server(ID, userAccount, salt);
}

double User_Queries::getBalance(const string &username)
{
    const char *sql =
        "SELECT cas.balance "
        "FROM client_account_status cas "
        "JOIN client_personal_info cpi "
        "ON cas.client_ID = cpi.client_ID "
        "WHERE cpi.username = $1;";

    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return -1;
    }

    double balance = atof(PQgetvalue(res, 0, 0));
    PQclear(res);

    return balance;
}

bool DatabaseUpdates::updatePasswordByUsername(const string &username, const string &newPassword, const string &newSalt)
{
    if (!PG_Guard::ensure_conn(conn, "updatePasswordByUsername"))
        return false;

    const char *sql =
        "UPDATE client_personal_info "
        "SET password = $1, salt = $2 "
        "WHERE username = $3;";

    const char *values[3] = {newPassword.c_str(), newSalt.c_str(), username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 3, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_command(conn, res, "updatePasswordByUsername UPDATE failed"))
        return false;

    PQclear(res);
    return true;
}
string DatabaseUpdates::getSaltByUsername(const string &username)
{
    if (!PG_Guard::ensure_conn(conn, "getSaltByUsername"))
        return "";

    const char *sql =
        "SELECT salt FROM client_personal_info WHERE username = $1 LIMIT 1;";
    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_tuples(conn, res, "getSaltByUsername SELECT failed"))
        return "";

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return "";
    }

    string salt = PQgetvalue(res, 0, 0);
    PQclear(res);
    return salt;
}
string DatabaseUpdates::getPasswordByUsername(const string &username)
{
    if (!PG_Guard::ensure_conn(conn, "getPasswordByUsername"))
        return "";

    const char *sql =
        "SELECT password FROM client_personal_info WHERE username = $1 LIMIT 1;";
    const char *values[1] = {username.c_str()};

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);
    if (!PG_Guard::expect_tuples(conn, res, "getPasswordByUsername SELECT failed"))
        return "";

    if (PQntuples(res) == 0)
    {
        PQclear(res);
        return "";
    }

    string password = PQgetvalue(res, 0, 0);
    PQclear(res);
    return password;
}
