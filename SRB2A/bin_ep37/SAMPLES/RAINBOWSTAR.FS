// FraggelScript example
// (c) 2001-2002, Thierry Van Elsuwe

[level info]
levelname  = RainBowStar
creator    = Thierry 'Hurdler' Van Elsuwe
consolecmd = gr_solvetjoin off
gravity    = 80

[Scripts]

include("things.h");

script 150
{
    int fog_density = 0;
    while (fog_density <= 300)
    {
        runcommand("gr_fogdensity ", fog_density);
        fog_density = fog_density + 1;
        wait(1);
   }
}

script 151
{
    print("\2FraggleScript math functions...\n");
    print("pow ( 2, 0.5 ) /2      : ", pow(2,0.5)/2,       "\n");
    print("sqrt ( 2 ) /2          : ", sqrt(2)/2,          "\n");
    print("sin ( PI/4 )           : ", sin(0.7853981634),  "\n");
    print("cos ( PI/4 )           : ", cos(0.7853981634),  "\n");
    print("asin ( sqrt ( 2 ) /2 ) : ", asin(sqrt(2)/2),    "\n");
    print("acos ( sqrt ( 2 ) /2 ) : ", acos(sqrt(2)/2),    "\n");
    print("atan ( 1 )             : ", atan(1),            "\n");
    print("floor ( 1.054546456 )  : ", floor(1.054546456), "\n");
}

// You can add here some global variables
int  brol = 0;

script 1
{
    runcommand("cls\n");
    wait(20);

    {
        string helloWorld;
        helloWorld = "Hello " + "World" + " !\n" + 99 + 1 + " : "+ (99+1);
        print("Hurdler: ", helloWorld, "\n");
    }

    startscript(150);
    //startscript(151);
    
    setcorona(PLASMA_L, CORONA_TYPE, LIGHT_SPR);
    setcorona(PLASMA_L, CORONA_COLOR, "FF0000FF");
    setcorona(PLASMAEXP_L, CORONA_COLOR, "FF0000FF");
    setcorona(PLASMA_L, LIGHT_COLOR, "FF0000FF");
    setcorona(PLASMAEXP_L, LIGHT_COLOR, "FF0000FF");
    setcorona(BLUETALL_L, CORONA_OFFY, 40.0);
    
    startscript(10);
    
    print("Welcome!\n");
    beep();
    wait(200);
    runcommand("say Woo! I'm enjoying the new Legacy features!\n");
    wait(200);
    print("\2Yeah, baby... This is the joy of FraggleScript!\n");
    wait(200);
    timedtip(100, "Ready?\n");
    beep();
    wait(100);
    timedtip(100, "Steady!\n");
    wait(50);
    timedtip(100, "Set...\n");
    wait(50);
    timedtip(500, "Enjoy !\n");
    beep();
    
    spawn(HEALTHPOTION, 0, 250, 0, 300);
    spawn(HEALTHPOTION, -600, 1000, 0, 250);
    spawn(SHOTGUY, -700, 900, 0, 250);
}

int ccolor = 0;
int col_r = 0;
int col_g = 0;
int col_b = 0;
int col_a = 0;
script 10
{
    fixed offset = 20.0;
    fixed inc = 1.0;
    while (0==0)
    {
        wait(1);
        col_r = (col_r+1) & 255;
        col_g = (col_g+1) & 255;
        col_b = 255;
        col_a = 255;
        ccolor = col_a*256*256*256 + col_b*256*256 + col_g*256 + col_r;
        setcorona(BLUETALL_L, CORONA_COLOR, ccolor);
        offset = offset+inc;
        if (offset<20.0 || offset>30.0)
            inc = -inc;
        setcorona(BLUETALL_L, CORONA_OFFY, offset);
        setcorona(BLUETALL_L, CORONA_SIZE, (offset-10)*8);
   }
}

script 2
{
    timedtip(400, "You entered a 3D coloured sector\n");
    beep();
}

int s3_first = 0;
script 3
{
    if (s3_first==0)
    {
        timedtip(400, "Plouf in 3D translucent water\n");
        beep();
        s3_first = 1;
    }
}

int s4_first = 0;
script 4
{
    if (s4_first==0)
    {
        timedtip(400, "3D translucent water below 3D translucent floor\n");
        beep();
        s4_first = 1;
    }
}

script 5
{
    timedtip(400, "coloured sector with alpha intensity\n");
}

int s6_first = 0;
script 6
{
    if (s6_first==0)
    {
        timedtip(400, "nearly opaque 3D water\n");
        beep();
        s6_first = 1;
    }
}

int s7_first = 0;
script 7
{
    if (s7_first==0)
    {
        timedtip(400, "3D translucent water with small alpha valuee\n");
        beep();
        s7_first = 1;
    }
}


startscript(1);
