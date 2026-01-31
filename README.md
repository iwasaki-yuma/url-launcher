# URL Launcher

Simple CLI tool that stores named URL shortcuts in SQLite and opens them in a browser.

## Build

Prereqs (Ubuntu):

```
sudo apt update
sudo apt install -y cmake g++ libsqlite3-dev
```

Build:

```
mkdir -p build
cd build
cmake ..
cmake --build .
```

The binary is `build/mytool`.

## Usage

Add one or more URLs to a name:

```
./mytool add <name> <url1> [url2 ...]
```

Open all URLs stored for a name:

```
./mytool open <name>
```

## Notes

- The SQLite database file `urls.db` is created in the working directory.
- The launcher tries `google-chrome`, then `chromium`, then `xdg-open`.
