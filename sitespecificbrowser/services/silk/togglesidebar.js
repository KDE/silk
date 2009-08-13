/* ------------------------------------------------- *
 * Show/Hide the header, footer and sidebar          *
 *                                                   *
 * Site:    Gitorious.org                            *
 * Author:  Sebastian Kügler <sebas@kde.org>         *
 * License: LGPL v2.1                                *
 * ------------------------------------------------- */

function toggleVisibility(id)
{
    el = document.getElementById(id);
    if (el.style.display == 'none')
    {
        el.style.display = '';
        window.silk.GM_log('showing');
    } else {
        el.style.display = 'none';
        window.silk.GM_log('hiding');
    }
}

toggleVisibility('sidebar');
toggleVisibility('header');
toggleVisibility('footer');

// Evil hack to force a reflow since Qt 4.5.1 doesn't do it
var v = document.body.offsetHeight;
