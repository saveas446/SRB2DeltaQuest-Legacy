using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Windows.Forms;
using FormSerialisation;
using System.IO;
using System.Runtime.InteropServices;
//Comments:
/*
*****************************************************
           ________  _____     _____     ________
          / ______/ / __  \   / __  \   /_____  /
         / /_____  / /_/  |  / /_/  /   _____/ /
        /_____  / / _  __/  / ___  |   / _____/ 
       ______/ / / / \ \   / /__/  |  / /_____
START /_______/ /_/   \_\ /_______/  /_______/  SOURCE CODE!
Created by M. J. Spangler, known online as: TheHappyNihilist, Shardvex
Thanks to SSNTails for making the official launcher, which inspired this one.

*****************************************************

Requirements:

Windows (Linux port coming soon)
.NET Framework 4.0 or higher.

*****************************************************

Contact information:

Author's email:		furryaletheia@gmail.com
Author's website:	http://shardvex.blogspot.com/

*****************************************************

Release Log:

1/1/2014 
 * Beta v1.0 - Released new year's day!
 * Original version - GOD this was a terrible version!
1/22/2014
 * Version 1.1
 * Lots and lots of bugfixes.
 * Added lots of hosting options, including a host dedicated server feature
 * Completely redesigned the interface
 * You can now do demo recording/playback
2/8/2014
 * Version 1.2
 * Bugfix - Now you can load wads with spaces in the path.
 * Bugfix - Export to batch file actually works this time.
 * You can now warp to maps using their in-game numbering system.
2/8/2014
 * Version 1.2.1
 * A few minor bugfixes
 * Bugfix - Now you can execute a script
 * Added useless Sonic eyecandy. Sonic runs across the bottom of the form.
3/19/2014
 * Now it supports all of SRB2 2.1's player colors, like Black, Rosewood, and Zim!
 * Fixed hosting servers & deticated servers (wasn't working for 2.1)
 * Various other little tweaks and bugfixes

Features:

*All of the features in the official SRB2 launcher.

*You can save and load all the launcher settings to a file.

*You can create batch files (.bat) from your settings.

*You can record, play, and time demos.

*You can create and join a dedicated server.

*You can set your server password from the host menu.

*You can make SRB2 output its console to a command prompt window.

*There is an extra field where you can type in the name of a SOC to add.

*You can change the order of the file list which subsequently changes the order they're loaded in SRB2.

*You can add and remove multiple custom files (wads etc) simultaneously.

*You can run SRB2 and add files from other directories. 
(so if SRB2 is in your documents and there's a file on your desktop you want to use, it won't crash!)

*You can drag and drop files and directories into the file list (dragging in directories 
recursively adds all files from the directory, and all its sub directories) 

*****************************************************

Plans for startSRB2:

*Make a Player controls setup or MainCFG editor

*Make an option to launch SRB2 with a custom config file.

*Make a feature that retrieves a list of net-games from the master server, and you can join one of them by 
selecting it.


*****************************************************

About the source code:

Welcome to the startSRB2 source code! This frontend works similar to the official SRB2 Launcher,
though it has a few options that the official launcher lacks.

Most of the strings have spaces at the beginning and end. This is so that all the arguments don't
get combined into one long word.

I started this when I was still a newbie to programming. So please overlook any bad coding.

Many coders like to declare all their variables at the top of their functions, but not me!

I think it's best to declare variables when you first use them to ensure that they are 
always initialized to some valid value and that their intended use is always apparent. 
The alternative is typically to declare all variables in one location, 
typically at the top of the block or, even worse, at the top of a function. 
However, then the variable becomes separated from its meaning. 

By declaring the variable when it is first used, you give it context. 
You give it a reason to exist, which goes a long way towards understanding what the variable is for.
 
*****************************************************

Okay, I'm done blabbing. Here's the code:
 */

namespace startSRB2
{

    public partial class startSRB2 : Form
    {

        string autosavefile = "autosave.xml";

        public startSRB2()
        {
            InitializeComponent();
        }

        private void LauncherForm_Load(object sender, EventArgs e)
        {
            this.remember_ts.Checked = Properties.Settings.Default.Remember;

            //Reload settings from previous session, if the "remember settings" option is checked.
            if (remember_ts.Checked == true)
            {
                if (File.Exists(autosavefile))
                {
                    FormSerialisor.Deserialise(this, autosavefile);
                }
            }
            else { }
        }

