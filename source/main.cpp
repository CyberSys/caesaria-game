// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#include "core/utils.hpp"
#include "core/exception.hpp"
#include "vfs/directory.hpp"
#include "game/settings.hpp"
#include "game/game.hpp"
#include "core/logger.hpp"
#include "core/stacktrace.hpp"
#include "core/osystem.hpp"
#include "steam.hpp"

#ifdef GAME_PLATFORM_WIN
  #undef main
#endif

#ifdef GAME_PLATFORM_ANDROID
#include <SDL.h>
#include <SDL_system.h>
#endif

int main(int argc, char* argv[])
{
  crashhandler::install();

  vfs::Directory workdir;
#ifdef GAME_PLATFORM_ANDROID
  workdir  = vfs::Path( SDL_AndroidGetExternalStoragePath() );
#else
  workdir = vfs::Path( argv[0] ).directory();
#endif

  game::Settings& options = game::Settings::instance();
  Logger::registerWriter(Logger::consolelog, "");

  options.setwdir(workdir.toString());
  bool wdirChanged = options.checkwdir( argv, argc );
  if (wdirChanged)
    workdir = SETTINGS_STR(workDir);

  options.resetIfNeed(argv, argc);
  Logger::registerWriter(Logger::filelog, workdir.toString());

  Logger::info("Setting workdir to " + SETTINGS_STR(workDir));

  Logger::info("Loading game settings");
  if (options.haveLastConfig())
    options.loadLastConfig();

  options.checkCmdOptions(argv, argc);
  options.checkC3present();

  if (!KILLSWITCH(verbose))
  {
    Logger::addFilter(LogWriter::info);
    Logger::addFilter(LogWriter::debug);
  }

  std::string systemLang = SETTINGS_STR(language);

  if (steamapi::available())
  {
    if (!steamapi::connect())
    {
      Logger::fatal("Failed to connect to steam");
      return EXIT_FAILURE;
    }

    if (systemLang.empty())
      systemLang = steamapi::language();
  }

  options.changeSystemLang(systemLang);

  Logger::info("Language set to " + SETTINGS_STR(language));
  Logger::info("Using native C3 resources from " + SETTINGS_STR(c3gfx));
  Logger::info("Cell width set to " + SETTINGS_VALUE(cellw).toInt());

  try
  {
    Game game;
    game.initialize();
    while( game.exec() );

    game.destroy();
  }
  catch( Exception& e )
  {
    Logger::fatal("Critical error: " + e.getDescription());
    crashhandler::printstack();
  }

  if (steamapi::available())
    steamapi::close();

  crashhandler::remove();

  return 0;
}
