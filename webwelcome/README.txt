Web Welcome Applet
====================
- Goal: Greet and welcome the user, guide through the setup of some of the most important webservices

- Setup:

Applet
    - loads list of plugins as ServiceButtons

ServiceButton
    - loaded from KPluginInfo

services/
- logo.png: logo image of the webservice, size 120x40 (wxh)
- metadata.desktop
    - Servicetype: Silk/WebService
    - Comment: ends up in tooltip
    - Name: ends up in lists where the logo.png isn't used, e.g. icon views
    - config.ui: configuration UI for the service setup
    - WebServiceKCM
        - configuration module for a specific webservice
        - load with "kcmshell4 webservice_twitter" for example
        -


