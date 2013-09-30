# NFK Lobby

NFK Lobby is a lobby application for a video game called [Need For Kill](http://info.needforkill.ru/).

It mainly of three major parts:

### Planet Scanner

Displays list of current games.

  * Works with several planets simultaneously
  * Filters output by gametype and fullness of a server
  * Allowes to join a game as a player or a spectator
  * Grabs player list from http://nfk.pro2d.ru/
  * Automatically refreshes information
     

![Planet Scaner Screenshot](http://i.imgur.com/ndKBFVe.png)
<p align="center">Image is clickable</p>


### Chat

Implementation of IRC client.

  * Identifies with NickServ
  * Automatically connects to a server on the startup
  * Automatically joins specified channels
  * Waits specified time before automatically joining specified channels (useful when using cloak)
  * Allows to use custom fonts
  * Allows to add any servers with any supported encoding
  * Uses [Quassel](http://quassel-irc.org/)-like message coloring
     

![Chat Screenshot](http://i.imgur.com/ju1W8BC.png)
<p align="center">Image is clickable</p>


### Game Preferences

Shares game preferences among players.

  * Allows players to share gametypes they want to play, instead of asking "who wants to play X?" every single time in the chat
  * Fun fact: Built on top of IRC
     

![Game Preferences Screenshot](http://i.imgur.com/1AKIDHZ.png)
<p align="center">Image is clickable</p>



## More Screenshots

### Using Docks

All three tabs are actually docks, which means that they can be detached and be moved inside the application window.
Here is an example of how that might be useful:

![Using Docks Screenshot](http://i.imgur.com/vf4mF7H.png)
<p align="center">Image is clickable</p>


### Settings

Want even more screenshots? Here is [a gallery of settings screenshots](http://imgur.com/a/iLY9N) just for you.


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
