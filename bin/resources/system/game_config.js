var g_config = {
    level : {
        res_menu:0,
        res_restart:2,
    },

    audio : {
      theme : 2,
      speech : 4
    },

    climate : {
      central : 0,
      northen : 1,
      desert  : 2
    },

    gift : {
      modest : 1,
      generous : 2,
      lavish : 3
    },

    languages : [
                  { lang : "English",     ext : "en", },
                  { lang : "Русский",     ext : "ru", talks : ":/audio/wavs_citizen_ru.zip" },
                  { lang : "Українська",  ext : "ua", },
                  { lang : "Deutsch",     ext : "de", talks : ":/audio/wavs_citizen_de.zip" },
                  { lang : "Svenska"    , ext : "sv", },
                  { lang : "Español"    , ext : "sp", talks : ":/audio/wavs_citizen_sp.zip" },
                  { lang : "Român"      , ext : "ro", },
                  { lang : "Français"   , ext : "fr", },
                  { lang : "Czech"      , ext : "cs", },
                  { lang : "Hungarian"  , ext : "hu", },
                  { lang : "Italian"    , ext : "it", talks : ":/audio/wavs_citizen_it.zip" },
                  { lang : "Polish"     , ext : "pl", },
                  { lang : "Suomi"     ,  ext : "fn", },
                  { lang : "Português"  , ext : "pr", },
                  { lang : "Cрпски"    ,  ext : "sb", },
                  { lang : "Korean"     , ext : "kr", font : "HANBatangB.ttf" }
                ]
}

var game = {}
game.ui = {}
game.ui.dialogs = {}
game.sound = {}

/********************** namespace level **********************************/
var sim = {}

sim.climate = {}

sim.ui = {}
sim.ui.topmenu = {}
sim.ui.topmenu.help = {}
sim.ui.topmenu.file = {}
sim.ui.topmenu.debug = {}
sim.ui.topmenu.advisors = {}
sim.ui.topmenu.options = {}
sim.ui.advisors = {}
sim.ui.dialogs = {}

/********************** namespace lobby **********************************/
var lobby = {}
lobby.ui = {}
lobby.ui.newgame = {}
lobby.ui.loadgame = {}
lobby.ui.mainmenu = {}
lobby.ui.options = {}
