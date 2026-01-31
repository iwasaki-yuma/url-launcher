cmake_minimum_required(VERSION 3.10)

# プロジェクト名
project(url-launcher)

# C++17を使用
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 実行ファイルを作成
add_executable(mytool
    main.cpp
    SQLiteDB.cpp
    URLLauncher.cpp
)

# SQLite3ライブラリをリンク
target_link_libraries(mytool sqlite3)
