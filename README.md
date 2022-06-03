![UNDETECTED](https://img.shields.io/badge/UNDETECTED-5aca4f?style=for-the-badge)
![Downloads](https://img.shields.io/github/downloads/Aivaki/Mem/total?color=lightgray&style=for-the-badge)
[![BuyMeACoffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-ffdd00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://www.buymeacoffee.com/aivaki)

# Mem - aimbot for CS:GO
> Mem is a `C++/CLI` project with a Windows Forms user interface:

![Mem](https://user-images.githubusercontent.com/58384167/169660067-b95f3630-bc38-444e-91f7-676fb4a124dd.png)

### Features
> The main function is an easy-to-use aimbot

> - The cheat will only start when `CS:GO` is running
> - Automatic hiding of the cheat window (to open, you need to use `Alt + Tab`)
> - Automatic update of offsets on every launch
> - Own extension after first run (**.mem**)
> - Checking for walls based on radar - if someone sees the enemy, then the aimbot works
>> Initially there was a ray tracing based check, but it   added a bit of internal stuff, and at some point it stopped working, so it had to be abandoned in favor of a simpler

- Simple and clear interface with additional data
- Clearing the last activity in the system
- Automatic saving of your settings in the registry
- Changing the name of the file at each launch
- Changing the name of the window each time you interact with it
- High performance and stability (**works well on a weak PC**)
- If there is more than one person in the game with a cheat, then it will only work for the one who appeared on the map faster

  And other features

### More nice things
> In order to better understand the project and not only

- Clear and documented code
- Friendly variables names
- Good implementation of working with memory
- Class for generating a completely random string
- Virtually no warnings at the build stage

  And much more 

# Instruction
> **Provided for beginners**, but you need to know the basics of working in Visual Studio

### **1.** If you don't have this installed, then you need to download and install [DirectX Software Development Kit](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
![Site](https://user-images.githubusercontent.com/58384167/169665762-a241de83-89d1-4ee2-8d6c-6412e52bdd3e.png)

### **2.** Get the project in any convenient way, and then go to `Project properties -> VC++ directories`. Paste this "`$(DXSDK_DIR)Include;$(IncludePath)`" in `Include directories`
### Now paste "`$(DXSDK_DIR)Lib\x86;$(VC_LibraryPath_x86);$(WindowsSDK_LibraryPath_x86)`" into `Library directories`

### **3.** Download [Curl](https://drive.google.com/file/d/1DMKF4cnUGeL2gpnLWZIpI7UyxbFCyonk/view?usp=sharing) and extract to a convenient location

### **4.** Go to `Project properties -> C/C++ -> Additional include directories`. Select the "`include`" folder in "`libcurl-vc-x86-release-static-ipv6-sspi-winssl`"
### Now go to `Project properties -> Linker -> Additional library directories`. Select the "`lib`" folder in "`libcurl-vc-x86-release-static-ipv6-sspi-winssl`"

### **5.** Go to `Linker -> Input`. And paste this "`libcurl_a.lib;Ws2_32.lib;Crypt32.lib;Wldap32.lib;Normaliz.lib;%(AdditionalDependencies)`" in `Additional dependencies`

### **6.** Done! Now you can build the project on the configuration of the solution "`Release`" and the platform "`x86`"

## Comment
> Feel free to correct me. I will accept all criticism, fix the code and more!
###### I couldn't find a reference when I first started. But in the end I found it. You see this project thanks to [ClemX](https://github.com/clem45/ClemX). I am grateful to its creator, because without him now there would be nothing
