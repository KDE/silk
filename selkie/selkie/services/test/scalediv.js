/* ------------------------------------------------- *
 * Show/Hide the header, footer and sidebar          *
 *                                                   *
 * Site:    Selkie Test Site                         *
 * Author:  Sebastian K?gler <sebas@kde.org>         *
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

setStyle("content", "-webkit-transition", "-webkit-transform .4s ease");
//document.getElementById("picture").style.webkitTransform='rotate(1080deg)'
window.silk.GM_log("scaling down content");

document.getElementById("content").style.webkitTransform='scale(.3)';
//document.getElementById("mountain").style.webkitTransform='rotate(360deg)';

setTimeout("scaleUp()", 400);

// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
//var v = document.body.offsetHeight;
