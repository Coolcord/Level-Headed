Level-Headed
============

Random Level Generator and Level Script Compiler for Super Mario Bros. 1 on the NES

[![GitHub release](https://img.shields.io/github/release/Coolcord/Level-Headed.svg)](https://GitHub.com/Coolcord/Level-Headed/releases)
[![Github all releases](https://img.shields.io/github/downloads/Coolcord/Level-Headed/total.svg)](https://GitHub.com/Coolcord/Level-Headed/releases)
[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)

# [Download for Windows](https://github.com/Coolcord/Level-Headed/releases/download/v0.3.6/Level-Headed.v0.3.6.Setup.exe)

## About Level-Headed

 Level-Headed is an application that is still under development. Ultimately, I am
 designing it to be a random level generator that will work across multiple platforming
 games. In its current state, Level-Headed can essentially create new Mario games based
 off of Super Mario Bros. 1 (SMB1) with a click of a button! Users can also create their
 own levels using the "SMB1_Compliance" scripting language. As of now, only SMB1 is
 supported, but in the future, support for more games will be added.
 
 In order for Level-Headed to work properly, a base ROM must be supplied to Level-Headed. You can read more about which ROMs are supported [here](https://github.com/Coolcord/Level-Headed/wiki/Supported-ROMs).
 
 Check out the [Level-Headed Wiki](https://github.com/Coolcord/Level-Headed/wiki) for more information and tutorials on how to use Level-Headed.
 
 If you're a Windows user, you can download the Windows build under the [releases](https://github.com/Coolcord/Level-Headed/releases) tab. Linux and Mac users should compile the application and plugins via the [Unix Build Script](https://github.com/Coolcord/Level-Headed/wiki/Compiling-Level%E2%80%90Headed-via-the-Unix-Build-Script). Alternatively, Level-Headed can be [compiled via Qt Creator](https://github.com/Coolcord/Level-Headed/wiki/Compiling-Level%E2%80%90Headed-via-Qt-Creator).
 
 If you have any questions about Level-Headed, feel free to contact me at coolcord24@tutanota.com

 ## FAQ

 **Q: What's going to be in the next update?**
 
 A: You can see what changes are going to be implemented by looking at the [changelog](https://raw.githubusercontent.com/Coolcord/Level-Headed/master/Doc/Changelog.txt).

 **Q: When are you going to add new patterns into the level generator? What about new level types?**
 
 A: Currently, levels still only have access to about 10 patterns per level type, which is why the levels can sometimes be a bit stale. The v0.4.x series of updates will address this by focusing on creating new patterns for existing levels as well as adding completely new level types all together. v0.4 is still a ways off though, as I intend to write a graphical pattern editor to easily make new patterns for the level generator. Development of this tool will need to be completed before development of v0.4 can start.
 
 **Q: How do I add my own graphics and music patches?**
 
 A: There's a tutorial on how to do that on the wiki [here](https://github.com/Coolcord/Level-Headed/wiki/How-to-Add-Your-Own-Graphics-and-Music-Into-Level%E2%80%90Headed).
 
 **Q: I set a level type to uncommon, but I got it multiple times in a row! Is this a bug?**
 
 A: Technically, no. For now, when the level generator goes to generate a new level, it does not care what kind of level came before or what will come after. All it cares about is the percentage chance that it calculates based upon the weights the user provides. Think of this like rolling a dice. There's nothing stopping you from rolling four 1's in a row, though it's unlikely to happen. You can read more about commonalities [here](https://github.com/Coolcord/Level-Headed/wiki/How-Level%E2%80%90Headed-Works#commonalities). I may rework this system in the future, as many people seem to expect it to work differently. For now, just try dropping what you don't like to "Rare" as that'll make a difference.
 
 **Q: There's no variety!**
 
 A: Bear in mind that Level-Headed is only v0.3, so it's only a fraction of what I have planned. Think of current releases as a preview of what's to come. With each update, this will become less and less of a problem.
 
 **Q: When will you support X game?**
 
 A: Level-Headed is being designed from the ground up with multi-game support in mind, but don't expect to see support for another game until after v1.0. That's a very, very long ways off.
 
 **Q: Is Level-Headed dead? You haven't updated it in months!**
 
 A: No, it is far from dead. I tend to work on Level-Headed off and on when I have spare time and when
 I have the interest. Consequently, development tends to be very slow and sporadic. However, I can't see
 myself ever dropping this project completely.
 
