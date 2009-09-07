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
        window.silk.showNotification('Trigger Showing ...');
        window.silk.GM_log("trigger showing");
    } else {
        el.style.display = 'none';
        window.silk.showNotification('Trigger Hiding ...');
        window.silk.GM_log("trigger hiding");
    }
}

toggleVisibility('header');
toggleVisibility('footer');

// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
var v = document.body.offsetHeight;
