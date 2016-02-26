var g_wasChangesShow = false;
var g_lobbyButtons = [];

function OnShowChanges(force)
{
  engine.log( "JS:OnShowChanges" );

  if( force )
  {
    engine.setOption( "showLastChanges", true);
    engine.setOption( "lastChangesNumber", 0 );
    g_wasChangesShow = false;
  }

  var lastChangesNumber = g_session.lastChangesNum;
  var currentChangesNumber = engine.getOption( "lastChangesNumber" );
  engine.setOption( "lastChangesNumber", lastChangesNumber );

  if( lastChangesNumber !== currentChangesNumber )
    engine.setOption( "showLastChanges", true);

  if( !g_wasChangesShow && engine.getOption( "showLastChanges" ) > 0 )
  {
    g_wasChangesShow = true;

    var wnd = g_ui.addWindow(0,0,400,500);
    wnd.model = ":/changes/" + lastChangesNumber + ".changes";

    var btn = wnd.addButton( 13, wnd.height-36, 300, 24 );
    btn.text = "##hide_this_msg##";
    btn.font = "FONT_2";
    btn.style = "whiteBorderUp";
    btn.callback = function() {
                                var showChanges = !engine.getOption("showLastChanges");
                                engine.setOption( "showLastChanges", showChanges );
                                btn.text = showChanges ?  "##show_this_msg##" : "##hide_this_msg##";
                              };

    wnd.addExitButton(wnd.width-36,wnd.height-36);

    wnd.moveToCenter();
    wnd.mayMove = false;
    wnd.setModal();
  }
}

function OnStartCareer()
{
  OnChangePlayerName(true,function() {  g_session.startCareer(); } );
}

function setLanguage(config)
{
  var currentFont = engine.getOption("font");

  if (!config.talks)
    config.talks = ":/audio/wavs_citizen_en.zip";

  g_session.setLanguage(config.ext,config.talks);

  if (confgi.font != undefined && currentFont != config.font)
  {
    engine.setOption("font",config.font);
    g_session.setFont(config.font);
  }
}

function OnShowLanguageDialog()
{
  var g_ui = new Ui();

  var langModel = [
                    { lang : "English",     ext : "en", },
                    { lang : "Русский",      ext : "ru", talks : ":/audio/wavs_citizen_ru.zip" },
                    { lang : "Українська",    ext : "ua", },
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
                    { lang : "Cрпски"    ,   ext : "sb", },
                    { lang : "Korean"     , ext : "kr", font : "HANBatangB.ttf" }
                  ];

  var wnd = g_ui.addWindow(0);
  wnd.geometry = { x:0, y:0, w:512, h:384 };
  wnd.moveToCenter();
  wnd.closeAfterKey( {escape:true, rmb:true} );

  var listbox = wnd.addListbox(15, 40, wnd.width-30, wnd.height-90);
  listbox.setTextAlignment("center", "center");
  listbox.background = true;
  listbox.onSelectedCallback = function(index) {
                                                 setLanguage(langModel[index]);
                                               }

  for (var i in langModel)
  {
    var index = listbox.addLine(langModel[i].lang);
    listbox.setData( index, "lang", langModel[i].ext );
  }

  var currentLang = engine.getOption("language");
  listbox.selectedWithData = { name:"lang", data:currentLang };

  var btn = wnd.addButton(15, wnd.height - 40, wnd.width-30, 24);
  btn.text = "##continue##";
  btn.callback = function () {
                  g_session.reloadScene();
                  wnd.deleteLater();
                }
}

function OnShowLogs()
{
  var logfile = g_session.logfile();
  if (!logfile.exist())
    g_ui.addInformationDialog( "", "Can't found logfile" );
  else
    g_session.openUrl(logfile.str());
}

