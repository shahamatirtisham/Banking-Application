#include "../../shared/classes/Date.hpp"
#include "../../shared/classes/UserAccount.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <postgresql/libpq-fe.h>

using namespace std;





static void fatal(PGconn* c, const string& msg) {

    // ERROR REDIRECTOR [IF ERRORS OCCUR THIS FUNCTIONS RETURNS ERRORS]

    cerr << msg << "\n";
    if (c) cerr << PQerrorMessage(c);
    if (c) PQfinish(c);
    exit(1);
}

static bool exec_cmd(PGconn* c, const string& sql) {

    //SQL executor [This one executes & returns false in case of error]

    
    PGresult* r = PQexec(c, sql.c_str());
    auto st = PQresultStatus(r);
    bool ok = (st == PGRES_COMMAND_OK || st == PGRES_TUPLES_OK);
    if (!ok) {
        cerr << "SQL failed:\n" << sql << "\n";
        cerr << PQerrorMessage(c);
    }
    PQclear(r);
    return ok;
}

static bool db_exists(PGconn* c, const string& dbname) {
    const char* sql = "SELECT 1 FROM pg_database WHERE datname = $1;";
    const char* vals[1] = { dbname.c_str() };

    PGresult* r = PQexecParams(c, sql, 1, nullptr, vals, nullptr, nullptr, 0);
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
    // dbname is an identifier; escape properly
    char* q = PQescapeIdentifier(c, dbname.c_str(), dbname.size());
    if (!q) return false;

    string sql = string("CREATE DATABASE ") + q + ";";
    PQfreemem(q);

    return exec_cmd(c, sql);
}

static PGconn* connect_checked(const string& conninfo) {
    PGconn* c = PQconnectdb(conninfo.c_str());
    return c;
}




PGconn* db_initialise(
    const string& host = "localhost",
    const string& port = "5432",
    const string& user = "myuser",
    const string& password = "mypass",
    const string& target_db = "bankdb",
    const string& admin_db = "postgres"
) {

    auto conninfo = [&](const string& db) {
        return "host=" + host + " port=" + port + " dbname=" + db +
               " user=" + user + " password=" + password;
    };

    // 1: Ensure if database exists (using admin DB) ---
    {
        PGconn* admin = connect_checked(conninfo(admin_db));
        if (PQstatus(admin) != CONNECTION_OK) {
            fatal(admin, "Could not connect to admin DB (is Postgres running? creds ok?).");
        }

        if (!db_exists(admin, target_db)) {
            if (!create_db(admin, target_db)) {
                fatal(admin, "CREATE DATABASE failed (does user have CREATEDB privilege?).");
            }
        }

        PQfinish(admin);
    }

    // --- Step 2: Connect to target DB ---
    
    PGconn* db = connect_checked(conninfo(target_db));
    
    if (PQstatus(db) != CONNECTION_OK) {
        fatal(db, "Could not connect to target DB even after ensuring it exists.");
    }

    // --- Step 3: Create tables / constraints / seed data ---
    // Wrap in a transaction so partial init doesn't leave a mess.
    if (!exec_cmd(db, "BEGIN;")) {
        fatal(db, "Failed to BEGIN transaction for init.");
    }

    // Example banking schema (edit as you like)
    const vector<string> ddl = {
        R"SQL(
        CREATE TABLE IF NOT EXISTS accounts (
            id          BIGSERIAL PRIMARY KEY,
            name        TEXT NOT NULL,
            balance     NUMERIC(18,2) NOT NULL DEFAULT 0,
            created_at  TIMESTAMPTZ NOT NULL DEFAULT now(),
            CONSTRAINT balance_nonnegative CHECK (balance >= 0)
        );
        )SQL",

        R"SQL(
        CREATE TABLE IF NOT EXISTS transactions (
            id          BIGSERIAL PRIMARY KEY,
            account_id  BIGINT NOT NULL REFERENCES accounts(id) ON DELETE CASCADE,
            amount      NUMERIC(18,2) NOT NULL,
            kind        TEXT NOT NULL CHECK (kind IN ('deposit','withdraw')),
            created_at  TIMESTAMPTZ NOT NULL DEFAULT now()
        );
        )SQL",

        // optional: a schema version table (useful as you grow)
        R"SQL(
        CREATE TABLE IF NOT EXISTS schema_version (
            version INT PRIMARY KEY,
            applied_at TIMESTAMPTZ NOT NULL DEFAULT now()
        );
        )SQL"
    };

    for (const auto& sql : ddl) {
        if (!exec_cmd(db, sql)) {
            exec_cmd(db, "ROLLBACK;");
            fatal(db, "Schema init failed; rolled back.");
        }
    }

    // Example seed (optional)
    // This is safe-ish because of ON CONFLICT.
    if (!exec_cmd(db, R"SQL(
        INSERT INTO schema_version(version)
        VALUES (1)
        ON CONFLICT (version) DO NOTHING;
    )SQL")) {
        exec_cmd(db, "ROLLBACK;");
        fatal(db, "Seeding failed; rolled back.");
    }

    if (!exec_cmd(db, "COMMIT;")) {
        exec_cmd(db, "ROLLBACK;");
        fatal(db, "Failed to COMMIT init transaction.");
    }

    return db; // ready-to-use connection
}

int main() {
    PGconn* conn = db_initialise(
        "localhost", "5432",
        "myuser", "mypassword",
        "bankdb"
    );

    cout << "DB initialised and connected.\n";
    PQfinish(conn);
    return 0;
}