        private void start_button_Click(object sender, EventArgs e)
        {

            //Path to SRB2 executable
            string srb2exe = Path.GetFileName(path_tbox.Text);
            string srb2path = Path.GetDirectoryName(path_tbox.Text);

            //Get all the parameters/arguments
            System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
            startInfo.FileName = @srb2exe;

            string info = GetParameters();
            startInfo.Arguments = info;
            //This next line makes SRB2 able to find srb2.srb
            startInfo.WorkingDirectory = srb2path;
            //MessageBox.Show(info);

            //Now, execute srb2 with arguments/parameters
            try
            {
                System.Diagnostics.Process.Start(startInfo);
            }
            catch (System.Exception excep)
            {
                MessageBox.Show(excep.Message);
                //Damn it. =(
            }

        }


        private string GetParameters()
        {

            //Get manual parameters, we need spaces at the beginning and end.
            string manualparameters = " " + parameters_textbox.Text + " ";

            //Multiplayer name
            string playername;
            if (multi_check.Checked)
            {
                playername = " +name " + name_tbox.Text;
            }
            else
            {
                playername = null;
            }

            //Connect to IP and "join my dedicated server"

            string connectip;
            if (multi_check.Checked
                && joinhost_tab.SelectedTab == joinhost_tab.TabPages["join_tab"]
                    && !dedicjoin_check.Checked)
            {
                connectip = " -connect " + ip_tbox.Text;
            }
            else if (multi_check.Checked
                && joinhost_tab.SelectedTab == joinhost_tab.TabPages["join_tab"]
                    && dedicjoin_check.Checked)
            {
                connectip = " -clientport 5030 -connect " + LocalIPAddress();
            }
            else
            {
                connectip = null;
            }

            //Host server
            string host;
            if (multi_check.Checked
                && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"])
            {
                host = " -server 1 ";
            }
            else
            {
                host = null;
            }


            //Server name
            string servername;
            if (multi_check.Checked
                && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"])
            {
                servername = " +servername " + servname_tbox.Text;
            }
            else
            {
                servername = null;
            }

            //Server password
            string password;
            if (multi_check.Checked
                && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                && password_check.Checked)
            {
                password = " -password " + password_tbox.Text;
            }
            else
            {
                password = null;
            }


            //Point limit
            string pointlimit = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && pointlimit_check.Checked)
            {
                pointlimit = " +pointlimit " + point_tbox.Text;
            }
            else
            {
                pointlimit = null;
            }

            //Time limit
            string timelimit = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && timelimit_check.Checked)
            {
                timelimit = " +timelimit " + time_tbox.Text;
            }
            else
            {
                timelimit = null;
            }

            //Maximum players
            string maxplayers = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && max_check.Checked)
            {
                maxplayers = " +maxplayers " + max_tbox.Text;
            }
            else
            {
                maxplayers = null;
            }

            //Disable special rings
            string specialrings = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && norings_check.Checked)
            {
                specialrings = " +specialrings 0 ";
            }
            else
            {
                specialrings = null;
            }

