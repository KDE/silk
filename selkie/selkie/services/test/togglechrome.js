/* ------------------------------------------------- *
 * Show/Hide the header, footer                      *
 *                                                   *
 * Site:    Selkie Test Site                         *
 * Author:  Sebastian K?gler <sebas@kde.org>         *
 * License: LGPL v2.1                                *
 * ------------------------------------------------- */

duration = 300;

function toggleVisibility(id)
{
    window.silk.GM_log("toggleVisibility");
    duration = 300;
    setStyle(id, "-webkit-transition", "-webkit-transform " + duration + "ms ease");
    el = document.getElementById(id);
    if (el.style.display == 'none')
    {
        el.style.display = '';
        //window.silk.showNotification('Trigger Showing ...');
        window.silk.GM_log("trigger showing");
        return 1;
    } else {
        el.style.display = 'none';
        //window.silk.showNotification('Trigger Hiding ...');
        window.silk.GM_log("trigger hiding");
        return 0;
    }
}

function setStyle(objId, style, value) {
   document.getElementById(objId).style[style]= value;
}

function toggle(id) {
    window.silk.GM_log("toggle" + id);
    el = document.getElementById(id);
    duration = 300;
    setStyle(id, "-webkit-transition", "-webkit-transform " + duration + "ms ease");
    if (el.style.display == 'none')
    {
        el.style.display = '';
        el.style.webkitTransform='scaleY(1)';
        window.silk.GM_log("scaling in " + id);
    } else {
        el.style.webkitTransform='scaleY(0)';
        window.silk.GM_log("scaling out " + id);
        setTimeout("toggleVisibility(\'" + id + "\')", duration);
    }
}

toggle("header");
toggle("footer");
toggle("box");
// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
var v = document.body.offsetHeight;
