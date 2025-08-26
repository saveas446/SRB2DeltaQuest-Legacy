SRB2 MIDI Archive v1.62 - Cobbled together by Sonix the sucky do and Vertz1515.
Shared through Sonix the sucky do's SpaceX internet and Vert1515's NASA internet.

Thank you for downloading the SRB2 MIDI Archive! This archive contains MIDIs from
every version of SRB2 you can think of (Besides 2k3, it doesn't have any). SRB1, 
SRB1.5, SSNTails' old Fangames. Sonic: Emerald Quest, and Sonic MADVenture are also here because of their ties with SRB2 
itself. As such, I hope you enjoy listening to this. It also contains songs 
that never made it into a version of the game and always stuck out. Within 
each folder is a .TXT file. These are the notes and commentary Vertz and I have
on each version's soundtrack as well as the individual songs featured within them. 
Without further ado, I will now let you listen to this game's wonderful MIDI soundtrack.



Message from Vertz:

... and if you're curious about how we were able to obtain these midis, you're not alone. In fact, 
we haven't found any guides online that explain this process. So, let me take this opportunity 
to share with you how Sonix and I were able to rip these midis.

To extract MIDI files from SRB1, SRB2TGF / SRB1XMAS, SRB1.5, and SSNTails' old CNC games, I use Dragon UnPACKer 5. 
First, I open the program and navigate to "File" > "HyperRipper" > "Formats". Next, I ensure that the "MIDI Audio" 
checkbox is selected under "Formats", and then I click on "Search" on the window toolbar. On the same window, I configure the
"Source" directory to be the source file of the TGF game I want to extract MIDI files from (e.g. srb13f.gam for SRB1, 
SRBXMAS.CCA for SRB1Xmas, or Srb1.5.cca for SRB1.5). After that, I click on "Search" and wait for the progress bar to finish. 
Once done, I click on "Ok" to return to the main window and then drag each MIDI file found to a directory on my computer. 
Unfortunately, this method does not preserve the original filenames, so you may need to use a version of Clickteam Fusion 
to obtain them. However, using this method, you will have successfully extracted every MIDI file in the game, which is a win in my book.

You can extract MIDI files from any Doom-related engine by using that particular method, although the most effective approach 
is to use Slade. To do this, open an SRB, WAD, or PK3 file, locate the MIDI files, select them, right-click, and choose "export." it's as easy as that! 

If you want to extract MIDI files from DLL files, Slade cannot open them, even if you force it to. However, you can trick Slade into thinking it is 
opening a WAD file by using a Hex Editor or Notepad to modify the DLL file's header. Change "SDLL" to "PWAD" at the beginning of the file, 
save the changes, and Slade will open the DLL file without any issues, allowing you to view and extract any relevant data. Though I haven't tested
if those DLL files still work in game after edited so I would suggest making a copy just in case.

Here's another method for getting TGF / CNC games that I consider only to be used as a last resort. It doesn't require any external programs
and it's pretty much the most direct way to obtain midis. Essentially, you open file explorer, navigate to C:> Users > (Your Current User Profile) > AppData > Local > Temp
from here, you need to open up your game and make sure you're on a screen where the midis you want are loaded (Can be played on the current instance your game is at)
if you look at your temp folder, you should see the midis appear in the root of the folder, but sometimes (usually on newer versions of CNC stand alone progroms)
midis are placed in some folder with a garbled name, which in the case of Sonic MADventure, usually ends in .tmp. Now, all you need to do is take the midis and copy them to 
another directory before they're unloaded. However, the only time I've used this method was for Sonic MADventure as I've stated previously, but that was only because I
couldn't use any other midi extracting techniques on the game, so I highly reccomend using the HyperRipper method before you try this one.

That being said, if you know of other means to obtain midis in better ways, then feel free to let me know.