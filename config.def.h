#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const Bool viewontag         = True;    /* Switch view on tag switch */
static const char *fonts[]          = { 
    "Fira Code:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintfull",
    "WenQuanYi Micro Hei:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintfull",
    "SauceCodePro Nerd Font Mono:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintfull",
    "Noto Color Emoji:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintfull",
    "JoyPixels:pixelsize=24:antialias=true:autohint=true",
};
static const char dmenufont[]       = "monospace:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintfull";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]      = "#42A5F5";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "1 \uf120", "2 \uf7ae", "3 \uf121", "4 \uf04b", "5 \ue62e", "6 \uf251", "7 \ue727", "8 \uf537", "9 \uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,           -1 },
	{ "fzfmenu",      NULL,       NULL,       0,            1,           -1 },
	{ "ncmpcpp",      NULL,       NULL,       0,            1,           -1 },
	{ "blueman-manager", NULL,       NULL,    0,            1,           -1 },
	{ NULL, "netease-cloud-music-gtk",       NULL,    0,    1,           -1 },
	//{ "qutebrowser",  NULL,       NULL,     1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "=[]",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,	 ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD, XK_k,	 ACTION##stack,	{.i = INC(-1) } }, \
    { MOD, XK_equal, ACTION##stack, {.i = 0 } }, \
    { MOD, XK_x,     ACTION##stack, {.i = -1 } }, 
    /*{ MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } }, \  */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x24", NULL };
//static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-d", "120", "20", NULL };
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *jgmenucmd[]  = { "jgmenu_run", NULL };
static const char *musiccmd[] = { "st", "-c", "ncmpcpp", "-g" "127x20+300+0", "-i", "-e", "sh", "-c", "\"ncmpcpp\"", NULL };

#include "focusurgent.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(              XK_1,		0)
	TAGKEYS(              XK_2,		1)
	TAGKEYS(              XK_3,		2)
	TAGKEYS(              XK_4,		3)
	TAGKEYS(              XK_5,		4)
	TAGKEYS(              XK_6,		5)
	TAGKEYS(              XK_7,		6)
	TAGKEYS(              XK_8,		7)
	TAGKEYS(              XK_9,		8)
	{ MODKEY,             XK_w,		spawn,		SHCMD("rofi -show window -modi window -columns 1 -width 50 -lines 10 -bw 2 -location 1 -line-margin 10 -bw 2") },
	{ MODKEY,             XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,   XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,             XK_minus,	spawn,		SHCMD("lmc down") },
	{ MODKEY|ShiftMask,   XK_minus,	spawn,		SHCMD("lmc up") },
	/* { MODKEY,          XK_BackSpace,	spawn,		SHCMD("") }, */
	{ MODKEY|ShiftMask,   XK_BackSpace,	spawn,  SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Reboot computer?\")\" = Yes ] && sudo -A reboot") },
	{ MODKEY,             XK_q,		killclient,	{0} },
	{ MODKEY,             XK_grave,	spawn,	    SHCMD("~/useScript/rofi/rofi-uni.sh") },
	{ MODKEY,             XK_e,		spawn,		SHCMD("st -e lf") },
	{ MODKEY,             XK_c,		spawn,		SHCMD("~/useScript/testCollection.sh") },
	{ MODKEY|ShiftMask,   XK_c,		spawn,		SHCMD("~/useScript/testCollection.sh -c") },
	{ MODKEY,             XK_p,		spawn,		SHCMD("~/useScript/testCollection.sh -i") },
	{ MODKEY|ShiftMask,   XK_p,		spawn,		SHCMD("~/useScript/testCollection.sh -ci") },
	{ MODKEY,             XK_d,		spawn,		SHCMD("rifle \"$(fd --changed-within=3d . ~/Downloads | fzfmenu)\"") },
	{ MODKEY,             XK_space,	zoom,		{0} },
	{ MODKEY|ShiftMask,   XK_space,	togglefloating,	{0} },
	{ MODKEY,             XK_g,     focusurgent,    {0} },
	{ MODKEY|ControlMask, XK_space,	setlayout,	{.v = &layouts[1]} },
	{ MODKEY,             XK_t,		setlayout,	{.v = &layouts[0]} },
	{ MODKEY|ShiftMask,   XK_t,		setlayout,	{.v = &layouts[2]} },
	{ MODKEY,             XK_u,		setlayout,	{.v = &layouts[3]} },
	{ MODKEY|ShiftMask,   XK_u,		setlayout,	{.v = &layouts[4]} },
	{ MODKEY,             XK_y,		setlayout,	{.v = &layouts[5]} },
	{ MODKEY|ShiftMask,   XK_y,		setlayout,	{.v = &layouts[6]} },

	{ MODKEY,             XK_i,		incnmaster, {.i = +1 } },
	{ MODKEY|ShiftMask,   XK_i,		incnmaster, {.i = -1 } },
	{ MODKEY,             XK_r,		spawn,      {.v = dmenucmd } },
	{ MODKEY,             XK_f,		togglefullscr,	{0} },
 	{ MODKEY,             XK_h,     viewtoleft,     {0} },
 	{ MODKEY,             XK_l,     viewtoright,    {0} },
 	{ MODKEY|ShiftMask,   XK_h,     tagtoleft,      {0} },
 	{ MODKEY|ShiftMask,   XK_r,     tagtoright,     {0} },
	{ MODKEY,             XK_a,		setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,   XK_a,		setmfact,	    {.f = -0.05} },
	{ MODKEY,             XK_Return,spawn,		{.v = termcmd } },
	{ MODKEY|ShiftMask,   XK_Return,togglescratch,	{.v = scratchpadcmd } },
	{ MODKEY,             XK_m,     spawn,		SHCMD("st -e proxychains -q neomutt") },
	{ MODKEY|ShiftMask,   XK_m,		spawn,		SHCMD("proxychains -q mbsync -a 2>&1 >/dev/null") },
	{ MODKEY,             XK_v,		spawn,		SHCMD("st -e $EDITOR -c \"VimwikiIndex\"") },
	{ MODKEY,             XK_b,		togglebar,	{0} },
	{ MODKEY,             XK_Insert,spawn,		SHCMD("notify-send \"📋 Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },
    { MODKEY,             XK_F1,    spawn,      {.v = screenshotcmd } },
    { MODKEY,             XK_F2,    spawn,      {.v = jgmenucmd } },
	{ MODKEY,             XK_F3,	quit,		{0} },
	{ MODKEY,             XK_F4,	spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Hibernate computer?\")\" = Yes ] && sudo -A zzz") },
	{ MODKEY,             XK_F6,	spawn,		SHCMD("torwrap") },
	{ MODKEY,             XK_F7,	spawn,		SHCMD("td-toggle") },
	{ MODKEY,             XK_F8,	spawn,		SHCMD("mpc preview") },
	{ MODKEY,             XK_F9,	spawn,		SHCMD("mpc toggle") },
	{ MODKEY,             XK_F10,	spawn,		SHCMD("mpc next") },
	{ MODKEY,             XK_F11,	spawn,		{.v = musiccmd } },
	{ MODKEY,             XK_F12,	spawn,		SHCMD("mailsync") },

	{ MODKEY,             XK_Delete,spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,             XK_Scroll_Lock,	spawn,	SHCMD("killall screenkey || screenkey &") },


	/* { MODKEY,          XK_space,  setlayout,      {0} }, */

     { MODKEY,            XK_comma,  focusmon,       {.i = -1 } },
     { MODKEY,            XK_period, focusmon,       {.i = +1 } },
     { MODKEY|ShiftMask,  XK_comma,  tagmon,         {.i = -1 } },
     { MODKEY|ShiftMask,  XK_period, tagmon,         {.i = +1 } },

	{ MODKEY|ShiftMask,   XK_w,		 spawn,		SHCMD("$BROWSER") },
	{ MODKEY|ControlMask, XK_w,		 spawn,		SHCMD("proxychains -q tabbed surf -e") },

	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */
	{ MODKEY,              XK_bracketleft,          rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_bracketleft,          rotatestack,    {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_n,                    restorewin,     {0} },
	{ MODKEY,              XK_n,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_n,                    restorewin,     {0} },
	{ MODKEY,              XK_o,                    hideotherwins,  {0}},
	{ MODKEY|ShiftMask,    XK_o,                    restoreotherwins, {0}},

	{ 0, XF86XK_AudioMute,		    spawn,		SHCMD("lmc toggle") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("lmc up") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("lmc down") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button3,        spawn,          {.v = jgmenucmd} },
	{ ClkStatusText,        0,              Button3,        spawn,          SHCMD("htop") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};