function OnChangePlayerName(force,continueCallback)
{
  var playerName = engine.getOption("playerName");
  if (playerName === null || playerName.length === 0)
    playerName = "##new_governor##";

  if (playerName.length === 0 || force)
  {
    var wnd = g_ui.addWindow(0);
    wnd.geometry = { x:0, y:0, w:380, h:128 }
        wnd.closeAfterKey( {escape:true} );
    wnd.mayMove = false;
    wnd.title = "##enter_your_name##";

    var exitFunc =  function() {
                                  wnd.deleteLater();
                                  if(continueCallback)
                                      continueCallback();
                               }

    var editbox = wnd.addEditbox(32, 50, 320, 32);
    editbox.textAlign = { h:"upperLeft", v:"center" };
    editbox.font = "FONT_2_WHITE";
    editbox.textOffset = { x:20, y:0 };
    editbox.tooltip = "##enter_dinasty_name_here##";
    editbox.text = playerName;
    editbox.cursorPos = playerName.length;
    editbox.onEnterCallback = exitFunc;
    editbox.onTextChangeCallback = function(text) { engine.setOption( "playerName", text ); }

    var lbNext = wnd.addLabel(180, 90, 100, 20);
    lbNext.text = "##plname_continue##";
    lbNext.font = "FONT_2";
    lbNext.textAlign = { h:"lowerRight", v:"center" };

    var btnContinue = wnd.addTexturedButton(300, 85, 27, 27);
    btnContinue.states = { rc:"paneling", normal:179, hover:180, pressed:181, disabled:179 };
    btnContinue.tooltip = "##plname_start_new_game##";
    btnContinue.callback = exitFunc;

    var lbExitHelp = wnd.addLabel(18, 90, 200, 20);
    lbExitHelp.text = "##press_escape_to_exit##";
    lbExitHelp.font = "FONT_1";

    wnd.moveToCenter();
    wnd.setModal();

    editbox.setFocus();
  }
}

function addMainMenuButton(caption,callback)
{
  var buttonSize = { w:200, h:25 };
  var btnFont = "FONT_2";
  var offsetY = 40;

  var button = new Button(0);
  button.geometry = { x:0, y:0, w:buttonSize.w, h:buttonSize.h };
  //button.setBackgroundStyle( style );
  button.font = btnFont;
  button.text = caption;
  if (callback)
  {
      engine.log(callback.toString());
      button.callback = callback;
  }

  g_lobbyButtons.push(button);

  var resolution = g_session.resolution;
  var offset = { x:(resolution.w - buttonSize.w) / 2, y:(resolution.h - offsetY * g_lobbyButtons.length) / 2 };
  for (var i in g_lobbyButtons)
  {
     button = g_lobbyButtons[i];
     button.position = offset;
     offset.y += offsetY;
     engine.log( "i:" + i + "x:"+offset.x + " y:" + offset.y);
  }
}

function mainMenuClear()
{
  for (var i in g_lobbyButtons)
    g_lobbyButtons[i].deleteLater();
}

function OnContinuePlay()
{
    engine.log("OnContinuePlay");
}

function OnShowNewGameMenu()
{
    engine.log("OnShowNewGameMenu");
}

function OnShowLoadGameMenu()
{
    engine.log("OnShowLoadGameMenu");
}

function OnShowGameOptionsMenu()
{
    engine.log("OnShowGameOptionsMenu");
}

function OnShowGameCredits()
{
    engine.log("OnShowGameCredits");
}

function OnShowAdvancedMaterials()
{
    engine.log("OnShowAdvancedMaterials");
}

function OnShowMainMenu()
{
  mainMenuClear();

  var lastGame = engine.getOption("lastGame");
  if(!lastGame)
      addMainMenuButton("##mainmenu_continueplay##", OnContinuePlay);

  addMainMenuButton("##mainmenu_newgame##", OnShowNewGameMenu);
  addMainMenuButton("##mainmenu_load##",    OnShowLoadGameMenu);
  addMainMenuButton("##mainmenu_options##", OnShowGameOptionsMenu);
  addMainMenuButton("##mainmenu_credits##", OnShowGameCredits);

  var dlc = new Path(":/dlc");
  if (dlc.exist)
     addMainMenuButton("##mainmenu_mcmxcviii##", OnShowAdvancedMaterials);

  addMainMenuButton("##mainmenu_quit##",         g_session.quitGame);

  OnShowChanges(false);
}

