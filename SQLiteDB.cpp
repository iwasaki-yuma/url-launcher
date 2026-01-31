#include "SQLiteDB.h"
#include <iostream>
#include <vector>

bool SQLiteDB::open(const std::string& dbPath) {
    // sqlite3_open: データベースファイルを開く（存在しない場合は新規作成）
    // 引数1: データベースファイルのパス
    // 引数2: データベースハンドルへのポインタ（接続情報がここに格納される）
    // 戻り値: SQLITE_OK(0)なら成功、それ以外はエラー
    int rc = sqlite3_open(dbPath.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        // sqlite3_errmsg: 最後に発生したエラーメッセージを取得
        std::cerr << "データベースを開けません: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    const char* dbFile = sqlite3_db_filename(db, "main");
    std::cout << "DB path: " << (dbFile ? dbFile : "(null)") << std::endl;
    std::cout << "データベースに接続しました: " << dbPath << std::endl;
    return true;
}

bool SQLiteDB::createTable() {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS urls ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "cmd_name TEXT NOT NULL,"
        "url TEXT NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)" ;
    
    char* errMsg = nullptr;
    
    // sqlite3_exec: SQLを実行する（シンプルなSQL向け）
    // 引数1: データベースハンドル
    // 引数2: 実行するSQL文
    // 引数3: コールバック関数（結果を処理する関数、不要ならnullptr）
    // 引数4: コールバック関数に渡す引数
    // 引数5: エラーメッセージを格納するポインタ
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "テーブル作成エラー: " << errMsg << std::endl;
        sqlite3_free(errMsg);  // エラーメッセージのメモリを解放
        return false;
    }
    std::cout << "テーブルを作成しました" << std::endl;
    return true;
}

bool SQLiteDB::insert(const std::string& cmd_name, const std::string& url) {
    const std::string sql = "INSERT INTO urls (cmd_name, url) VALUES (?, ?);";
    sqlite3_stmt* stmt;  // プリペアドステートメント（SQLの実行準備済みオブジェクト）
    
    // sqlite3_prepare_v2: SQLをコンパイルして実行準備をする
    // ?（プレースホルダ）を使うことでSQLインジェクション対策になる
    // 引数1: データベースハンドル
    // 引数2: SQL文
    // 引数3: SQL文のバイト数（-1なら自動計算）
    // 引数4: プリペアドステートメントへのポインタ
    // 引数5: 未使用のSQL文の残り（通常nullptr）
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL準備エラー: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // sqlite3_bind_text: プレースホルダ(?)に値をバインド（設定）する
    // 引数1: プリペアドステートメント
    // 引数2: プレースホルダの位置（1始まり）
    // 引数3: バインドする文字列
    // 引数4: 文字列の長さ（-1なら自動計算）
    // 引数5: メモリ管理方法（SQLITE_TRANSIENTはコピーを作成）
    sqlite3_bind_text(stmt, 1, cmd_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, url.c_str(), -1, SQLITE_TRANSIENT);
    
    // sqlite3_step: SQLを実行する
    // 戻り値: SQLITE_DONE（完了）、SQLITE_ROW（結果行あり）など
    rc = sqlite3_step(stmt);
    
    // sqlite3_finalize: プリペアドステートメントを解放（メモリリーク防止）
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "INSERT エラー: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    std::cout << "コマンドを登録しました " << url << std::endl;
    return true;
}

std::vector<std::string> SQLiteDB::select(const std::string& cmd_name) {
    const char* sql = "SELECT url FROM urls WHERE cmd_name = ?;";
    std::vector<std::string> results; //selectの結果(URL)を格納する変数
    sqlite3_stmt* stmt;  // プリペアドステートメント（SQLの実行準備済みオブジェクト）
    std::cout << "cmd_name: [" << cmd_name << "] len=" << cmd_name.size() << std::endl;
    
    // SELECTもprepareで準備
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SELECT準備エラー: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    // プレースホルダに値をバインド
    sqlite3_bind_text(stmt, 1, cmd_name.c_str(), -1, SQLITE_TRANSIENT);
    
    std::cout << "\n=== 登録されているURL ===" << std::endl;
    
    // sqlite3_step: 1行ずつ結果を取得
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            results.emplace_back(reinterpret_cast<const char*>(text));
        } else {
            results.emplace_back("");
        }
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "SELECT エラー: " << sqlite3_errmsg(db) << std::endl;
    }
    
    sqlite3_finalize(stmt);
    return results;
}

void SQLiteDB::close() {
    if (db) {
        // sqlite3_close: データベース接続を閉じる
        // 必ず呼び出してリソースを解放する
        sqlite3_close(db);
        std::cout << "データベースを閉じました" << std::endl;
    }
}

SQLiteDB::~SQLiteDB() {
    close();
}
