# NFK Lobby

NFK Lobby is a lobby application for a video game called [Need For Kill](http://info.needforkill.ru/).

It consists of three modules:

### Planet Scanner

"Planet" is an online game registry server -- the server with which all created online games register, and which the players looking for online games query.

The Planet Scanner module communicates with a Planet and displays a list of current games.

  * Works with several planets simultaneously
  * Filters output by gametype and fullness of a server
  * Allows to join a game as a player or a spectator in a few clicks
  * Has integration with http://nfk.pro2d.ru/ 3rd party service
    * For each game, it displays a list of players in the game by grabbing it from http://nfk.pro2d.ru/ (planet doesn't provide a player list information)
    * Allows to open player's profile on http://nfk.pro2d.ru/
  * Has option to periodically refresh the information


![Planet Scanner Screenshot](http://i.imgur.com/ndKBFVe.png)
<p align="center">Image is clickable</p>


### Chat

From scratch implementation of a IRC client, automatically connecting to the game's channel on startup.

  * Identifies with NickServ
  * Automatically connects to a server on the startup
  * Automatically joins specified channels
  * Allows to delay auto-joining channels for a specified period time (useful when using a cloak)
  * Allows to configure fonts
  * Allows to add any IRC servers
  * Allows to use any character encoding supported by Qt
  * Uses [Quassel](http://quassel-irc.org/)-like message coloring


![Chat Screenshot](http://i.imgur.com/ju1W8BC.png)
<p align="center">Image is clickable</p>


### Game Preferences

A global board where players can share their game preferences.

  * Allows players to share gametypes they want to play, instead of asking "who wants to play X?" every few minutes in the chat
  * Built on top of IRC, i.e. doesn't require any extra server


![Game Preferences Screenshot](http://i.imgur.com/1AKIDHZ.png)
<p align="center">Image is clickable</p>



## More Screenshots

### Using Docks

All three tabs are actually docks, which means that they can be detached and be moved inside the application window.
Here is an example of how that might be useful:

![Using Docks Screenshot](http://i.imgur.com/vf4mF7H.png)
<p align="center">Image is clickable</p>


### Settings

Want to see even more screenshots? Here is [a gallery of settings screenshots](http://imgur.com/a/iLY9N).


## Contributing

* Create an issue describing what you want to do and wait for a reply

* [Fork the repo](https://help.github.com/articles/fork-a-repo)

* Make your changes

* [Submit a pull request](https://help.github.com/articles/using-pull-requests)

## Contact

Should you have any question you can:
  * Find me on Need For Kill IRC channel [irc://irc.wenet.ru:6667/nfk](irc://irc.wenet.ru:6667/nfk)
  * Contact me by email: `nurupo dot contributions at gmail dot com`.

## License

NFK Lobby is licensed under [GPLv3](COPYING).

Note that icons are licensed [differently](/resources/icons/LICENSE).
