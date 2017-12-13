Level-Headed
============

Random Level Generator and Level Script Compiler for Super Mario Bros. 1 on the NES

-===========================- About Level-Headed -===========================-

 Level-Headed is an application that is still under development. Ultimately, I am
 designing it to be a random level generator that will work across multiple platforming
 games. In its current state, Level-Headed can essentially create new Mario games based
 off of Super Mario Bros. 1 (SMB1) with a click of a button! Users can also create their
 own levels using the "SMB1_Compliance" scripting language. As of now, only SMB1 is
 supported, but in the future, support for more games will be added.
 
 In order for Level-Headed to work properly, a base ROM must be supplied to Level-Headed.
 As of the "Hammer Time" Build (v0.2.0), the following 2 ROMs are supported:
 
 Super Mario Bros. (JU) (PRG0) [!].nes (MD5: 811B027EAF99C2DEF7B933C5208636DE)
 
 Super Mario Bros. (JU) (PRG1).nes (MD5: 94EDE9347C1416105F1C08EC26B5B73A)
 
 Check out "Level-Headed Water Vapor Build Readme.pdf" under the "Doc" folder for more
 information on how to use Level-Headed.
 
 If you're a Windows user, you can download the Windows build under the releases tab.
 
 Linux and Mac users should compile the application and plugins through QtCreator.
 
 All code in this repository is released under the GPLv2 License.
 
 If you have any questions about Level-Headed, feel free to contact me at coolcord24@tutanota.com

 -===========================- Hammer Time FAQ -===========================-
 
 Q: What is hammer time mode?
 
 A: This is a new setting for the level generator that I may remove in the future. For now, this
 feature replaces about 20% of every individual enemy spawn (not including grouped enemies).
 This can make the game quite challenging in some cases. It's worth trying if you think the game
 is too easy.
 
 Q: What else is new in this update?
 
 A: Most of the new features are under "Base Game" options. You can also check out the
 changelog for more information on what's new.
 
 Q: Has the level generator improved in this update?
 
 A: Yup! Though not directly. Level-Headed makes use of patterns for the "SMB1_Compliance"
 generator. This update does not introduce any new patterns, so levels will still feel rather
 redundant. However, this update introduces move tables, which allow Level-Headed to resize
 levels in the ROM before creating level scripts. This essentially acts as primitive difficulty scaling.
 Later levels are longer and have a higher enemy density. In other words, the levels "Hammer Time"
 (v0.2.0) generates are harder and longer than what "Water Vapor" (v0.1.0) generated.
 
 Q: I see that you have included graphics and music patches. Can you include mine in the next update?
 
 A: Sure, I'll consider it! The graphics and music help make each game feel slightly more unique. Over time,
 I'd like to collect as many as I can to help add more variety.
 
 Q: Do you plan to add support for custom patches (.ips, .ppf, etc)?
 
 A: At this point, no. If you want to use these, you can try to generate a game with Level-Headed first
 and then applying the patch, but I can't guarentee that it'll work.
 
 Q: What's the plan for the next update?
 
 A: There are two major goals that I'm hoping to work towards. The first is a pattern database for the 
 "SMB1_Compliance" generator. This should make it significantly easier to create new patterns and also
 allow users to create their own. The second goal is to add support for another game that isn't a
 Nintendo property. Maybe Supertux? For now, I'm thinking about releasing smaller updates more
 often that slowly move towards these goals.
