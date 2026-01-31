# URL Launcher

SQLite に名前付きの URL を保存し、ブラウザで開くシンプルな CLI ツールです。

## ビルド

前提 (Ubuntu):

```
sudo apt update
sudo apt install -y cmake g++ libsqlite3-dev
```

ビルド:

```
mkdir -p build
cd build
cmake ..
cmake --build .
```

実行ファイルは `build/mytool` です。

## 使い方

名前に URL を登録:

```
./mytool add <name> <url1> [url2 ...]
```

登録済みの URL をまとめて開く:

```
./mytool open <name>
```

## 補足

- SQLite の DB ファイル `urls.db` は実行したディレクトリに作成されます。
- 起動時は `google-chrome` → `chromium` → `xdg-open` の順で試します。
