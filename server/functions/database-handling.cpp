 #include "../../shared/classes/Date.hpp"
 #include "../classes/UserAccount_server.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <postgresql/libpq-fe.h>

using namespace std;

class overall_con_db{

public:
    virtual void show()=0;
    virtual ~overall_con_db() = default;
};



class pgconfig_info{
private:
    string host;
    string port;
    string user;
    string password;
    string target_db;
    string admin_db;


public:
    pgconfig_info(
        string host_in = "localhost",
        string port_in = "5432",
        string user_in = "myuser",
        string password_in = "mypass",
        string target_db_in = "bankdb",
        string admin_db_in = "postgres"
    )
        : host(host_in),
          port(port_in),
          user(user_in),
          password(password_in),
          target_db(target_db_in),
          admin_db(admin_db_in) {}

    // Connect to a specified DB; if none provided, connect to admin_db
    
    PGconn* connect(const string& dbname = "") {
        const string& db = dbname.empty() ? admin_db : dbname;

        auto conninfo = [&]() {
            return "host=" + host + " port=" + port + " dbname=" + db +
                   " user=" + user + " password=" + password;
        };

        PGconn* conn = PQconnectdb(conninfo().c_str());

        if (!conn || PQstatus(conn) != CONNECTION_OK) {
            cerr << (conn ? PQerrorMessage(conn) : "PQconnectdb returned nullptr") << "\n";
            cout << "Connection Failed\n";
            if (conn) PQfinish(conn);
            return nullptr;
        }

        return conn;
    }

    const string& get_target_db() const { return target_db; }
    const string& get_admin_db() const { return admin_db; }


    void show(){
        cout << "host: " << host << endl;
        cout << "port: " << port << endl;
        cout << "user: " << user << endl;
        cout << "target db : " << target_db << endl;
        cout << "admin db : " << admin_db << endl;
    }

};

class database : public pgconfig_info,public overall_con_db{
private:

    PGconn* connx;
    bool it_exists;
    string db_name;

public:
    database(PGconn* connx_in = nullptr, bool status = false, string name = "")
        : connx(connx_in),
          db_name(name.empty() ? get_target_db() : name),
          pgconfig_info(),
          it_exists(status)
    {
        // If no connection passed in (or it's bad), do bootstrap
        if (!connx || PQstatus(connx) != CONNECTION_OK) {

            // 1) connect to admin DB for existence/create
            PGconn* admin = connect(get_admin_db());
            if (!admin) {
                it_exists = false;
                connx = nullptr;
                return;
            }

            // 2) if DB does NOT exist, create it
            if (!db_exists(admin, db_name)) {
                bool made = create_db(admin, db_name);
                if (made) {
                    cout << "Database has been made\n";
                    it_exists = true;
                } else {
                    cout << "Create Database failed\n";
                    it_exists = false;
                    PQfinish(admin);
                    connx = nullptr;
                    return;
                }
            } else {
                it_exists = true; // already existed
            }

            // 3) close admin connection
            PQfinish(admin);

            // 4) connect to target DB
            connx = connect(db_name);
            if (!connx) {
                it_exists = false;
                return;
            }

            // 5) initialize schema ONLY on target DB
            if (!init_schema()) {
                cerr << "Schema init failed\n";
                it_exists = false;
                return;
            }

        } else {
            // Incoming connection is valid; assume it targets the right DB
            it_exists = true;

            // Initialize schema on that connection
            if (!init_schema()) {
                cerr << "Schema init failed\n";
                it_exists = false;
                return;
            }
        }
    }

    ~database() {
        if (connx) PQfinish(connx);
        connx = nullptr;
    }

    PGconn* get_conn() const { return connx; }
    bool exists() const { return it_exists; }

    static bool db_exists(PGconn* c, const string& dbname) {
        if (!c) return false;

        const char* sql = "SELECT 1 FROM pg_database WHERE datname = $1;";
        const char* vals[1] = { dbname.c_str() };

        PGresult* r = PQexecParams(c, sql, 1, nullptr, vals, nullptr, nullptr, 0);
        if (!r) {
            cerr << "DB existence check failed: PQexecParams returned nullptr\n";
            return false;
        }

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
            cerr << "DB existence check failed:\n" << PQerrorMessage(c);
            PQclear(r);
            return false;
        }

        bool exists = PQntuples(r) > 0;
        PQclear(r);
        return exists;
    }

    static bool create_db(PGconn* c, const string& dbname) {
        if (!c) return false;

        char* q = PQescapeIdentifier(c, dbname.c_str(), dbname.size());
        if (!q) return false;

        string sql = string("CREATE DATABASE ") + q + ";";
        PQfreemem(q);

        return exec_cmd(c, sql);
    }

    static bool exec_cmd(PGconn* c, const string& sql) {
        if (!c) return false;

        PGresult* r = PQexec(c, sql.c_str());
        if (!r) {
            cerr << "PQexec returned nullptr\n";
            return false;
        }

        auto st = PQresultStatus(r);
        bool ok = (st == PGRES_COMMAND_OK || st == PGRES_TUPLES_OK);
        if (!ok) {
            cerr << "SQL failed:\n" << sql << "\n";
            cerr << PQerrorMessage(c);
        }
        PQclear(r);
        return ok;
    }

    // Schema initialization
