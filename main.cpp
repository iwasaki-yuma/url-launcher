#include <iostream>
#include <cstring>
#include <vector>
#include "SQLiteDB.h"
#include "URLLauncher.h"

int main(int argc, char* argv[]) {
    if(argc < 2){
        std::cerr << "引数が不足しています。"<<std::endl;
        return 1;
    }

    SQLiteDB db;
    URLLauncher launcher;
    
    // データベースを開く（ファイル名を指定）
    if (!db.open("urls.db")) {
        return 1;
    }

    //ここから本処理
     if(argc ==3 && strcmp(argv[1], "open") == 0){
        // なければテーブル作成、あるならNOT EXISTSで無視
        db.createTable();
        //コマンドを実行する処理。argv[2]にコマンド名が入っており、それに対応するURLを取得してブラウザで開く
        // URLを取得
        std::vector<std::string> urls = db.select(argv[2]);
        if (urls.empty()) {
            std::cerr << "該当するURLが見つかりませんでした。" << std::endl;
            return 1;
        }
        for (const auto& url : urls) {
            std::cout << url << " を開きます" << std::endl;
            launcher.openURL(url);
        }
        // close()は~SQLiteDB()で自動的に呼ばれる

        return 0;
    }else if(argc >= 3 && strcmp(argv[1], "add") == 0){
        // なければテーブル作成、あるならNOT EXISTSで無視
        db.createTable();

        //URLが複数あるときのコマンド。argv[2]にコマンド名、argv[3]以降にURLが入っている
        for(int i = 3; i < argc ; i++){
            db.insert(argv[2], argv[i]);
        }
        return 0;
    }else{
        std::cerr << "不正な引数です。" <<std::endl;
        return 1;
    }
    
    return 0;
}
