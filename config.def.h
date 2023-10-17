/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define RGBf(r,g,b)    { r / 255.f, g / 255.f, b / 255.f }
#define RGBAf(r,g,b,a) { r / 255.f, g / 255.f, b / 255.f, a / 255.f}

#define TERMINAL "foot"

/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int mousefollowsfocus         = 1;  /* mouse follows focus */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartborders              = 1;
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const float bordercolor[]           = RGBAf(0x44, 0x44, 0x44, 0xff);
static const float focuscolor[]            = RGBAf(0x00, 0x55, 0x77, 0xff);
static const float urgentcolor[]           = RGBAf(0xff, 0x00, 0x00, 0xff);
static const unsigned int gappx            = 5;  /* default gap size */
/* To conform the xdg-protocol, set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1, 0.1, 0.1, 1.0}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
        "swaybg", "-m", "stretch", "-i", "pictures/bg/nature_blue.png", NULL,
        "someblocks", NULL,
        NULL /* terminate */
};

/* tagging - tagcount must be no greater than 31 */
static const int tagcount = 9;

static const Rule rules[] = {
	/* app_id     title       tags mask     iscentered isfloating  isterm  noswallow  monitor scratchkey */
	/* examples:
	{ "Gimp",     NULL,       0,            0,         1,          0,      1,         -1,     0 },
	*/
	{ "firefox",  NULL,       1 << 1,       0,         0,          0,      1,         -1,     0 },
	{ NULL,     "scratchpad", 0,            1,         1,          1,      1,         -1,    's' },
	{ NULL,     "spcalc",     0,            1,         1,          1,      1,         -1,    'c' },
	{ TERMINAL,   NULL,       0,            0,         0,          1,      1,         -1,     0 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* monitors */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = "compose:ralt,caps:escape",
};

static const int repeat_rate = 50;
static const int repeat_delay = 190;

/* Trackpad */
static const int tap_to_click = 0;
static const int tap_and_drag = 0;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = 0.0;
/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *browsercmd[]  = { "firefox-developer-edition", NULL };
static const char *browsercmd_p[]  = { "firefox-developer-edition", "-P", NULL };
static const char *menucmd[]     = { "bemenu_run_history", "-p", "$", NULL };
static const char *termcmd[]     = { TERMINAL, "-d", "none", NULL };

/* named scratchpads - First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = { "s", TERMINAL, "-d", "none", "-T", "scratchpad", "-a", "scratchpad", NULL };
static const char *scratchpadcalc[] = { "c", TERMINAL, "-d", "none", "-T", "spcalc", "-a", "spcalc", "-e", "bc", "-lq", NULL};

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ MODKEY,                    XKB_KEY_d,          spawn,          {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,     togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                    XKB_KEY_apostrophe, togglescratch,  {.v = scratchpadcalc } },

	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_v,          focusstack,     {.i =  0} },
	{ MODKEY,                    XKB_KEY_c,          focusstack,     {.i = INT_MAX} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          movestack,      {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          movestack,      {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_V,          movestack,      {.i =  0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_C,          movestack,      {.i = INT_MAX} },
	{ MODKEY,                    XKB_KEY_i,          incnmaster,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_I,          incnmaster,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05} },
	{ MODKEY,                    XKB_KEY_Tab,        view,           {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,          killclient,     {0} },
	{ MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_F,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_b,          setlayout,      {.v = &layouts[3]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_B,          setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_s,          togglesticky,   {0} },
	{ MODKEY,                    XKB_KEY_f,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,            {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },

	{ MODKEY,                    XKB_KEY_x,          spawn,          SHCMD("swaylock") },
	{ 0,                         XKB_KEY_Print,      spawn,          SHCMD("grim -g \"$(slurp)\" - | tee screenshot.png | wl-copy -t image/png") },
	{ MODKEY,                    XKB_KEY_Insert,     spawn,          SHCMD("notify-send \"Clipboard contents\" \"$(wl-paste -n)\"") },
	{ MODKEY,                    XKB_KEY_g,          spawn,          SHCMD("emoji") },

	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_E,          spawn,          SHCMD(TERMINAL " -d none nmtui") },
	{ MODKEY,                    XKB_KEY_e,          spawn,          SHCMD(TERMINAL " -d none btop") },

	{ MODKEY,                    XKB_KEY_w,          spawn,          {.v = browsercmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_W,          spawn,          {.v = browsercmd_p} },

	{ MODKEY,                    XKB_KEY_z,          spawn,          SHCMD("zathura") },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Z,          spawn,          SHCMD(TERMINAL " -d none open_book") },
	{ MODKEY,                    XKB_KEY_m,          spawn,          SHCMD("open_man") },

	/* Audio controls */
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_M,               spawn,     SHCMD("pamixer -t && sigblock -b vol") },
	{ 0,                         XKB_KEY_XF86AudioMute,   spawn,     SHCMD("pamixer -t && sigblock -b vol") },
	{ 0,                         XKB_KEY_XF86AudioRaiseVolume, spawn,SHCMD("pamixer --allow-boost -i 5 && sigblock -b vol") },
	{ 0,                         XKB_KEY_XF86AudioLowerVolume, spawn,SHCMD("pamixer --allow-boost -d 5 && sigblock -b vol") },
	{ MODKEY,                    XKB_KEY_bracketright,    spawn,     SHCMD("pamixer --allow-boost -i 5 && sigblock -b vol") },
	{ MODKEY,                    XKB_KEY_bracketleft,     spawn,     SHCMD("pamixer --allow-boost -d 5 && sigblock -b vol") },

	/* Monitor brightness controls */
	{ 0,                         XKB_KEY_XF86MonBrightnessUp,  spawn,SHCMD("brightnessctl s 10%+") },
	{ 0,                         XKB_KEY_XF86MonBrightnessDown,spawn,SHCMD("brightnessctl s 10%-") },

	/* Notification controls */
	{ WLR_MODIFIER_CTRL,                    XKB_KEY_space, spawn,      SHCMD("makoctl dismiss")},
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_space, spawn,      SHCMD("makoctl dismiss --all")},
	{ WLR_MODIFIER_CTRL|MODKEY,             XKB_KEY_space, spawn,      SHCMD("makoctl restore")},

	/* Misc. controls */
	{ 0,                         XKB_KEY_XF86Favorites,      spawn,  SHCMD("toggletouchpad") },
	{ 0,                         XKB_KEY_XF86TouchpadToggle, spawn,  SHCMD("toggletouchpad") },

	/* Gaps */
	{ MODKEY,                    XKB_KEY_minus,      setgaps,        {.i = -1} },
	{ MODKEY,                    XKB_KEY_equal,      setgaps,        {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_plus,       setgaps,        {.i =  0} },
	{ MODKEY,                    XKB_KEY_a,          togglegaps,     {0} },

	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                  8),

	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_BackSpace,  quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4),  CHVT(5),  CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
#undef CHVT
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
