// #include "../../shared/classes/Date.hpp"
// #include "../../shared/classes/UserAccount.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <postgresql/libpq-fe.h>

using namespace std;

class pgconfig_info {
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
            // ✅ FIX: use `db`, not `admin_db` (otherwise dbname arg is ignored)
            return "host=" + host + " port=" + port + " dbname=" + db +
                   " user=" + user + " password=" + password;
        };

        PGconn* conn = PQconnectdb(conninfo().c_str());

        // ✅ FIX: guard conn==nullptr before PQerrorMessage/PQfinish
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
};

class database : public pgconfig_info {
private:
    PGconn* connx;
    bool it_exists;
    string db_name;

public:
    database(PGconn* connx_in = nullptr, bool status = false, string name = "")
        : connx(connx_in),
          // ✅ FIX: if name not provided, use configured target DB
          db_name(name.empty() ? get_target_db() : name),
          pgconfig_info(),
          it_exists(status)
    {
        // If no connection passed in (or it's bad), do bootstrap
        if (!connx || PQstatus(connx) != CONNECTION_OK) {

            // 1) connect to admin DB for existence/create
            connx = connect(get_admin_db());
            if (!connx) {
                it_exists = false;
                return;
            }

            // 2) if DB does NOT exist, create it
            if (!db_exists(connx, db_name)) {
                bool made = create_db(connx, db_name);
                if (made) {
                    cout << "Database has been made\n";
                    it_exists = true;
                } else {
                    cout << "Create Database failed\n";
                    it_exists = false;
                    // keep admin connection around only until destructor
                    return;
                }
            } else {
                // ✅ FIX: if it already exists, this should be true
                it_exists = true;
            }

            // 3) IMPORTANT: switch from admin DB to target DB connection
            PQfinish(connx);
            connx = connect(db_name);
            if (!connx) {
                it_exists = false;
                return;
            }

        } else {
            // ✅ FIX: valid incoming connection; treat as exists/ok
            it_exists = true;
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

        // ✅ FIX: guard r==nullptr
        if (!r) {
            cerr << "DB existence check failed: PQexecParams returned nullptr\n";
            return false;
        }

        if (PQresultStatus(r) != PGRES_TUPLES_OK) {
            cerr << "DB existence check failed:\n" << PQerrorMessage(c);
            PQclear(r);
            return false; // treat as not-exists/unknown
        }

        bool exists = PQntuples(r) > 0;
        PQclear(r);
        return exists;
    }

    static bool create_db(PGconn* c, const string& dbname) {
        if (!c) return false;

        // dbname is an identifier; escape properly
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
};

int main() {
    // Smoke test
    database db;
    if (db.get_conn() && db.exists()) {
        cout << "Connected to target DB '" << "' successfully.\n";
    } else if (db.get_conn()) {
        cout << "Connected, but existence flag is false (unexpected).\n";
    } else {
        cout << "Failed to connect.\n";
    }

    return 0;
}