bool init_schema() {
    if (!connx || PQstatus(connx) != CONNECTION_OK) return false;

    if (!exec_cmd(connx, "BEGIN;")) return false;

    const vector<string> ddl = {

        // ---------- CLIENT TABLES ----------
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

        // ---------- CLIENT ACTIVITY ----------
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

        // ---------- TRANSACTIONS ----------
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

        // ---------- ADMIN ----------
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

        // Keeping admin_id (otherwise you can't tie activity to an admin)
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

        // ---------- INDEXES ----------
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

    void show(){
        cout << "Status : ";
        
        if(it_exists){
            cout << "Found & Fetchable." << endl;
        }
        else{
            cout << "Not Found."<< endl;
        }

        cout << "Database name : " << db_name << endl;

        pgconfig_info:: show();

    }



};


class User_Queries{

private:
    PGconn* conn;

    static void clear_result(PGresult*r){
        if(r){
            PQclear(r);
        }
    }

public:

    User_Queries(PGconn *c): conn(c){}

bool checkUniqueUsername(const std::string& username)
{
    if (!conn || PQstatus(conn) != CONNECTION_OK)
        return false;

    const char* sql =
        "SELECT 1 FROM client_personal_info WHERE username = $1 LIMIT 1;";

    const char* values[1] = { username.c_str() };

    PGresult* res = PQexecParams(
        conn,
        sql,
        1,          
        nullptr,    
        values,
        nullptr,
        nullptr,
        0          
    );

    if (!res)
    {
        std::cerr << "Username check failed: PQexecParams returned nullptr\n";
        return false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::cerr << "Username check failed:\n"
                  << PQerrorMessage(conn);
        PQclear(res);
        return false;
    }

    // If any row exists → username already taken
    bool isUnique = (PQntuples(res) == 0);

    PQclear(res);
    return isUnique;
}

void addUser(const UserAccount_server& user)
{
    if (!conn || PQstatus(conn) != CONNECTION_OK) {
        cerr << "Invalid database connection.\n";
        return;
    }

    if (!checkUniqueUsername(user.getUsername())) {
        cerr << "Username already exists.\n";
        return;
    }

    PGresult* res = PQexec(conn, "BEGIN;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to begin transaction.\n";
        PQclear(res);
        return;
    }
    PQclear(res);

    const char* sql1 =
        "INSERT INTO client_personal_info "
        "(client_ID, name, username, password, DOB, account_no, favAni, salt) "
        "VALUES ($1,$2,$3,$4,$5,$6,$7,$8);";

    Date DOB = user.getDOB();
    string Date_str = to_string(DOB.getDate()) + "/" +
                       to_string(DOB.getMonth()) + "/" +
                       to_string(DOB.getYear());


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
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Insert personal info failed:\n" << PQerrorMessage(conn);
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }
    PQclear(res);

    const char* sql2 = "INSERT INTO client_account_status (client_ID) VALUES ($1);";
    const char* values2[1] = { user.getClientID().getHexadecimalVal().c_str() };
    res = PQexecParams(conn, sql2, 1, nullptr, values2, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Insert account status failed:\n" << PQerrorMessage(conn);
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }
    PQclear(res);

    res = PQexec(conn, "COMMIT;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Commit failed.\n";
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }
    PQclear(res);

    cout << "User successfully added.\n";
}



    bool hasEnoughBalance(const std::string& username, double amount)
    {
        if (!conn || PQstatus(conn) != CONNECTION_OK)
            return false;

        const char* sql =
            "SELECT cas.balance "
            "FROM client_account_status cas "
            "JOIN client_personal_info cpi "
            "ON cas.client_ID = cpi.client_ID "
            "WHERE cpi.username = $1;";

        const char* values[1] = { username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 1, nullptr, values, nullptr, nullptr, 0);

        if (!res || PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
            clear_result(res);
            return false;
        }

        double balance = std::atof(PQgetvalue(res, 0, 0));
        clear_result(res);

        return balance >= amount;
    }

    bool updateBalance(const std::string& username, double newBalance)
    {
        if (!conn || PQstatus(conn) != CONNECTION_OK)
            return false;

        const char* sql =
            "UPDATE client_account_status cas "
            "SET balance = $1 "
            "FROM client_personal_info cpi "
            "WHERE cas.client_ID = cpi.client_ID "
            "AND cpi.username = $2;";

        std::string balStr = std::to_string(newBalance);
        const char* values[2] = { balStr.c_str(), username.c_str() };

        PGresult* res = PQexecParams(conn, sql, 2, nullptr, values, nullptr, nullptr, 0);

        bool ok = res && PQresultStatus(res) == PGRES_COMMAND_OK;
        clear_result(res);

        return ok;
    }


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
};






int main() {
    database db;

    if (db.get_conn() && db.exists()) {
        cout << "Connected and schema ensured.\n";
    } else {
        cout << "Failed to connect or initialize schema.\n";
    }

    return 0;
}
