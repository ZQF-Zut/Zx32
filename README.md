# Wx32
a win32 api wrapper using utf-8 and modern c++

## Build
- xmake
```shell
> git clone --depth 1 https://github.com/Dir-A/Wx32.git
> cd Wx32

-- build static --
> xmake config -k static
> xmake build -y
> xmake install -o out_dir_static wx32

-- build shared --
> xmake config -k shared
> xmake build -y
> xmake install -o out_dir_shared wx32
```

## Quick Start
- xmake
```shell
> xmake create -l c++ -t console hello_wx32
```

modify `hello_wx32/xmake.lua`
```lua
-- add
add_repositories("Drepo https://github.com/Dir-A/Drepo.git")
add_requires("wx32")
set_languages("c++20")
set_encodings("utf-8") -- msvc: /utf-8

add_rules("mode.debug", "mode.release")

target("hello_wx32")
    -- add
    add_packages("wx32")
    set_kind("binary")
    add_files("src/*.cpp")
```
modify `hello_wx32/src/main.cpp`
```c++
#include <iostream>
// add
#include <Wx32/APIs.h>

using namespace std;

int main(int argc, char** argv)
{
    // add
    Wx32::API::MessageBoxU8(nullptr, "wx32😋", "hello🥳", MB_OK);
    cout << "hello world!" << endl;
    return 0;
}
```
build and run
```shell
> xmake build -y
> xmake run
```
