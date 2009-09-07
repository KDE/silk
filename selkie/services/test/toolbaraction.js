/* ------------------------------------------------- *
 * Show/Hide the header, footer and sidebar          *
 *                                                   *
 * Site:    Selkie Test Site                         *
 * Author:  Sebastian Kügler <sebas@kde.org>         *
 * License: LGPL v2.1                                *
 * ------------------------------------------------- */

/* USAGE:
 * objId    = element id.
 * style    = the style to be changed.
 * value    = the value assigned to the style.
*/
function setStyle(objId, style, value) {
   document.getElementById(objId).style[style]= value;
}

function scaleUp() {
    id = "content";
    document.getElementById(id).style.webkitTransform='scale(1)';
    window.silk.GM_log("scaling up " + id);

}

setStyle("content", "-webkit-transition", "-webkit-transform .2s ease");
//document.getElementById("mountains").style.webkitTransform='rotate(1080deg)'

document.getElementById("content").style.webkitTransform='scale(.8)';

setTimeout("scaleUp()", 200);

// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
var v = document.body.offsetHeight;
