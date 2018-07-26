Level-Headed
============

Random Level Generator and Level Script Compiler for Super Mario Bros. 1 on the NES

[![GitHub release](https://img.shields.io/github/release/Coolcord/Level-Headed.svg)](https://GitHub.com/Coolcord/Level-Headed/releases)
[![Github all releases](https://img.shields.io/github/downloads/Coolcord/Level-Headed/total.svg)](https://GitHub.com/Coolcord/Level-Headed/releases)
[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)

# [Download v0.2.4 Stable for Windows](https://github.com/Coolcord/Level-Headed/releases/download/v0.2.4/Level-Headed.v0.2.4.zip)

## The Hunt for Bugs is on in v0.3.0 Release Candidate 2!
If you're interested in testing the next update of Level-Headed, you can join in and download the public beta update [here](https://github.com/Coolcord/Level-Headed/releases). Be prepared for bugs and unbalanced gameplay (yes, even more than usual!). As usual, feedback is appreciated and encouraged!

## About Level-Headed

 Level-Headed is an application that is still under development. Ultimately, I am
 designing it to be a random level generator that will work across multiple platforming
 games. In its current state, Level-Headed can essentially create new Mario games based
 off of Super Mario Bros. 1 (SMB1) with a click of a button! Users can also create their
 own levels using the "SMB1_Compliance" scripting language. As of now, only SMB1 is
 supported, but in the future, support for more games will be added.
 
 In order for Level-Headed to work properly, a base ROM must be supplied to Level-Headed.
 The following 2 ROMs are supported:
 * Super Mario Bros. (JU) (PRG0) [!].nes (MD5: 811B027EAF99C2DEF7B933C5208636DE)
 * Super Mario Bros. (JU) (PRG1).nes (MD5: 94EDE9347C1416105F1C08EC26B5B73A)
 
 Check out the [Level-Headed Wiki](https://github.com/Coolcord/Level-Headed/wiki) for more information and tutorials on how to use Level-Headed.
 
 If you're a Windows user, you can download the Windows build under the [releases](https://github.com/Coolcord/Level-Headed/releases) tab. Linux and Mac users should compile the application and plugins through Qt Creator.
 
 If you have any questions about Level-Headed, feel free to contact me at coolcord24@tutanota.com

 ## FAQ
 
 **Q: What's new in Bullet Time (v0.3.0)?**
 
 A: The Bullet Time update mostly focuses on enemies and difficulty settings. Expect to see Lakitus, Bullet Bill Cannons, as well as a few other surprises. Difficulty settings currently only affect enemies and gameplay settings, but there is now a lot more options than there were in v0.2.4. A few minor ASM hacks (some made by me and some by the community) have been added to make the update more interesting, such as auto scrolling levels, Fire Bros., multiple Spiny Egg behaviors, and new powerups. The music patcher can now combine some music packs, as well as apply tone colors over them for even more variations. The level generator had a few bug fixes in some patterns, but beyond that, the level designs in v0.3.0 will remain very similar to v0.2.x. On the backend, [Sequential Archives](https://github.com/Coolcord/Sequential_Archive) have been implemented allowing for external graphics and music patches via [Hexagon Patches](https://github.com/Coolcord/Hexagon). This update is a major step, considering that many pieces of this programmatic puzzle are starting to come together.
 
 **Q: I see that you have included graphics and music patches. Can you include mine in the next update?**
 
 A: Sure, I'll consider it! The graphics and music help make each game feel slightly more unique. Over time,
 I'd like to collect as many as I can to help add more variety.
 
 **Q: How do I add my own graphics and music patches?**
 
 A: There's a tutorial on how to do that on the wiki [here](https://github.com/Coolcord/Level-Headed/wiki/How-to-Add-Your-Own-Graphics-and-Music-Into-Level%E2%80%90Headed).
 
 **Q: What's going to be in the next update?**
 
 A: You can see what changes are going to be implemented by looking at the [changelog](https://raw.githubusercontent.com/Coolcord/Level-Headed/master/Changelog.txt).
 
 **Q: What are you trying to work towards?**
 
 A: The next major goal is a pattern database for the "SMB1_Compliance" generator. This should make it significantly easier to create new patterns and also
 allow users to create their own. Progress has been made towards this with the addition of [Sequential Archives](https://github.com/Coolcord/Sequential_Archive) in v0.3.0, but there's still a lot more work to be done before this can happen.
 
 **Q: I set a level type to uncommon, but I got it multiple times in a row! Is this a bug?**
 
 A: Technically, no. Each commonality is associated with a weight like so:
 * Very Common = 50 points
 * Common = 30 points
 * Uncommon = 15 points
 * Rare = 5 points
 * None = 0 points
 
 When the level generator goes to generate a new level, it does not care what kind of level came before or will come after. All it cares about is the percentage chance that it calculates based upon the weights the user provides. Think of this like rolling a dice. There's nothing stopping you from rolling four 1's in a row, though it's unlikely to happen. To get an idea of the numbers, using the default settings, each level has a 32.26% chance of being a Standard Overworld level and a 9.68% chance of being an underwater level. Personally, this issue doesn't bother me at all, as the numbers seem to always work themselves out when playing multiple games in sequence. However, I may rework this system in the future, as many people seem to expect it to work differently. For now, just try dropping what you don't like to "Rare" as that'll make a big difference.
 
 **Q: There's no variety!**
 
 A: Bear in mind that Level-Headed is only v0.3, so it's only a fraction of what I have planned. Think of current releases as a preview of what's to come. With each update, this will become less and less of a problem.
 
 **Q: When will you support X game?**
 
 A: Level-Headed is being designed from the ground up with multi-game support in mind, but don't expect to see support for another game until after v1.0. That's a very, very long ways off.
 
 **Q: Is Level-Headed dead? You haven't updated it in months!**
 
 A: No, it is far from dead. I tend to work on Level-Headed off and on when I have spare time and when
 I have the interest. Consequently, development tends to be very slow and sporadic. However, I can't see
 myself ever dropping this project completely.
 