function OnLobbyStart()
{
  engine.log( "JS:OnLobbyStart" );
  //g_session.clearUi();

  var screen = g_session.resolution;

  var btnHomePage = new TexturedButton(0);
  var startx = screen.w/2 - 128 - 32;
  btnHomePage.geometry = { x:startx, y:screen.h - 70, w:64, h:64 };
  btnHomePage.states = { rc:"logo_rdt", normal:1, hover:2, pressed:2, disabled:2 };
  btnHomePage.callback = function() { g_session.openUrl( "http://www.caesaria.net" ); }
  btnHomePage.tooltip = "Open website in browser";

  var btnSteamPage = new TexturedButton(0);
  btnSteamPage.geometry = { x:startx + 64, y:screen.h - 70, w:64, h:64 };
  btnSteamPage.states = { rc:"steam_icon", normal:1, hover:2, pressed:2, disabled:2 };
  btnSteamPage.callback = function() { g_session.openUrl("http://store.steampowered.com/app/327640"); }
  btnSteamPage.tooltip = "Open steam store page in browser";

  var btnIrcPage = new TexturedButton(0);
  btnIrcPage.geometry = { x:startx + 64*2, y:screen.h - 70, w:64, h:64 };
  btnIrcPage.states = { rc:"irc_rdt", normal:1, hover:2, pressed:2, disabled:2 };
  btnIrcPage.callback = function() { g_session.openUrl( "http://webchat.quakenet.org/?channels=caesaria" ); }
  btnIrcPage.tooltip = "Open developer's chat in browser";

  var btnReportBugPage = new TexturedButton(0);
  btnReportBugPage.geometry = { x:startx + 64*3, y:screen.h - 70, w:64, h:64 };
  btnReportBugPage.states = { rc:"breport", normal:1, hover:2, pressed:2, disabled:2 };
  btnReportBugPage.callback = function() { g_session.openUrl( "https://bitbucket.org/dalerank/caesaria/issues/new" ); }
  btnReportBugPage.tooltip = "Report about bug";

  var btnTranslationPage = new TexturedButton(0);
  btnTranslationPage.geometry = { x:startx + 64*4, y:screen.h - 70 , w:64, h:64 };
  btnTranslationPage.states = { rc:"translation", normal:1, hover:2, pressed:2, disabled:2 };
  btnTranslationPage.callback = function() { g_session.openUrl( "https://docs.google.com/spreadsheets/d/1vpV9B6GLUX5G5z3ftucBFl7pXr-I0QvHPI9vW6K4xlY" ); }
  btnTranslationPage.tooltip = "Help with translation!";

  OnShowMainMenu();
}

function OnShowCredits()
{
    g_session.playAudio( "combat_long", 50, "theme" );

    var fade = g_ui.addFade(0xA0);
    fade.addCloseCode(0x1B); //escape
  fade.addCloseCode(0x4);//right mouse button

    var credits = g_session.credits;
  for( var i=0; i < credits.length; i++ )
  {
    var lb = new Label(fade.widget);
        lb.geometry = { x:0, y:fade.height + i * 15, w:fade.width, h:15};
        lb.text = credits[i];
    lb.textAlign = { h:"center", v:"center" };
    lb.font = "FONT_2_WHITE";
        lb.subElement = true;

    var animator = new PositionAnimator(lb.widget);
        animator.removeParent = true;
        animator.destination = {x:0, y:-20};
    animator.speed = {x:0, y:-0.5};
  }

  var btnExit = new Button(fade.widget);
    btnExit.geometry = {x:fade.width - 150, y:fade.height-34, w:140, h:24};
    btnExit.text = "##close##";
    btnExit.callback = function() {
                                    fade.deleteLater();
                                    g_session.playAudio( "main_menu", 50, "theme" );
                                  }
}
