Web Welcome Applet
====================
- Goal: Greet and welcome the user, guide through the setup of some of the most important webservices

The web welcome widget is a widget that sits on the desktop at first login, allowing the user to quickly set up some popular web services to fully integrate into KDE. Examples are Twitter, GMail, Wikipedia. For these services, addons are configured for some applications, links added to the favourites, data imported and accounts set up. The result is a workspace which already integrates the user's ecosystem, and simplifies the setup.


Architecture
=============

Plasma Applet
    - loads list of plugins as ServiceButtons
    - classes
        - Applet (the Plasmoid, it holds the QGraphicsWidget Dialog)
        - Dialog holds the actual widget, containing the webview and the servicebuttons, the dialog queries for service plugins and creates a servicebutton for every installed webservice setup found
        - Stylesheet:
            - The stylesheet is used to theme the webwidget holding the introductory message
            - The stylesheet parsing is available in Plasma::Theme::styleSheet() as of 4.5, so this can be ported (and thus cleaned up) once we dare depending on it, These services are groupd under the ServiceType Silk/WebService
        - ServiceButton
            - shows pretty picture of the service
            - loaded using KPluginInfo

- /services/$something/Kcm*
    - Holds a configuration (traditional QWidget) interface with a couple of options, sane defaults chosen, see below under Capabilities
    - these config interfaces are KCMs, so we can create a simple "Sytem Settings" category or app as well with it
    - might be interesting to embed these KCMs into the Plasma Widget
- logo.png: logo image of the webservice, size 120x40 (wxh)
- webservice.desktop
    - Servicetype: Silk/WebService
    - Comment: ends up in tooltip
    - Name: ends up in lists where the logo.png isn't used, e.g. icon views
    - config.ui: configuration UI for the service setup
    - load with "kcmshell4 webservice_twitter" for example


Capabilities of the KCMs
=========================


Wikipedia
----------
priority:
[ ] Enable Wikipedia Search using "wiki <searchterm>" in KRunner
[x] wp:shortcut in web browser leads to Wikipedia

nice to have:
[ ] Install Wikipedia standalone app (needs selkie)

Twitter
-------
[x] Add Twitter Widget to the desktop
[x] Add choqok to Favourite Application
user: [    ]
pass: [*** ]

GMail
-----
priority:
[x] Setup KMail to as Email client (pending KMail based on Akonadi)
[ ] Make my GMail emails, contacts and calendaring information available offline
[ ] Make my contacts searchable in KRunner
user: [    ]
pass: [*** ]

Facebook
--------


FlickR
-------

YouTube
--------
[ ] Add minitube to my favourite applications
[ ]