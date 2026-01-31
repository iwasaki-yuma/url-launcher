#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <string>
#include <vector>
#include <sqlite3.h>

class SQLiteDB {
private:
    sqlite3* db;  // SQLiteデータベースハンドル（接続情報を保持）
    
public:
    // データベースを開く
    bool open(const std::string& dbPath);
    
    // テーブルを作成
    bool createTable();
    
    // INSERT
    bool insert(const std::string& cmd_name, const std::string& url);
    
    // SELECT
    std::vector<std::string> select(const std::string& cmd_name);
    
    // データベースを閉じる
    void close();
    
    // デストラクタ: オブジェクト破棄時に自動的にクローズ
    ~SQLiteDB();
};

#endif // SQLITEDB_H