            //Sudden death
            string suddendeath = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && death_check.Checked)
            {
                suddendeath = " +suddendeath 1 ";
            }
            else
            {
                suddendeath = null;
            }

            //dedicated server, host and join.
            string dedicated = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && dedicated_check.Checked)
            {
                dedicated = " -dedicated ";
            }
            else
            {
                dedicated = null;
            }

            //Show online/advertise on the master server
            // As of SRB2 2.1, "-internetserver" is no longer used. Instead, "-room" handles hosting on the internet.
            string room = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && online_check.Checked)
            {
                room = " -room " + room_tbox.Text;
            }
            else if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && !online_check.Checked)
            {
                room = null;
            }
            else
            { }

            //Don't advance stage
            string advancemap = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && noadvance_check.Checked)
            {
                advancemap = " +advancemap 0 ";
            }
            else
            {
                advancemap = null;
            }

            //Force skin
            string forceskin = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && forceskin_check.Checked)
            {
                forceskin = " +forceskin 1 +skin" + forceskin_tbox.Text + " ";
            }
            else
            {
                forceskin = null;
            }

            //No download files from server
            string nodownload = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["join_tab"]
                        && nofiles_check.Checked)
            {
                nodownload = " -nodownload";
            }
            else
            {
                nodownload = null;
            }

            //No check server for files
            string nocheckforfiles = null;
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["join_tab"]
                        && nocheck_check.Checked)
            {
                nocheckforfiles = " -nofiles";
            }
            else
            {
                nocheckforfiles = null;
            }


            //Check OpenGL mode
            string opengl;
            if (opengl_radio.Checked)
            {
                opengl = " -opengl";
            }
            else
            {
                opengl = null;
            }

            //Check Win mode
            string winmode;
            if (winmode_check.Checked)
            {
                winmode = " -win";
            }
            else
            {
                winmode = null;
            }

            //Check Debug mode
            string debugmode;
            if (debug_check.Checked)
            {
                debugmode = " -debug";
            }
            else
            {
                debugmode = null;
            }


            /***************Warp and gametype begin*****************/

            //Warp to map for single player, in the main options tab.
            //There are two warp options, one for single player and one for multiplayer
            string warpmap;
            if (warp_check.Checked && !multi_check.Checked)
            {
                warpmap = " -warp " + MapNumConverter(warp_tbox.Text); // <--This method converts extended map number to integer
            }
            else
            {
                warpmap = null;
            }

            /*Warp to map for multiplayer, in the network options tab.*/
            //First, find if a gametype is selected and save the gametype number to a string
            string gametype = null;
            if (gametype_combo.SelectedIndex != -1)
            {
                if (gametype_combo.SelectedItem.ToString() == "Cooperative")
                {
                    gametype = "0";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Competition")
                {
                    gametype = "1";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Race")
                {
                    gametype = "2";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Match")
                {
                    gametype = "3";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Team Match")
                {
                    gametype = "4";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Tag")
                {
                    gametype = "5";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Hide and Seek")
                {
                    gametype = "6";
                }
                else if (gametype_combo.SelectedItem.ToString() == "Capture the Flag")
                {
                    gametype = "7";
                }
                else
                {
                }
            }
            string multiwarp = null;
            //If warp is selected and gametype is not selected
            if (multi_check.Checked
                    && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                        && multiwarp_check.Checked 
                            && gametype_combo.SelectedIndex == -1)
            {
                multiwarp = " -warp " + MapNumConverter(multiwarp_tbox.Text);
            }
            //If warp and gametype are selected
            else if (multi_check.Checked
                        && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                            && multiwarp_check.Checked && gametype_combo.SelectedIndex != -1)
            {
                multiwarp = " -warp " + MapNumConverter(multiwarp_tbox.Text) + " -gametype " + gametype + " ";
            }
            //If gametype is selected but warp is not
            else if (multi_check.Checked
                        && joinhost_tab.SelectedTab == joinhost_tab.TabPages["host_tab"]
                            && !multiwarp_check.Checked && gametype_combo.SelectedIndex != -1)
            {
                multiwarp = " -gametype " + gametype + " "; //In this case, the variable multiwarp contains the gametype, rather than warp.
            }
            else
            {
                multiwarp = null;
            }
            //Wow,that was some pretty complex logic. @_@

            /****************Warp and gametype end****************/

            ///Check sound/music combo box selection


            /*
             * Music and SFX
               Music Only
               SFX Only
               No Sound
             */
            string whatsound = null;
            if (sound_combo.SelectedIndex != -1)
            {
                if (sound_combo.SelectedItem.ToString() == "Music and SFX")
                {
                    whatsound = null;
                }
                else if (sound_combo.SelectedItem.ToString() == "Music Only")
                {
                    whatsound = " -nosound";
                }
                else if (sound_combo.SelectedItem.ToString() == "SFX Only")
                {
                    whatsound = " -nomusic";
                }
                else if (sound_combo.SelectedItem.ToString() == "No Sound")
                {
                    whatsound = " -nomusic -nosound";
                }
            }
            else
            {
                whatsound = null;
            }


            //Get the .WAD, .SOC, and other files
            string wadfiles;
            var wads = new List<string>();
            foreach (string wadfile in wad_list.Items)
            {
                Uri srb2exe = new Uri(path_tbox.Text);
                Uri waduri = new Uri(wadfile);
                Uri relativeUri = srb2exe.MakeRelativeUri(waduri);
                wads.Add(" \"" + Uri.UnescapeDataString(relativeUri.ToString().Replace('/', '\\')) + "\" ");
            }
            string wadlist = String.Join(" ", wads);

            //WADS will only be added if NOT joining a netgame
            if (wadlist != "")
            {
                wadfiles = " -file " +
                    wadlist;
            }
            else
            {
                wadfiles = null;
            }


            //What character: Sonic, Tails, Knux?
            string skin = null;

            if (player_combo.SelectedIndex != -1)
            {
                if (player_combo.SelectedItem.ToString() == "Sonic")
                {
                    skin = " +skin SONIC";
                }
                else if (player_combo.SelectedItem.ToString() == "Tails")
                {
                    skin = " +skin TAILS";
                }
                else if (player_combo.SelectedItem.ToString() == "Knuckles")
                {
                    skin = " +skin KNUCKLES";
                }
            }
            else
            {
                skin = null;
            }


            /*
               Skin colors for Demo 2:

                None (Green)
                Grey
                Peach
                Dark Red
                Silver
                Orange
                Red
                Blue
                Dark Blue
                Pink
                Beige
            */

            //Color!
            string color = null;
            if (color_combo.SelectedIndex != -1)
            {
                if (color_combo.SelectedItem.ToString() == "None (Green)")
                {
                    color = " +color 0 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Grey")
                {
                    color = " +color 1 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Peach")
                {
                    color = " +color 2 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Dark Red")
                {
                    color = " +color 3 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Silver")
                {
                    color = " +color 4 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Orange")
                {
                    color = " +color 5 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Red")
                {
                    color = " +color 6 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Blue")
                {
                    color = " +color 7 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Dark Blue")
                {
                    color = " +color 8 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Pink")
                {
                    color = " +color 9 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Beige")
                {
                    color = " +color 10 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Pink")
                {
                    color = " +color 11 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Lavender")
                {
                    color = " +color 12 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Purple")
                {
                    color = " +color 13 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Orange")
                {
                    color = " +color 14 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Rosewood")
                {
                    color = " +color 15 ";
                }
                else if (color_combo.SelectedItem.ToString() == "Beige")
                {
                    color = " +color 16 ";
                }
            }
            else
            { }

            //Time to join all of the parameters into one long string!!!!!

            //!IMPORTANT

            string allparameters =
                /*Main tab*/ manualparameters + opengl + winmode + debugmode + warpmap + whatsound
                /*WADS tab*/+ wadfiles
                /*Game tab*/ + skin + color
                /*Host sub-tab*/ + servername + password + pointlimit + timelimit + maxplayers + room + host
                + specialrings + dedicated + suddendeath + forceskin + advancemap + multiwarp
                /*Join sub-tab*/ + connectip + nocheckforfiles + nodownload
                /*player*/ + playername
                /*that's it! semicolon-->*/;

            return allparameters;
        }


        public string LocalIPAddress() //This is for joining dedicated servers. It returns the host's LAN IP (Local IP)
        {
            IPHostEntry host;
            string localIP = "";
            host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (IPAddress ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    localIP = ip.ToString();
                    break;
                }
            }
            return localIP;
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://wiki.srb2.org/wiki/Command_line_parameters");
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.srb2.org/");
        }

        private void add_button_Click(object sender, EventArgs e)
        {

            OpenFileDialog addWadDialog1 = new OpenFileDialog();
            addWadDialog1.Title = "Add files";
            addWadDialog1.Filter = "Common Files|*.wad; *.lua; *.soc|WAD Files|*.wad|LUA Files|*.lua|SOC Files|*.soc|SRB Files|*.srb|DTA Files|*.dta|PLR Files|*.plr|WPN Files|*.wpn|All Supported Files|*.wad; *.lua; *.soc; *.srb; *.dta; *.plr; *.wpn|All Files|*.*";
            addWadDialog1.FilterIndex = 1;
            addWadDialog1.CheckFileExists = true;
            addWadDialog1.CheckPathExists = true;
            addWadDialog1.Multiselect = true;

            if (addWadDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                foreach (string fileName in addWadDialog1.FileNames)
                {
                    wad_list.Items.Add(fileName);
                }
            }

        }

        private void delete_button_Click(object sender, EventArgs e)
        {
            if (wad_list.Items.Count > 0 && wad_list.SelectedIndex != -1)
            {
                for (int i = wad_list.SelectedIndices.Count - 1; i >= 0; i--)
                {
                    wad_list.Items.RemoveAt(wad_list.SelectedIndices[i]);
                }
            }
            else
            {

            }
        }
        private void warp_check_CheckedChanged(object sender, EventArgs e)
        {
            if (warp_check.Checked == true)
            {
                warp_tbox.ReadOnly = false;
            }
            else if (warp_check.Checked == false)
            {
                warp_tbox.ReadOnly = true;
            }
            else { }
        }

        private void color_combo_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (color_combo.SelectedIndex != -1)
            {
                if (color_combo.SelectedItem.ToString() == "Green")
                {
                    color_pic.BackColor = System.Drawing.Color.Lime;
                }
                else if (color_combo.SelectedItem.ToString() == "White")
                {
                    color_pic.BackColor = System.Drawing.Color.White;
                }
                else if (color_combo.SelectedItem.ToString() == "Silver")
                {
                    color_pic.BackColor = System.Drawing.Color.Silver;
                }
                else if (color_combo.SelectedItem.ToString() == "Grey")
                {
                    color_pic.BackColor = System.Drawing.Color.Gray; //Gray or Grey? Sheeesh English make up your mind.
                }
                else if (color_combo.SelectedItem.ToString() == "Black")
                {
                    color_pic.BackColor = System.Drawing.Color.Black; //OMG THIS CULOR IZ AHSUM
                }
                else if (color_combo.SelectedItem.ToString() == "Cyan")
                {
                    color_pic.BackColor = System.Drawing.Color.Cyan;
                }
                else if (color_combo.SelectedItem.ToString() == "Teal")
                {
                    color_pic.BackColor = System.Drawing.Color.Teal;
                }
                else if (color_combo.SelectedItem.ToString() == "Dark Blue")
                {
                    color_pic.BackColor = System.Drawing.Color.Navy;
                }
                else if (color_combo.SelectedItem.ToString() == "Blue")
                {
                    color_pic.BackColor = System.Drawing.Color.MediumBlue;
                }
                else if (color_combo.SelectedItem.ToString() == "Peach")
                {
                    color_pic.BackColor = System.Drawing.Color.PeachPuff;
                }
                else if (color_combo.SelectedItem.ToString() == "Tan")
                {
                    color_pic.BackColor = System.Drawing.Color.Tan;
                }
                else if (color_combo.SelectedItem.ToString() == "Pink")
                {
                    color_pic.BackColor = System.Drawing.Color.Pink;
                }
                else if (color_combo.SelectedItem.ToString() == "Lavender")
                {
                    color_pic.BackColor = System.Drawing.Color.MediumPurple; //This is the closest C# equivalent I could find.
                }
                else if (color_combo.SelectedItem.ToString() == "Purple")
                {
                    color_pic.BackColor = System.Drawing.Color.Purple;
                }
                else if (color_combo.SelectedItem.ToString() == "Orange")
                {
                    color_pic.BackColor = System.Drawing.Color.Orange;
                }
                else if (color_combo.SelectedItem.ToString() == "Rosewood")
                {
                    color_pic.BackColor = System.Drawing.Color.Chocolate; //This is the closest C# equivalent I could find. It's basically dark orange.
                }
                else if (color_combo.SelectedItem.ToString() == "Beige")
                {
                    color_pic.BackColor = System.Drawing.Color.Beige;
                }
                else if (color_combo.SelectedItem.ToString() == "Brown") //This is the closest C# equivalent I could find.
                {
                    color_pic.BackColor = System.Drawing.Color.Sienna;
                }
                else if (color_combo.SelectedItem.ToString() == "Red")
                {
                    color_pic.BackColor = System.Drawing.Color.Red;
                }
                else if (color_combo.SelectedItem.ToString() == "Dark Red")
                {
                    color_pic.BackColor = System.Drawing.Color.DarkRed;
                }
                else if (color_combo.SelectedItem.ToString() == "Neon_Green")
                {
                    color_pic.BackColor = System.Drawing.Color.Lime; //This is the closest C# equivalent I could find. 
                }
                else if (color_combo.SelectedItem.ToString() == "Green")
                {
                    color_pic.BackColor = System.Drawing.Color.Green;
                }
                else if (color_combo.SelectedItem.ToString() == "Zim")
                {
                    color_pic.BackColor = System.Drawing.Color.DarkSeaGreen; //This is the closest C# equivalent I could find. 
                }
                else if (color_combo.SelectedItem.ToString() == "Olive")
                {
                    color_pic.BackColor = System.Drawing.Color.Olive;
                }
                else if (color_combo.SelectedItem.ToString() == "Yellow")
                {
                    color_pic.BackColor = System.Drawing.Color.Yellow;
                }
                else if (color_combo.SelectedItem.ToString() == "Gold")
                {
                    color_pic.BackColor = System.Drawing.Color.Goldenrod;
                }
                else { }
            }
            else { color_pic.BackColor = default(Color); }

        }

        private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://wiki.srb2.org/wiki/Console");
        }

        private void parameters_label_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://wiki.srb2.org/wiki/Command_line_parameters");
        }

        void MoveUp(ListBox myListBox)
        {
            int selectedIndex = myListBox.SelectedIndex;
            if (selectedIndex > 0 & selectedIndex != -1)
            {
                myListBox.Items.Insert(selectedIndex - 1, myListBox.Items[selectedIndex]);
                myListBox.Items.RemoveAt(selectedIndex + 1);
                myListBox.SelectedIndex = selectedIndex - 1;
            }
        }

        void MoveDown(ListBox myListBox)
        {
            int selectedIndex = myListBox.SelectedIndex;
            if (selectedIndex < myListBox.Items.Count - 1 & selectedIndex != -1)
            {
                myListBox.Items.Insert(selectedIndex + 2, myListBox.Items[selectedIndex]);
                myListBox.Items.RemoveAt(selectedIndex);
                myListBox.SelectedIndex = selectedIndex + 1;

            }
        }
        private void down_btn_Click(object sender, EventArgs e)
        {
            MoveDown(wad_list);
        }

        private void up_btn_Click(object sender, EventArgs e)
        {
            MoveUp(wad_list);
        }

        //Menu related methods:
        //***************************
        private String _filename;

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {

            int savetabindex = devMaps.SelectedIndex;
            //Save the tab index so you're not inconveniently switched back to the main tab

            while (Controls.Count > 0)
            {
                Controls[0].Dispose();
            }
            InitializeComponent();

            devMaps.SelectedIndex = savetabindex;
            //Reload the tab index
            _filename = null;

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog OpenData = new OpenFileDialog();
            OpenData.Title = "Open Profile";
            OpenData.Filter = "Profile Files|*.xml|All Files|*.*";
            OpenData.FilterIndex = 1;
            OpenData.CheckFileExists = true;
            OpenData.CheckPathExists = true;
            OpenData.Multiselect = false;

            if (OpenData.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                FormSerialisor.Deserialise(this, OpenData.FileName);
                this.Text = System.IO.Path.GetFileNameWithoutExtension(OpenData.FileName) + " - startSRB2";
                _filename = OpenData.FileName;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowSaveDialog();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (String.IsNullOrEmpty(_filename))
            {
                ShowSaveDialog();
            }
            else
            {
                FormSerialisor.Serialise(this, _filename);
            }

        }

        DialogResult ShowSaveDialog()
        {
            var SaveDataAs = new SaveFileDialog();
            SaveDataAs.Title = "Save Profile";
            SaveDataAs.Filter = "Profile Files|*.xml|All Files|*.*";
            SaveDataAs.FilterIndex = 1;
            var result = SaveDataAs.ShowDialog();

            if (result == DialogResult.OK)
            {
                FormSerialisor.Serialise(this, SaveDataAs.FileName);
                this.Text = System.IO.Path.GetFileNameWithoutExtension(SaveDataAs.FileName) + " - startSRB2";
                _filename = SaveDataAs.FileName;
            }

            return result;
        }

        private void createBatchToolStripMenuItem_Click(object sender, EventArgs e)
        {

            System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();

            string info = GetParameters();
            string srb2exe = Path.GetFileName(path_tbox.Text);
            string srb2path = Path.GetDirectoryName(path_tbox.Text);
            
            startInfo.Arguments = info;
            startInfo.FileName = @srb2exe;
            //This next line makes SRB2 able to find srb2.srb
            startInfo.WorkingDirectory = srb2path;
                 
            //Save parameters/arguments to a bat file!
            var SaveBatch = new SaveFileDialog();
            SaveBatch.Title = "Create Batch";
            SaveBatch.Filter = "Batch Files|*.bat|All Files|*.*";
            SaveBatch.InitialDirectory = Application.StartupPath;
            SaveBatch.FilterIndex = 1;
            var result = SaveBatch.ShowDialog();

            if (result == DialogResult.OK)
            {
                if (startInfo.WorkingDirectory != null) //If SRB2 is in another directory
                {
                    StreamWriter sw = new StreamWriter(SaveBatch.FileName);

                    sw.WriteLine("cd /D"
                    + " \"" + startInfo.WorkingDirectory + "\" " + Environment.NewLine
                    + " \"" + startInfo.FileName + "\" "
                    + startInfo.Arguments);
                    sw.Close();
                }
                else //If SRB2 is in this program's directory
                {
                    StreamWriter sw = new StreamWriter(SaveBatch.FileName);

                    sw.WriteLine(" \"" + startInfo.FileName + "\" "
                    + startInfo.Arguments);
                    sw.Close();
                }

            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            aboutbx about = new aboutbx();
            about.ShowDialog();
        }
        //*************************
        //End of menu related methods

        private void player_combo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (player_combo.SelectedIndex != -1)
            {
                if (player_combo.SelectedItem.ToString() == "Sonic")
                {
                    character_pic.Image = global::startSRB2.Properties.Resources.sonic;
                }
                else if (player_combo.SelectedItem.ToString() == "Tails")
                {
                    character_pic.Image = global::startSRB2.Properties.Resources.tails;
                }
                else if (player_combo.SelectedItem.ToString() == "Knuckles")
                {
                    character_pic.Image = global::startSRB2.Properties.Resources.knux;
                }
            }
            else
            {
                character_pic.Image = null;
            }
        }

        private void browse_btn_Click(object sender, EventArgs e)
        {

            OpenFileDialog browseexedialog = new OpenFileDialog();
            browseexedialog.Title = "Path to SRB2 executable";
            browseexedialog.Filter = "Executable Files|*.exe|All Files|*.*";
            browseexedialog.FilterIndex = 1;
            browseexedialog.CheckFileExists = true;
            browseexedialog.CheckPathExists = true;
            browseexedialog.Multiselect = false;

            if (browseexedialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                path_tbox.Text = browseexedialog.FileName;
            }
        }

        private void multi_check_CheckedChanged(object sender, EventArgs e)
        {
            if (multi_check.Checked == true)
            {
                joinhost_tab.Enabled = true;
            }
            else if (multi_check.Checked == false)
            {
                joinhost_tab.Enabled = false;
            }
            else { }

        }

        private void wad_list_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop, false))
                e.Effect = DragDropEffects.All;
            else
                e.Effect = DragDropEffects.None;
        }

        private void wad_list_DragDrop(object sender, DragEventArgs e)
        {

            string[] directoryName = (string[])e.Data.GetData(DataFormats.FileDrop, false);

            List<string> filepaths = new List<string>();
            foreach (var s in directoryName)
            {
                if (Directory.Exists(s)) //To get inside of this if-statement the user must drop in a folder.
                {
                    string[] files = Directory.GetFiles(directoryName[0]);
                    foreach (string file in files)
                    {
                        string extension = Path.GetExtension(file);
                        if (extension == ".wad" || extension == ".soc" || extension == ".lua" ) //We need to filter out files without the right extension!
                        {
                            //Add files from folder:
                            wad_list.Items.Add(file);
                            /*This will only add files from the immediate directory. Not subdirectories.
                             *This is so that if some stupid person drops 
                             *in something huge like their C drive, startSRB2 won't crash!*/

                        }
                    }
                }
                else //To get inside this ELSE the user must drop in a file(s)
                {
                    //Add filepath:
                    wad_list.Items.Add(s); /*We don't filter out extensions here, 
                                            * because the user is dropping individual files rather than folders.*/
                }
            }
        }

        public string MapNumConverter(string MapNumText)
        //This method converts extended map number to integer
        {
            System.Text.RegularExpressions.Regex regex = 
                new System.Text.RegularExpressions.Regex("^[a-zA-Z0-9]*$");
            string mapreturn = null;

            MapNumText = MapNumText.Replace(" ", string.Empty);//Remove all white spaces from string

            if (!String.IsNullOrWhiteSpace(MapNumText) 
                && regex.IsMatch(MapNumText)
                && MapNumText.Length == 2) //If the string contains two characters and is alphanumeric.
            {
                mapreturn = MapAlgorithm(MapNumText);
            }
            else
            {
                mapreturn = MapNumText;
            }
            return mapreturn;

        }

        public string MapAlgorithm(string MapNumber)
        {
            //Algorithm found here: https://wiki.srb2.org/wiki/Extended_Map_Number
            char xchar = MapNumber[0];
            char ychar = MapNumber[1];

            int p;
            if (Char.IsLetter(xchar))
            {
                p = (int)xchar % 32 - 1;//The position of x within the alphabet, the numbering begins at zero.
            }
            else if (Char.IsDigit(xchar))
            {
                p = (int)Char.GetNumericValue(xchar);
            }
            else
            {
                p = 0;
            }

            int q;
            if (Char.IsLetter(ychar))
            {
                q = (int)ychar % 32 + 9;//The position of x within the alphabet, the numbering begins at ten. 
            }
            else if (Char.IsDigit(ychar))
            {
                q = (int)Char.GetNumericValue(ychar);
            }
            else
            {
                q = 0;
            }

            //Algorithm:  ((36*p + q) + 100)

            string mapnum;

            if (Char.IsNumber(xchar)
                && Char.IsNumber(ychar)) //If both characters are already numbers, we don't need to convert!
            {
                mapnum = MapNumber;
            }
            else
            {
                mapnum = ((36 * p + q) + 100).ToString();
            }

            return mapnum;
        }

        private void start_button_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(start_button, "This button launches SRB2 with the current settings");
        }

        private void add_button_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(add_button, "Click this to add a WAD or SOC to the list");
        }

        private void delete_button_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(delete_button, "Click this to remove the selected files from the list");
        }

        private void up_btn_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(up_btn, "Click this to move the selected list item up");
        }

        private void down_btn_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(down_btn, "Click this to move the selected list item down");
        }

        private void browse_btn_MouseHover(object sender, EventArgs e)
        {
            toolTip1.SetToolTip(browse_btn, "Click this to browse for the SRB2 executable");
        }

        private void roomid_label_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://mb.srb2.org/masterserver.php");
        }

        private void export_ts_Click(object sender, EventArgs e)
        {
            var ExportList = new SaveFileDialog();
            ExportList.Title = "Export List to Text File";
            ExportList.Filter = "Text File|*.txt|All Files|*.*";
            ExportList.FilterIndex = 1;
            var result = ExportList.ShowDialog();
            string textout = "";

            if (result == DialogResult.OK)
            {
                
                foreach (string li in wad_list.Items)
                {
                    textout = textout + li + Environment.NewLine;
                }

                _filename = ExportList.FileName;
                File.WriteAllText(ExportList.FileName, textout);
            }
        }

        private void import_ts_Click(object sender, EventArgs e)
        {
            OpenFileDialog ImportList = new OpenFileDialog();
            ImportList.Title = "Import List from Text File";
            ImportList.Filter = "Text Files|*.txt|All Files|*.*";
            ImportList.FilterIndex = 1;
            ImportList.CheckFileExists = true;
            ImportList.CheckPathExists = true;
            ImportList.Multiselect = false;

            if (ImportList.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string[] text = System.IO.File.ReadAllLines(ImportList.FileName);
                text = text.Where(x => !string.IsNullOrEmpty(x)).ToArray(); // <--This removes all blank lines from the files list
                wad_list.Items.AddRange(text);
            }
        }

        private void startSRB2_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (remember_ts.Checked == true) 
            { Properties.Settings.Default.Remember = true; }
            else 
            { Properties.Settings.Default.Remember = false; }

            //Save the state of the the "remember settings" option
            Properties.Settings.Default.Save();

            //If the "remember settings" option is checked, save the launcher settings to an xml file called "autosave.xml"
            if (remember_ts.Checked == true)
            {
                FormSerialisor.Serialise(this, autosavefile);
            }
            else { }

        }

        private void viewHelpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            help helpform = new help();
            helpform.Show();
        }

        private void online_check_CheckedChanged(object sender, EventArgs e)
        {
            if (online_check.Checked == true)
            {
                room_tbox.Enabled = true;
            }
            else
            {
                room_tbox.Enabled = false;
            }
        }

        private void clear_btn_Click(object sender, EventArgs e)
        {
            wad_list.Items.Clear();
        }

        private void dedicjoin_check_CheckedChanged(object sender, EventArgs e)
        {
            if (dedicjoin_check.Checked == true)
            {
                ip_tbox.Enabled = false;
            }
            else
            {
                ip_tbox.Enabled = true;
            }
        }

        private void sound_combo_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void res_group_Enter(object sender, EventArgs e)
        {

        }

        private void path_tbox_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {

        }
    }


}