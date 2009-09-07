/* ------------------------------------------------- *
 * Show/Hide the header, footer and sidebar          *
 *                                                   *
 * Site:    Selkie Test Site                         *
 * Author:  Sebastian Kügler <sebas@kde.org>         *
 * License: LGPL v2.1                                *
 * ------------------------------------------------- */

function toggleVisibility(id)
{
    el = document.getElementById(id);
    if (el.style.display == 'none')
    {
        el.style.display = '';
        window.silk.showNotification('Toolbar: Showing ...');
        window.silk.GM_log("showing from toolbar" + id);
    } else {
        el.style.display = 'none';
        window.silk.showNotification('Toolbar: Hiding ...');
        window.silk.GM_log("hiding from toolbar" + id);
    }
}


/* USAGE:
 * objId    = element id.
 * style    = the style to be changed.
 * value    = the value assigned to the style.
*/
function setStyle(objId, style, value) {
   document.getElementById(objId).style[style]= value;
}

function scaleUp() {
    id = "mountains";
    document.getElementById(id).style.webkitTransform='scale(1)';
    window.silk.GM_log("scaling up" + id);

}

setStyle("mountains", "-webkit-transition", "-webkit-transform 0.2s ease-in");
//document.getElementById("mountains").style.webkitTransform='rotate(10000deg)'
//document.getElementById("mountains").style.webkitTransform='scale(4)'

document.getElementById("mountains").style.webkitTransform='scale(.25)'
//document.getElementById("mountains").style.webkitTransform='skewY(50)'
//toggleVisibility('mountains');

setTimeout("scaleUp()", 200);

// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
var v = document.body.offsetHeight;
