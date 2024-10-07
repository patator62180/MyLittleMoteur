## How to debug

# Requirements

* Install [CodeLLDB extension](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb)
* [Enable Developer mode on Windows](https://learn.microsoft.com/en-us/gaming/game-bar/guide/developer-mode)

# Fix sourcemaps for breakpoints

* Run the engine once using F5
* Open the Debug Console in VSCode
* Place a breakpoint somewhere in the code
* You should see something like this in the Debug Console:

`Could not resolve any locations for breakpoint at c:\Users\david\projects\MyLittleMoteur\engine\Main.cpp:235, but found a valid location at C:\tools\msys64\home\david\_bazel_david\vqpfikda\execroot\_main\engine\Main.cpp:235`

* We're going to use that information to set it for the sourcemap, so keep this error visible
* Open VSCode settings
* Search for `lldb.launch.sourceMap`
* Click Add Item. Following the example above, the key would be `C:\tools\msys64\home\david\_bazel_david\vqpfikda\execroot\_main` and the value `c:\Users\david\projects\MyLittleMoteur` (Warning: this is case sensitive, keep the lowercase 'c:\' in the value). This will tell the debugger what to replace for the root of the project.
