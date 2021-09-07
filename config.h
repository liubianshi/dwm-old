#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
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
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;    /* Switch view on tag switch */
static const char *fonts[]          = { 
    "FiraCode Nerd Font Mono:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintslight",
    "WenQuanYi Micro Hei Mono:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintslight",
    "Noto Color Emoji:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintslight",
    "JoyPixels:pixelsize=24:antialias=true:autohint=true:hintstyle:hintslight",
};
static const char dmenufont[]       = "monospace:pixelsize=24:antialias=true:autohint:false:hint:ture:hintstyle:hintslight";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
//static const char col_border[]      = "#42A5F5";
static const char col_border[]      = "#F58842";
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
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
	/* class          instance    title       tags mask     isfloating  isterminal  noswallow  monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,          0,           0,        -1 },
	{ "Firefox",      NULL,       NULL,       1 << 8,       0,          0,          -1,        -1 },
	{ "St",           NULL,       NULL,       0,            0,          1,           0,        -1 },
	{ "Alacritty",    NULL,       NULL,       0,            0,          1,           0,        -1 },
	{ NULL,           NULL,  "Event Tester",  0,            1,          0,           1,        -1 }, /* xev */
    { "fzfmenu",      NULL,       NULL,       0,            1,          0,           1,        -1 },
    { "Xfce4-appfinder", NULL,    NULL,       0,            1,          0,           1,        -1 },
    { "VirtualBox Machine", NULL, NULL,       0,            1,          0,           1,        -1 },
    { "ncmpcpp",      NULL,       NULL,       0,            1,          0,           1,        -1 },
    { "blueman-manager", NULL,    NULL,       0,            1,          0,           1,        -1 },
    { NULL, "netease-cloud-music-gtk",  NULL, 0,            1,          0,           1,        -1 },
    { NULL, "netease-cloud-music",      NULL, 0,            1,          0,           1,        -1 },
    { "Zotero",       NULL,       NULL,       1 << 3,       0,          0,           0,        -1 },
    { "Okular",       NULL,       NULL,       1 << 5,       0,          0,           0,        -1 },
    { "Wps",          NULL,       NULL,       1 << 2,       0,          0,           0,        -1 },
    { "Steam",        NULL,       NULL,       1 << 6,       0,          0,           0,        -1 },
    { "Vmplayer",     NULL,       NULL,       1 << 7,       0,          0,           0,        -1 },
    { "Zenity",       NULL,       NULL,       0,            1,          0,           1,        -1 },
    { "Emacs", "emacs", "doom-capture",       0,            1,          0,           1,        -1 },
    { "Display",      "display",  NULL,       0,            1,          0,           1,        -1 },
    { "copyq",        NULL,       NULL,       0,            1,          0,           1,        -1 },
    { "R_x11",        NULL,       NULL,       0,            0,          0,           1,        -1 },
    { "qutebrowser",  NULL,       NULL,       1 << 1,       0,          0,          -1,        -1 },
    { "Brave-browser", NULL,      NULL,       1 << 1,       0,          0,          -1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.62; /* factor of master area size [0.05..0.95] */
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
    { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
    { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
    { MOD, XK_equal, ACTION##stack, {.i = 0 } }, \
    { MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } }, \
    { MOD, XK_x,     ACTION##stack, {.i = -1 } },
    /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
    /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
//static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "127x20+300+0", NULL };
//static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-d", "127", "20", NULL };
static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-o", "window.dimensions.columns=127", "window.dimensions.lines=20", NULL };
static Key keys[] = {
    /* modifier                      key        function        argument */
    STACKKEYS(MODKEY,           focus)
    STACKKEYS(MODKEY|ShiftMask, push)
    TAGKEYS( XK_1, 0 )
    TAGKEYS( XK_2, 1 )
    TAGKEYS( XK_3, 2 )
    TAGKEYS( XK_4, 3 )
    TAGKEYS( XK_5, 4 )
    TAGKEYS( XK_6, 5 )
    TAGKEYS( XK_7, 6 )
    TAGKEYS( XK_8, 7 )
    TAGKEYS( XK_9, 8 )

    { MODKEY,             XK_0,      view,      {.ui = ~0 } },
    { MODKEY|ShiftMask,   XK_0,      tag,       {.ui = ~0 } },
    { MODKEY,             XK_comma,  viewtoleft,     {0} },
    { MODKEY|ShiftMask,   XK_comma,  tagtoleft,      {0} },
    { MODKEY,             XK_period, viewtoright,    {0} },
    { MODKEY|ShiftMask,   XK_period, tagtoright,    {0} },
    { MODKEY,             XK_q,      killclient,    {0} },
    { MODKEY|ShiftMask,   XK_q,      quit,       {0} },
    { MODKEY|ControlMask|ShiftMask,  XK_q,       quit,  {1} },
    { MODKEY,             XK_r,      spawn,      {.v = dmenucmd } },
    { MODKEY,             XK_f,      togglefullscr, {0} },
    { MODKEY,             XK_space,  zoom,      {0} },
    { MODKEY|ShiftMask,   XK_space,  togglefloating,    {0} },
    { MODKEY|ControlMask, XK_space,  setlayout, {.v = &layouts[1]} },

    { MODKEY,             XK_t,      setlayout, {.v = &layouts[0]} },
    { MODKEY|ShiftMask,   XK_t,      setlayout, {.v = &layouts[2]} },
    { MODKEY,             XK_u,      setlayout, {.v = &layouts[3]} },
    { MODKEY|ShiftMask,   XK_u,      setlayout, {.v = &layouts[4]} },
    { MODKEY,             XK_y,      setlayout, {.v = &layouts[5]} },
    { MODKEY|ShiftMask,   XK_y,      setlayout, {.v = &layouts[6]} },

    { MODKEY,             XK_i,      incnmaster, {.i = +1 } },
    { MODKEY|ShiftMask,   XK_i,      incnmaster, {.i = -1 } },

    { MODKEY,             XK_a,      setmfact,       {.f = +0.02} },
    { MODKEY|ShiftMask,   XK_a,      setmfact,      {.f = -0.02} },

    { MODKEY,             XK_Return, spawn,     {.v = termcmd } },
    { MODKEY|ShiftMask,   XK_Return, togglescratch, {.v = scratchpadcmd } },
    { MODKEY,            XK_b,       togglebar, {0} },

    { MODKEY,            XK_h,       focusmon,       {.i = -1 } },
    { MODKEY,            XK_l,       focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,  XK_h,       tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,  XK_l,       tagmon,         {.i = +1 } },
    { MODKEY,              XK_bracketleft,        rotatestack,    {.i = +1 } },
    { MODKEY|ShiftMask,    XK_bracketleft,        rotatestack,    {.i = -1 } },
    { MODKEY,              XK_bracketright,       hidewin,        {0} },
    { MODKEY|ShiftMask,    XK_bracketright,       restorewin,     {0} },
    { MODKEY,              XK_o,                  hideotherwins,  {0}},
    { MODKEY|ShiftMask,    XK_o,                  restoreotherwins, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
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

void
focusurgent(const Arg *arg)
{
    Monitor *m;
    Client *c;
    int i;
    
    for (m = mons; m; m = m->next) {
        for(c = m->clients; c && !c->isurgent; c = c->next);
        if (c)
            break;
    }

    if (c) {
        //unfocus(selmon->sel, 0);
        selmon = m;
        for(i = 0; i < LENGTH(tags) && !((1 << i) & c->tags); i++);
        if(i < LENGTH(tags)) {
            const Arg a = {.ui = 1 << i};
            view(&a);
            focus(c);
        }
    }
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
    /* signum           function */
    { "focusmon",       focusmon },
    { "focusstack",     focusstack },
    { "focusurgent",    focusurgent }, 
    { "defaultgaps",    defaultgaps }, /* reset gaps back to default */
    { "togglegaps",     togglegaps },  /* toggle gaps on and off */
    { "incrgaps",       incrgaps },    /* increase all gaps */
    { "incrogaps",      incrogaps },   /* increase outer gaps */
    { "incrohgaps",     incrohgaps },  /* increase outer horizontal gaps */
    { "incrovgaps",     incrovgaps },  /* increase outer vertical gaps */
    { "incrigaps",      incrigaps },   /* increase  inner gaps */
    { "incrihgaps",     incrihgaps },  /* increase inner horizontal gaps */
    { "incrivgaps",     incrivgaps },  /* increase inner horizontal gaps */
    { "incnmaster",     incnmaster },
    { "killclient",     killclient },
    { "quit",           quit },
    { "setlayout",      setlayout },
    { "setlayoutex",    setlayoutex },
    { "setmfact",       setmfact },
    { "tag",            tag },
    { "tagall",         tagall },
    { "tagex",          tagex },
    { "tagmon",         tagmon },
    { "togglebar",      togglebar },
    { "togglefloating", togglefloating },
    { "toggletag",      tag },
    { "toggletagex",    toggletagex },
    { "toggleview",     view },
    { "toggleviewex",   toggleviewex },
    { "view",           view },
    { "viewall",        viewall },
    { "viewex",         viewex },
    { "zoom",           zoom },
};



